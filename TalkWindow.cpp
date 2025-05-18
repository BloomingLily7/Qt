#include "TalkWindow.h"
#include "WindowManager.h"
#include <qtooltip.h>
#include "define.h"
#include <qjsonarray.h>
#include <qtextcursor.h>
#include <QTextBlock>
#include <qjsonobject.h>
#include "UseMySQL.h"
#include <qmessagebox>
#include "RootContactItem.h"
#include "ContactItem.h"
#include "CommonUtils.h"
#include "WindowManager.h"
#include "SendFile.h"

extern QString LoginUserAccount;

TalkWindow::TalkWindow(const int& id,QWidget *parent)
	: QWidget(parent), talkID(id)
{
	ui.setupUi(this);
	WindowManager::instance()->addWindowMap(talkID,this);
	//initTalkType();//初始化聊天类型
	init();

	
}

TalkWindow::~TalkWindow()
{
	WindowManager::instance()->removeWindowMap(talkID);
}

void TalkWindow::init()
{
	initStyle();
	initConnect();
	initTalkWindow();
}

//void TalkWindow::initTalkType()
//{
//	if (QString::number(talkID).length() <= GROUP_ID_LENGTH)
//	{
//		isGroupTalk = true;
//	}
//	else
//	{
//		isGroupTalk = false;
//	}
//}

void TalkWindow::initStyle()
{
	ui.bodySplitter->setSizes(QList<int>{600, 138});
	ui.textEdit->setFocus();
}

void TalkWindow::initConnect()
{
	//TalkWindow的父部件是TalkWindowShell
	connect(ui.sysmin,SIGNAL(clicked()),parent(),SLOT(onMin()));
	connect(ui.sysclose,SIGNAL(clicked()),parent(),SLOT(onClose()));
	connect(ui.closeBtn, SIGNAL(clicked()), parent(), SLOT(onClose()));

	connect(ui.faceBtn,SIGNAL(clicked()),parent(),SLOT(onEmotionBtnClicked()));

	connect(ui.sendBtn,&QToolButton::clicked,this,&TalkWindow::onSendBtnClicked);

	connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &TalkWindow::onTreeItemDoubleClicked);

	connect(ui.fileopenBtn,&QPushButton::clicked,this,&TalkWindow::onFileOpenBtnClicked);
}

void TalkWindow::initTalkWindow()
{
	if (QString::number(talkID).length() <= GROUP_ID_LENGTH)//群聊
	{
		initGroupTalk();
	}
	else
	{
		initSingleTalk();
	}
}

void TalkWindow::initGroupTalk()
{
	QTreeWidgetItem* rootItem = new QTreeWidgetItem();
	rootItem->setData(0, Qt::UserRole, 0);//根项Qt::UserRole角色下数据为0

	//获取群成员信息
	QList<QPair<QString, QString>> infoList;
	if (!UseMySQL::instance()->getRegisterObjectInfo(QString::number(talkID), infoList))
	{
		QMessageBox::information(nullptr, "error", "getRegisterObjectInfo Error.");
		return;
	}
	RootContactItem* rootWidget = new RootContactItem(false, ui.treeWidget);
	rootWidget->setText(QString::fromUtf8("%1 成员-%2").arg(UseMySQL::instance()->getGroupName(talkID)).arg(infoList.count()));

	ui.treeWidget->addTopLevelItem(rootItem);
	ui.treeWidget->setItemWidget(rootItem, 0, rootWidget);
	rootItem->setExpanded(true);//展开子项

	for (auto& info : infoList)
	{
		addMember(rootItem,info);
	}
}

void TalkWindow::initSingleTalk()
{
	QPixmap pixmap(":/Resources/MainWindow/skin.png");
	ui.widget->setFixedSize(pixmap.size());

	QLabel* label = new QLabel(ui.widget);
	label->setPixmap(pixmap);
	label->setFixedSize(ui.widget->size());

}

