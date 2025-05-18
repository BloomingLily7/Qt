#pragma once

#include <QSystemTrayIcon>

class SysTrayIcon  : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTrayIcon(QWidget *parent);
	~SysTrayIcon();
private:
	void init();
	void popupMenu();
private:
	QWidget* parent;
private slots:
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);
};
