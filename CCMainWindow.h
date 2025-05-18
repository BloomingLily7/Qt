#pragma once

#include <QWidget>
#include "ui_CCMainWindow.h"
#include "BasicWindow.h"
#include <QString>
class CCMainWindow : public BasicWindow
{
	Q_OBJECT

public:
	CCMainWindow(const QString& account, QWidget *parent = nullptr);
	~CCMainWindow();

private:
	void init();
	void drawButtonIcon(QPushButton*& btn, const QString& icon);
	void initButton();
	void initAppButton();
	QPushButton* addOtherButton(const QString& icon, const QString& objName, QSize size = QSize(20, 20));

	void initLevelButton();
	void drawLevelButton(int level);

	void updateSearchStyle(bool hasFocus);

	void setUserName();
	void setUserHeadPixmap();

	void initConnect();

	void initContact();

	void addUserGroup(QTreeWidgetItem* rootItem);

	void initSysTray();

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
	void onButtonClicked();

	void onTreeItemClicked(QTreeWidgetItem* item);
	void onTreeItemDoubleClicked(QTreeWidgetItem* item);

	void onTreeItemExpanded(QTreeWidgetItem* item);
	void onTreeItemCollapsed(QTreeWidgetItem* item);

	void onQuit();

private:
	Ui::CCMainWindowClass ui;
	QString account;
};
