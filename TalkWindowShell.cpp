#include "TalkWindowShell.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include "CustomProxyStyle.h"
#include <QDebug>
#include <QFile>
#include <qmessagebox.h>
#include "UseMySQL.h"
#include <QTextStream>
#include <QJsonValue>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include "WindowManager.h"
#include <qjsonarray.h>
#include "RecvFile.h"
extern QString LoginUserAccount;//定义于CCMainWindow.cpp

TalkWindowShell::TalkWindowShell(QWidget *parent)
	: BasicWindow(parent), emotionWindow(nullptr)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	init();
}

TalkWindowShell::~TalkWindowShell()
{
	if (emotionWindow)
	{
		emotionWindow->deleteLater();
	}
}

void TalkWindowShell::init()
{
	initStyle();
	connect(ui.listWidget,&QListWidget::itemClicked,this,&TalkWindowShell::onListItemClicked);
	initEmotionWindow();
	initJsFile();
	initTcpSocket();
	initUdpSocket();
}

void TalkWindowShell::initStyle()
{
	loadStyleSheet("TalkWindow");
	ui.listWidget->setStyle(new CustomProxyStyle());

	ui.splitter->setSizes(QList<int>{154,width()-154});
}

void TalkWindowShell::initEmotionWindow()
{
	emotionWindow = new EmotionWindow;
	emotionWindow->hide();

	connect(emotionWindow,&EmotionWindow::addEmotion,this,&TalkWindowShell::onaddEmotion);
}

void TalkWindowShell::initJsFile()
{
	//读取msgPageTmp.txt
	QFile tempFile(":/Resources/MainWindow/MsgHtml/msgPageTmp.txt");
	QString tempFileContent;
	if (tempFile.open(QFile::ReadOnly))
	{
		tempFileContent = tempFile.readAll();
		tempFile.close();
	}
	else
	{
		QMessageBox::information(nullptr,"error","Read msgPageTmp error");
		return;
	}
	if (!replaceJsTxt(tempFileContent))
	{
		return;
	}

	QFile js("./Resources/MainWindow/MsgHtml/msghandling.js");
	if (js.open(QFile::WriteOnly | QFile::Truncate))
	{
		QTextStream stream(&js);
		stream << tempFileContent;
		js.close();
		return;
	}
	else
	{
		QMessageBox::information(nullptr, "error", "Write msgHanding.js error");
		return;
	}
}

bool TalkWindowShell::replaceJsTxt(QString& fileContent)
{
	//需要替换的文本
	QString replaceVar = "var external = null;";//变量定义
	QString replaceObj = "external = channel.objects.external;";//共享对象赋值
	QString replaceRecv;
	QFile recvFile(":/Resources/MainWindow/MsgHtml/recvHtmlTmp.txt");
	if (recvFile.open(QFile::ReadOnly))
	{
		replaceRecv = recvFile.readAll();
		recvFile.close();
	}
	else
	{
		QMessageBox::information(nullptr, "error", "Read recvHtmlTmp error");
		return false;
	}
	//获取用户列表
	QList<int> userList = UseMySQL::instance()->getUserList();

	//替换文本内容
	QString var, obj, recv;
	for (int id : userList)
	{
		QString tmpVar = replaceVar;
		tmpVar.replace("external",QString("external_%1").arg(id));
		var += (tmpVar + "\n");

		QString tmpObj = replaceObj;
		tmpObj.replace("external", QString("external_%1").arg(id));
		obj += (tmpObj + "\n");

		QString tmpRecv = replaceRecv;
		tmpRecv.replace("recvHtml", QString("recvHtml_%1").arg(id));
		tmpRecv.replace("external", QString("external_%1").arg(id));
		recv += (tmpRecv + "\n");
	}
	fileContent.replace(replaceVar, var);
	fileContent.replace(replaceObj, obj);
	fileContent.replace(replaceRecv, recv);

	return true;

}

void TalkWindowShell::initTcpSocket()
{
	tcp = new QTcpSocket(this);
	tcp->connectToHost("127.0.0.1",tcpPort);
}

void TalkWindowShell::initUdpSocket()
{
	udp = new QUdpSocket(this);
	if (!udp->bind(udpPort, QAbstractSocket::ReuseAddressHint))
	{
		QMessageBox::information(nullptr, "error", "Udp Socket error");
		return;
	}
	connect(udp,&QUdpSocket::readyRead,this,&TalkWindowShell::readMsgFromServer);
}

void TalkWindowShell::addTalkWindow(TalkWindow* window, TalkWindowItem* windowItem)
{
	ui.rightStackedWidget->addWidget(window);//聊天窗口添加到堆栈部件
	ui.rightStackedWidget->setCurrentWidget(window);

	QListWidgetItem* item = new QListWidgetItem(ui.listWidget);
	ui.listWidget->addItem(item);//添加到列表部件
	ui.listWidget->setItemWidget(item, windowItem);//设置列表项的部件
	item->setSelected(true);//选中列表项

	map.insert(item, window);//保存列表项和窗口的映射

	connect(windowItem, &TalkWindowItem::signalCloseBtnClicked, this, [window, windowItem, item, this]()
		{
			map.remove(item);//移除列表项和窗口的映射

			//listWidget
			ui.listWidget->takeItem(ui.listWidget->row(item));
			delete item;
			windowItem->setParent(nullptr);
			windowItem->deleteLater();

			//StackedWidget
			ui.rightStackedWidget->removeWidget(window);
			window->setParent(nullptr);
			window->deleteLater();

			//为什么要多此一举设置父对象为空指针？
			//qDebug() << ui.rightStackedWidget->count();
			if (ui.rightStackedWidget->count() == 0)
			{
				//如果没有窗口了，关闭窗口

				this->close();
			}
		}
	);
	
	
}

