#pragma once

#include "BasicWindow.h"
#include "ui_SendFile.h"
#include <QJsonValue>
class SendFile : public BasicWindow
{
	Q_OBJECT

public:
	SendFile(QWidget* parent = nullptr);
	~SendFile();
private:
	void init();
private slots:;
	void on_openBtn_clicked();
	void on_sendBtn_clicked();

signals:
	void signalSendFile(QJsonValue data);

private:
	Ui::SendFileClass ui;
	QString filePath;
};
