#pragma once

#include <QObject>
#include "TalkWindowShell.h"


class WindowManager  : public QObject
{
	Q_OBJECT

public:
	WindowManager(QObject *parent = nullptr);
	~WindowManager();

public:
	static WindowManager* instance();

	QWidget* findWindowMap(const int id);
	void addWindowMap(const int id, QWidget* window);
	void removeWindowMap(const int id);

	TalkWindowShell* getWindowShell() const;
	int getCreateID() const;

	void addNewTalkWindow(const int id);

private:
	TalkWindowShell*  talkWindowShell;
	QMap<int, QWidget*> map;				//key: id value: 聊天窗口
	int createID;											//创建的窗口ID
};