void TalkWindowShell::updateCurrentWindow(QWidget* widget)
{
	QListWidgetItem *item =  map.key(widget,nullptr);
	if (item)
	{
		item->setSelected(true);
		ui.rightStackedWidget->setCurrentWidget(widget);

	}
}

void TalkWindowShell::onEmotionBtnClicked()
{
	//表情按钮点击之后，显示/隐藏表情窗体
	emotionWindow->setVisible(!emotionWindow->isVisible());

	//显示表情窗口
	QPoint pos = this->mapToGlobal(QPoint(0, 0));//将窗体坐标转换为全局坐标
	QPoint movepos = pos + QPoint(170, 220);
	emotionWindow->move(movepos);
}

void TalkWindowShell::onaddEmotion(int name)
{
	emotionWindow->hide();
	//获取当前窗口
	TalkWindow* window = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	if (window)
	{
		window->addEmotionImage(name);
	}
}

void TalkWindowShell::sendMsgToServer(QJsonValue data)
{
	TalkWindow* window = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());//获取当前窗口
	const QString talkID = QString::number(window->getTalkID());

	QJsonObject msgObj;
	//聊天窗口类型
	if (talkID.length() <= GROUP_ID_LENGTH)
	{
		msgObj.insert(TYPE, TYPE_GROUP);
	}
	else
	{
		msgObj.insert(TYPE, TYPE_SINGLE);
	}
	//组装发送者&接收者
	msgObj.insert(SEND,LoginUserAccount);
	msgObj.insert(RECV, talkID);

	//组装消息内容
	switch (data.type())
	{
		case QJsonValue::Array://普通消息
		{
			msgObj.insert(WHAT,WHAT_MSG);
			msgObj.insert(WHAT_MSG,data);
			break;
		}
		case QJsonValue::Object:
		{
			msgObj.insert(WHAT, WHAT_FILE);
			msgObj.insert(WHAT_FILE, data);
			break;
		}
		default:
		{
			QMessageBox::information(nullptr, "error", "Message Type error");
			return;
		}
	}

	//发送消息给服务器
	QString msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	qDebug() << msg;
	if (tcp->state() != QAbstractSocket::ConnectedState)
	{
		QMessageBox::information(nullptr, "error", "TcpSocket connect error");
		return;
	}
	tcp->write(msg.toUtf8());
}

void TalkWindowShell::readMsgFromServer()
{
	while (udp->hasPendingDatagrams())//判断udp端口中是否有未处理的数据
	{
		QByteArray msg(udp->pendingDatagramSize(),0);
		udp->readDatagram(msg.data(), msg.size());

		QJsonDocument jsonDoc = QJsonDocument::fromJson(msg);
		if (jsonDoc.isNull() || !jsonDoc.isObject())//json格式错误
		{
			return;
		}
		QJsonObject jsonObj = jsonDoc.object();//返回文档中包含的json对象

		//解析json对象
		QString sender = jsonObj[SEND].toString();//发送者
		if (sender == LoginUserAccount) return;//自己发送的消息不做处理

		QString type = jsonObj[TYPE].toString();//聊天窗口类型
		QString recviver = jsonObj[RECV].toString();//接收者(群号/员工ID)

		QString talkID = recviver;//默认为群聊
		if (type == TYPE_SINGLE)//如果是单聊
		{
			if (recviver != LoginUserAccount) return;//接收者不是自己，直接返回
			talkID = sender;
		}
		//获取聊天窗口
		QWidget* widget =  WindowManager::instance()->findWindowMap(talkID.toInt());
		if (!widget) return;
		this->updateCurrentWindow(widget);//更新当前窗口

		//获取消息类型
		QString what = jsonObj[WHAT].toString();
		QJsonValue data = jsonObj[what];//消息内容

		if (what == WHAT_MSG)//普通消息
		{
			QJsonArray masArray = data.toArray();
			dynamic_cast<TalkWindow*>(widget)->appendObjectMsg(masArray, sender);//添加消息
		}
		else if (what == WHAT_FILE)//文件消息
		{
			QJsonObject fileObj = data.toObject();
			RecvFile* recvFile = new RecvFile(fileObj, this);
			QString msg = QString::fromUtf8("收到来自") + UseMySQL::instance()->getUserName(sender) + QString::fromUtf8("的文件,是否接收");
			recvFile->setLabelMsg(msg);
			recvFile->show();
		}
	}
}

void TalkWindowShell::onListItemClicked(QListWidgetItem* item)
{
	QWidget* widget = map.value(item, nullptr);
	if (widget)
	{
		ui.rightStackedWidget->setCurrentWidget(widget);
	}
}