QJsonArray TalkWindow::parseDocumentToJson(QTextDocument* document)
{
	//遍历文档查找和检索内容
	QTextCursor cursor(document);
	cursor.movePosition(QTextCursor::Start);//光标移动到文档开头

	QJsonArray msgArray; //msg Json数组

	//检索文档内所有文本块
	while (!cursor.isNull())
	{
		QTextBlock block = cursor.block();	//当前文本块
		QTextBlock::Iterator it;//文本块迭代器

		QJsonArray blockArray;//文本块Json数组(msgArray的元素)
		for (it = block.begin();!(it.atEnd());it++)
		{
			QTextFragment fragment = it.fragment();
			if (fragment.isValid())
			{
				if (fragment.charFormat().isImageFormat())//获取当前片段文本格式
				{
					
					QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();//获取图片格式信息
					QJsonValue value = imageFormat.name().replace(IMG_QRC, QString()).replace(IMG_PNG, QString());//获取图片名称
					blockArray.append(QJsonObject{ {IMG, value} });//将image对象插入文本块数组内
				}
				else//文本
				{
					QString text = fragment.text();//获取片段纯文本
					text.replace("&", "&amp");
					text.replace("<","&lt");//不把<>识别为html标签
					text.replace((QChar)8232,BR);//软换行符替换为hmtl换行符<br>

					QJsonValue value = text;
					blockArray.append(QJsonObject{ {TXT, value} });
				}
			}
		}
		msgArray.append(blockArray);
		if (!cursor.movePosition(QTextCursor::NextBlock))//移动到下一个文本块
		{
			break;
		}
	}
	//qDebug() << "msgArray:" << msgArray;
	return msgArray;
}

void TalkWindow::addMember(QTreeWidgetItem* rootItem, const QPair<QString, QString>& pairs)
{
	//构造一个子项
	QTreeWidgetItem* childItem = new QTreeWidgetItem();
	childItem->setData(0, Qt::UserRole, 1);//子项UserRole角色下数据为1
	childItem->setData(0, Qt::UserRole + 1, pairs.first);//子项UserRole + 1角色下数据为员工ID

	ContactItem* childWidget = new ContactItem(ui.treeWidget);
	//头像
	QPixmap src(pairs.second);
	QPixmap mask(":/Resources/MainWindow/head_mask.png");//底片
	childWidget->setHeadPixmap(CommonUtils::getRoundPixmap(src, mask, childWidget->getHeadLabelSize()));//设置头像
	//设置用户名
	childWidget->setUserName(UseMySQL::instance()->getUserName(pairs.first));
	//设置签名
	childWidget->setSignName(UseMySQL::instance()->getUserSign(pairs.first));
	//添加子项到根项
	rootItem->addChild(childItem);
	ui.treeWidget->setItemWidget(childItem, 0, childWidget);
}

void TalkWindow::onSendBtnClicked()
{
	//不允许发送空白内容
	QString str = ui.textEdit->toPlainText();//获取输入框纯文本
	if (str.trimmed().isEmpty())
	{
		ui.textEdit->clear();
		QToolTip::showText(mapToGlobal(QPoint(600, 685)),
						   "发送信息为空",
						   this,
						   QRect(0,0,120,100),
						   2000);
		return;
	}

	//限制消息长度
	if (str.length() > MSG_LENGTH)
	{
		QToolTip::showText(mapToGlobal(QPoint(600, 685)),
						   QString("发送信息不能超过%1").arg(MSG_LENGTH),
						   this,
						   QRect(0, 0, 120, 100),
						   2000);
		return;
	}
	QJsonArray msgArray = parseDocumentToJson(ui.textEdit->document());

	//将json消息添加到html
	ui.msgWidget->appendMsg(msgArray, MYSELF);
	ui.textEdit->clear();//清空输入框
}



void TalkWindow::onTreeItemDoubleClicked(QTreeWidgetItem* item)
{
	bool isChild = item->data(0, Qt::UserRole).toBool();
	if (isChild)
	{
		QString talkID = item->data(0, Qt::UserRole + 1).toString();
		if (talkID == LoginUserAccount) return;//自己不能和自己聊天

		WindowManager::instance()->addNewTalkWindow(talkID.toInt());
	}
}

void TalkWindow::onFileOpenBtnClicked()
{
	SendFile* sendFile = new SendFile(this);
	sendFile->show();


}

void TalkWindow::setWindowName(const QString & name)
{
	ui.nameLabel->setStyleSheet("color: rgb(112,114,113); font: bold;");
	ui.nameLabel->setAlignment(Qt::AlignCenter);//设置文本居中
	ui.nameLabel->setText(name);
}

void TalkWindow::addEmotionImage(int name)
{
	ui.textEdit->setFocus();
	ui.textEdit->addEmotionHtml(name);
}

int TalkWindow::getTalkID()
{
	return talkID;
}

void TalkWindow::appendObjectMsg(const QJsonArray& jsonMsg, QString obj)
{
	ui.msgWidget->appendMsg(jsonMsg, obj);
}
