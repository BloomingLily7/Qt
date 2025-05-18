#pragma once

#include <QMenu>
#include <qmap.h>
class CustomMenu  : public QMenu
{
	Q_OBJECT

public:
	CustomMenu(QWidget *parent = nullptr);
	~CustomMenu();
public:
	void addMenuAciton(const QString& name,const QString& icon,const QString& text);

	QAction* getMenuAction(const QString& name);
private:
	QMap<QString, QAction*> map;//菜单名称和菜单项的映射关系
};
