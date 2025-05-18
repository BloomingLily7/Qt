#include "WindowManager.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include "UseMySQL.h"
Q_GLOBAL_STATIC(WindowManager, theInstance)

WindowManager::WindowManager(QObject *parent)
	: QObject(parent), talkWindowShell(nullptr), createID(0)
{}

WindowManager::~WindowManager()
{}

WindowManager* WindowManager::instance()
{
	return theInstance();
}

QWidget* WindowManager::findWindowMap(const int id)
{
	
	return map.value(id,nullptr);
}

void WindowManager::addWindowMap(const int id, QWidget* window)
{
	if (!map.contains(id))
	{
		map.insert(id,window);
	}
}

void WindowManager::removeWindowMap(const int id)
{
	map.remove(id);
}

TalkWindowShell* WindowManager::getWindowShell() const
{
	return talkWindowShell;
}

int WindowManager::getCreateID() const
{
	return createID;
}

void WindowManager::addNewTalkWindow(const int id)
{
	if (!talkWindowShell)
	{
		talkWindowShell = new TalkWindowShell();
		connect(talkWindowShell, &TalkWindowShell::destroyed, [this]()
			{
				talkWindowShell = nullptr;
			}
		);
	}
	//判断窗口是否已经存在
	QWidget* widget = findWindowMap(id);
	if (!widget)
	{
		createID = id;
		TalkWindow* talkWindow = new TalkWindow(id,talkWindowShell);
		TalkWindowItem* talkWindowItem = new TalkWindowItem(talkWindow);
		createID = 0;

		//获取聊天窗口相关信息
		QString itemMsg;
		QString windowName;
		QString headPath;
		if (QString::number(id).length() <= GROUP_ID_LENGTH) //是群聊
		{
			itemMsg = UseMySQL::instance()->getGroupName(id);
			windowName = itemMsg;
			headPath = UseMySQL::instance()->getGroupHeadPixmap(id);
		}
		else//是好友
		{
			QString account = QString::number(id);
			itemMsg = UseMySQL::instance()->getUserName(account);
			windowName = UseMySQL::instance()->getUserDepartment(account) + itemMsg;
			headPath = UseMySQL::instance()->getUserHeadPixmap(account);
		}

		//设置部件
		talkWindow->setWindowName(windowName);
		talkWindowItem->setMsgText(itemMsg);
		talkWindowItem->setHeadPixmap(headPath);

		//聊天窗口添加到窗口框架
		talkWindowShell->addTalkWindow(talkWindow, talkWindowItem);
	}
	else//窗口已经存在
	{
		talkWindowShell->updateCurrentWindow(widget);
	}
	talkWindowShell->onShow();
}
