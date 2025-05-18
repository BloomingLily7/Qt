#pragma once
#include "ui_TalkWindowShell.h"
#include "BasicWindow.h"
#include <QMap>
#include "EmotionWindow.h"
#include <qjsonvalue.h>
#include <qtcpsocket.h>
#include <qudpsocket.h>

#include "TalkWindow.h"
#include "TalkWindowItem.h"

////类声明，防止头文件相互包含
//class TalkWindow;
//class TalkWindowItem;

class TalkWindowShell : public BasicWindow

{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = nullptr);
	~TalkWindowShell();

private:
	void init();
	void initStyle();
	void initEmotionWindow();
	void initJsFile();
	bool replaceJsTxt(QString& fileContent);
	void initTcpSocket();
	void initUdpSocket();

public:
	void addTalkWindow(TalkWindow* window, TalkWindowItem* windowItem);
	void updateCurrentWindow(QWidget* widget);

private slots:
	void onListItemClicked(QListWidgetItem* item);
	void onEmotionBtnClicked();
	void onaddEmotion(int name);

	void sendMsgToServer(QJsonValue data);

	void readMsgFromServer();

private:
	Ui::TalkWindowShellClass ui;

	QMap<QListWidgetItem*, QWidget*> map;//列表项和窗口的映射
	EmotionWindow* emotionWindow;

	QTcpSocket* tcp;//tcp套接字
	const int tcpPort = 57777;//tcp端口
	

	QUdpSocket* udp;//udp套接字
	const int udpPort = 59999;//udp端口

};
