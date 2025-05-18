#include "SysTrayIcon.h"
#include <qwidget.h>
#include "CustomMenu.h"
#include <QDebug>
SysTrayIcon::SysTrayIcon(QWidget *parent)
	: QSystemTrayIcon(parent), parent(parent)
{
	init();
}

SysTrayIcon::~SysTrayIcon()
{}

void SysTrayIcon::init()
{
	setToolTip("QtQQ");//设置托盘图标提示信息
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));//设置托盘图标

	connect(this, &QSystemTrayIcon::activated, this, &SysTrayIcon::onIconActivated);
}

void SysTrayIcon::popupMenu()
{
	//初始化菜单
	CustomMenu* menu = new CustomMenu(parent);
	menu->addMenuAciton("show", ":/Resources/MainWindow/app/logo.ico",QString::fromUtf8("显示"));
	menu->addMenuAciton("quit", ":/Resources/MainWindow/app/page_close_btn_hover.png", QString::fromUtf8("退出"));

	//连接菜单项的信号
	connect(menu->getMenuAction("show"),SIGNAL(triggered()),parent,SLOT(onShow()));
	connect(menu->getMenuAction("quit"),SIGNAL(triggered()),parent,SLOT(onQuit()));

	menu->exec(QCursor::pos());//以全局屏幕坐标返回主屏幕光标位置
	menu->deleteLater();//删除菜单
}

void SysTrayIcon::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger)
	{
		parent->show();
		parent->activateWindow();
	}
	else if (reason == QSystemTrayIcon::Context)
	{
		popupMenu();
	}
}
