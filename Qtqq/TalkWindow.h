#pragma once

#include <QWidget>
#include "ui_TalkWindow.h"

class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(const int& id, QWidget *parent = nullptr);
	~TalkWindow();
private:
	void init();
	//void initTalkType();
	void initStyle();
	void initConnect();
	void initTalkWindow();
	void initGroupTalk();
	void initSingleTalk();

	QJsonArray parseDocumentToJson(QTextDocument* document);

	void addMember(QTreeWidgetItem* rootItem,const QPair<QString,QString>& pairs);

private slots:
	void onSendBtnClicked();
	void onTreeItemDoubleClicked(QTreeWidgetItem* item);
	void onFileOpenBtnClicked();

public:
	void setWindowName(const QString& name);
	void addEmotionImage(int name);
	int getTalkID();
	void appendObjectMsg(const QJsonArray& jsonMsg, QString obj);

private:
	Ui::TalkWindow ui;
	const int talkID;
	//bool isGroupTalk;
};
