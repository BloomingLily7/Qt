#include "TalkWindowItem.h"
#include "CommonUtils.h"
TalkWindowItem::TalkWindowItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
}

TalkWindowItem::~TalkWindowItem()
{}

void TalkWindowItem::init()
{
	ui.tclosebtn->setVisible(false);

	connect(ui.tclosebtn, &QPushButton::clicked, this,&TalkWindowItem::signalCloseBtnClicked);//信号连接信号
}

void TalkWindowItem::setHeadPixmap(const QString& headPath)
{
	QPixmap head(headPath);
	QPixmap mask(":/Resources/MainWindow/head_mask.png");//底片
	ui.headLabel->setPixmap(CommonUtils::getRoundPixmap(head, mask, ui.headLabel->size()));//设置头像
}

void TalkWindowItem::setMsgText(const QString& msg)
{
	ui.msgLabel->setText(msg);//设置消息文本
}

void TalkWindowItem::enterEvent(QEvent* event)
{
	ui.tclosebtn->setVisible(true);
	return QWidget::enterEvent(event);
}

void TalkWindowItem::leaveEvent(QEvent* event)
{
	ui.tclosebtn->setVisible(false);
	return QWidget::leaveEvent(event);
}
