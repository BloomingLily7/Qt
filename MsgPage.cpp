#include "MsgPage.h"
#include <qfile.h>
#include "define.h"
#include <QMessageBox>
#include "UseMySQL.h"
#include <qwebchannel.h>
#include "WindowManager.h"
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonvalue.h>
extern QString LoginUserAccount;//定义于CCMainWindow.cpp

MsgHtmlObj::MsgHtmlObj(QObject* parent, QString headPath)
	: QObject(parent), leftHeadPath(headPath)
{
	initMsgHtml();
}

MsgHtmlObj::~MsgHtmlObj()
{}

void MsgHtmlObj::initMsgHtml()
{
	//右边显示发送消息
	msgRight = getMsgHtml("userRight");
	msgRight.replace("%1",UseMySQL::instance()->getUserHeadPixmap(LoginUserAccount));

	//左边显示接收消息
	msgLeft = getMsgHtml("userLeft");
	msgLeft.replace("%1", leftHeadPath);

}

QString MsgHtmlObj::getMsgHtml(const QString& fileName)
{
	QFile file(HTML_PATH(fileName));
	QString fileContent;
	if (file.open(QFile::ReadOnly))
	{
		fileContent = file.readAll();
		file.close();
	}
	else
	{

		QMessageBox::information(nullptr, "error", QString("Write %1.html error").arg(fileName));
		
	}
	return fileContent;
}

MsgWebView::MsgWebView(QWidget* parent):QWebEngineView(parent),rightObj(nullptr)
{
	this->setContextMenuPolicy(Qt::NoContextMenu);//禁止右键弹出菜单

	MsgWebPage* page = new MsgWebPage(this);//初始化页面
	this->setPage(page);

	channel = new QWebChannel(this);//初始化通道
	this->page()->setWebChannel(channel);//设置通道

	this->load(QUrl(QString("qrc") + HTML_PATH("msgPage")));//加载页面

	registerChannelObject();//注册通道对象

	connect(this,SIGNAL(signalSendMsg(QJsonValue)),WindowManager::instance()->getWindowShell(),SLOT(sendMsgToServer(QJsonValue)));
}

MsgWebView::~MsgWebView()
{

}

void MsgWebView::appendMsg(const QJsonArray& jsonMsg, QString obj)
{
	QString msg = parseMsgFromJson(jsonMsg);

	QJsonObject msgObj;
	msgObj.insert("MSG",msg);

	//将jsonObject转换为紧凑格式的json字符串
	const QString jsonString = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	if (obj == MYSELF)
	{
		this->page()->runJavaScript(QString("appendHtml0(%1)").arg(jsonString));
		emit signalSendMsg(jsonMsg);
	}
	else
	{
		this->page()->runJavaScript(QString("recvHtml_%1(%2)").arg(obj).arg(jsonString));
	}
}

void MsgWebView::registerChannelObject()
{
	//注册发送消息对象
	rightObj = new MsgHtmlObj(this);
	channel->registerObject("external0",rightObj);

	//注册接收消息对象
	int talkID = WindowManager::instance()->getCreateID();
	QList<QPair<QString, QString>> infoList;
	


	if (!UseMySQL::instance()->getRegisterObjectInfo(QString::number(talkID), infoList))
	{
		QMessageBox::information(nullptr, "error", "getRegisterObjectInfo Error.");
		return;
	}
	for (auto& info : infoList)
	{
		QString account = info.first;
		QString headPath = info.second;

		MsgHtmlObj* leftObj = new MsgHtmlObj(this, headPath);
		channel->registerObject(QString("external_%1").arg(account), leftObj);
	}
}

QString MsgWebView::parseMsgFromJson(const QJsonArray& arr)
{
	QString msg;
	for (int i = 0;i < arr.count();i++)
	{
		QJsonArray blcokArray = arr[i].toArray();//获取json数组
		for (int j = 0; j < blcokArray.count();j++)//组装文本块
		{
			QJsonObject obj = blcokArray[j].toObject();//片段Json对象
			QString key =  obj.keys().at(0);
			if (key == TXT)
			{
				msg += obj[key].toString();
			}
			else if (key == IMG)
			{
				QString value = QString(IMG_SRC).arg(IMG_QRC + obj[key].toString() + IMG_PNG);
				msg += value;
			}
		}
		msg += BR;//添加换行符
		//最后末尾会多一个BR标签，可以不用管，但是有两个的话会显示一个
	}
	return msg;
}
