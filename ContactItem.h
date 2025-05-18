#pragma once

#include <QWidget>
#include "ui_ContactItem.h"

class ContactItem : public QWidget
{
	Q_OBJECT

public:
	ContactItem(QWidget *parent = nullptr);
	~ContactItem();

	void setUserName(const QString& name);
	void setSignName(const QString& sign);
	void setHeadPixmap(const QPixmap& pixmap);

	QString getUserName();
	QSize getHeadLabelSize();


private:
	Ui::ContactItemClass ui;
};
