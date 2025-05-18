#include "CustomMenu.h"
#include "CommonUtils.h"
CustomMenu::CustomMenu(QWidget *parent)
	: QMenu(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	CommonUtils::loadStyleSheet(this, "Menu");
}

CustomMenu::~CustomMenu()
{}

void CustomMenu::addMenuAciton(const QString & name, const QString & icon, const QString & text)
{
	QAction* action = this->addAction(QIcon(icon), text);
	map.insert(name, action);//将菜单名称和菜单项的映射关系存储到map中
}

QAction* CustomMenu::getMenuAction(const QString& name)
{
	return map[name];//根据名称获取菜单项
}
