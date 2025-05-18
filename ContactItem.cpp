#include "ContactItem.h"

ContactItem::ContactItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

ContactItem::~ContactItem()
{}

void ContactItem::setUserName(const QString & name)
{
	ui.userName->setText(name);
}

void ContactItem::setSignName(const QString& sign)
{
	ui.signName->setText(sign);
}

void ContactItem::setHeadPixmap(const QPixmap& pixmap)
{
	ui.headLabel->setPixmap(pixmap);
}

QString ContactItem::getUserName()
{
	return ui.userName->text();
}

QSize ContactItem::getHeadLabelSize()
{
	return ui.headLabel->size();
}
