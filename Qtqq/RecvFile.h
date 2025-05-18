#pragma once

#include "BasicWindow.h"
#include "ui_RecvFile.h"
#include "qjsonobject.h"
class RecvFile : public BasicWindow
{
	Q_OBJECT

public:
	RecvFile(const QJsonObject& data, QWidget *parent = nullptr);
	~RecvFile();
	void setLabelMsg(const QString& msg);
private:
	void init();
private slots:
	void on_cancelBtn_clicked();
	void on_okBtn_clicked();

private:
	Ui::ReceviveFile ui;
	QJsonObject fileObj;
};
