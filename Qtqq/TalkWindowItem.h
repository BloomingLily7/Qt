#pragma once

#include <QWidget>
#include "ui_TalkWindowItem.h"

class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = nullptr);
	~TalkWindowItem();

private:
	void init();

public:
	void setHeadPixmap(const QString& headPath);
	void setMsgText(const QString& msg);

signals:
	void signalCloseBtnClicked();

protected:
	void enterEvent(QEvent* event) override;//鼠标进入事件
	void leaveEvent(QEvent* event) override;//鼠标离开事件

private:
	Ui::TalkWindowItemClass ui;
};
