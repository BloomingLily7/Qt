#include "CCMainWindow.h"
#include <qpainter.h>
#include <qdebug.h>
#include <QTimer>
#include "UseMySQL.h"
#include "CommonUtils.h"
#include "SkinWindow.h"
#include "NotifyManager.h"
#include <QTreeWidgetItem>
#include "RootContactItem.h"
#include "ContactItem.h"
#include "QProxyStyle"
#include "SysTrayIcon.h"
#include "WindowManager.h"

#include "CustomProxyStyle.h"

QString LoginUserAccount;//登录用户的账号

CCMainWindow::CCMainWindow(const QString& account, QWidget* parent)
	: account(account),BasicWindow(parent)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::Tool /*| Qt::WindowStaysOnTopHint*/);//设置为工具窗口，不显示任务体表
	init();

	LoginUserAccount = account;//保存登录用户的账号
}

CCMainWindow::~CCMainWindow()
{}

void CCMainWindow::init()
{
	loadStyleSheet("CCMainWindow");
	initButton();
	ui.searchLineEdit->installEventFilter(this);//安装事件过滤器
	setUserName();
	setUserHeadPixmap();
	initConnect();
	initContact();
	initSysTray();
}

void CCMainWindow::drawButtonIcon(QPushButton*& btn, const QString& icon)
{
	QPixmap pixmap(btn->size());//创建画布
	pixmap.fill(Qt::transparent);

	QPixmap iconPixmap(icon);//图片
	QPainter p(&pixmap);
	p.drawPixmap((pixmap.width() - iconPixmap.width()) / 2, (pixmap.height() - iconPixmap.height()) / 2,iconPixmap);
	btn->setIcon(pixmap);
	btn->setIconSize(btn->size());
}

void CCMainWindow::initButton()
{
	//状态按钮
	drawButtonIcon(ui.statusBtn,":/Resources/MainWindow/StatusSucceeded.png");
	//等级按钮
	initLevelButton();
	//appWidget中的app按钮
	initAppButton();
}

void CCMainWindow::initAppButton()
{
	QHBoxLayout* appLayout = new QHBoxLayout();
	appLayout->setContentsMargins(0, 0, 0, 0);//设置边距
	appLayout->setSpacing(2);//设置间距

	QStringList appList = { "app_7","app_2","app_3","app_4","app_5","app_6" ,"app_skin"};
	for (auto app : appList)
	{
		appLayout->addWidget(addOtherButton(APP_ICON(app), app));
	}
	appLayout->addStretch();//添加伸缩空间
	ui.appWidget->setLayout(appLayout);//设置布局


	//bottomLayout
	QStringList bottomList = { "app_10","app_8","app_11","app_9"};
	for (auto app : bottomList)
	{
		ui.bottomLayout_up->addWidget(addOtherButton(APP_ICON(app), app));
	}
	//添加伸缩空间
	ui.bottomLayout_up->addStretch();
}

QPushButton* CCMainWindow::addOtherButton(const QString& icon, const QString& objName, QSize size)
{
	QPushButton* btn = new QPushButton(this);
	btn->setFixedSize(size);
	btn->setObjectName(objName);
	btn->setProperty("hasborder", true);//添加动态属性

	drawButtonIcon(btn, icon);
	connect(btn,&QPushButton::clicked,this,&CCMainWindow::onButtonClicked);
	return btn;
}

void CCMainWindow::initLevelButton()
{
	drawLevelButton(0);
	QTimer* timer = new QTimer(this);
	timer->setInterval(1000);
	connect(timer, &QTimer::timeout, [this]()
		{
			static int level = 0;
			if (level == 99) level = 0;
			drawLevelButton(++level);
		});

	timer->start();
}

void CCMainWindow::drawLevelButton(int level)
{
	QPixmap pixmap(ui.levelBtn->size());//创建画布
	pixmap.fill(Qt::transparent);

	QPainter p(&pixmap);
	p.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

	int tenNum = level / 10;
	int unitNum = level % 10;

	QPixmap levelValue(":/Resources/MainWindow/levelvalue.png");
	//绘制十位数
	p.drawPixmap(10,4,levelValue,tenNum * 6,0,6,7);
	//绘制个位数
	p.drawPixmap(16, 4, levelValue, unitNum * 6, 0, 6, 7);

	ui.levelBtn->setIcon(pixmap);
	ui.levelBtn->setIconSize(ui.levelBtn->size());
}

void CCMainWindow::updateSearchStyle(bool hasFocus)
{
	if (hasFocus)//有焦点
	{
		ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{"
												"background-color: rgb(255, 255, 255); "
												"border-bottom: 1px solid rgba(%1, %2, %3,100); }"
												"QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png);}"
								                "QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png);}"
												"QPushButton#searchBtn:pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhighdown.png);}"
		).arg(skinColor.red()).arg(skinColor.green()).arg(skinColor.blue()));
	}
	else
	{
		ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{"
												"background-color: rgba(%1, %2, %3,50); "
												"border-bottom: 1px solid rgba(%1, %2, %3,100); }"
												"QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/search_icon.png);}"
		).arg(skinColor.red()).arg(skinColor.green()).arg(skinColor.blue()));
	}
}

void CCMainWindow::setUserName()
{
	QString name = UseMySQL::instance()->getUserName(account);

	//如果名字过长，使用省略号
	name = ui.nameLabel->fontMetrics().elidedText(name, Qt::ElideRight, ui.nameLabel->width());
	ui.nameLabel->setStyleSheet("color: rgb(255, 255, 255);"
								"font: bold 17px;"
								);
	ui.nameLabel->setText(name);
}

void CCMainWindow::setUserHeadPixmap()
{
	QString path = UseMySQL::instance()->getUserHeadPixmap(account);
	QPixmap head(path);
	QPixmap mask(":/Resources/MainWindow/head_mask.png");//底片
	ui.headLabel->setPixmap(CommonUtils::getRoundPixmap(head, mask, ui.headLabel->size()));

}

void CCMainWindow::initConnect()
{
	//样式优先级
	//内联样式：直接在部件上设置的样式
	//ID选择器：#id
	// 类选择器：如QWidget
	// 属性选择器：如QWidget[hasborder=true]
	connect(NotifyManager::instance(), &NotifyManager::skinChanged, [this]()
		{
			updateSearchStyle(false);
		}
	);

	connect(ui.sysmin,&QPushButton::clicked,this,&CCMainWindow::onMin);
	connect(ui.sysclose,&QPushButton::clicked,this,&CCMainWindow::onQuit);
}

void CCMainWindow::initContact()
{
	//改变样式,去掉焦点矩形
	ui.treeWidget->setStyle(new CustomProxyStyle());

	//根项
	QTreeWidgetItem* rootItem = new QTreeWidgetItem();
	rootItem->setData(0,Qt::UserRole,0);//根项UserRole数据为0

	RootContactItem* rootWidget = new RootContactItem(true, ui.treeWidget);
	rootWidget->setText(QString::fromUtf8("联系人"));
	ui.treeWidget->addTopLevelItem(rootItem);
	ui.treeWidget->setItemWidget(rootItem, 0, rootWidget);//设置根项的部件

	//添加子项
	addUserGroup(rootItem);

	//连接信号
	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &CCMainWindow::onTreeItemClicked);
	connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &CCMainWindow::onTreeItemDoubleClicked);
	connect(ui.treeWidget, &QTreeWidget::itemExpanded, this, &CCMainWindow::onTreeItemExpanded);
	connect(ui.treeWidget, &QTreeWidget::itemCollapsed, this, &CCMainWindow::onTreeItemCollapsed);
}

void CCMainWindow::addUserGroup(QTreeWidgetItem* rootItem)
{
	//获取群号
	QList<int> group = UseMySQL::instance()->getUserGroup(account);

	for (int id : group)
	{
		QTreeWidgetItem* childItem = new QTreeWidgetItem();
		childItem->setData(0, Qt::UserRole, 1);//子项UserRole数据为1
		childItem->setData(0, Qt::UserRole + 1, id);

		ContactItem* childWidget = new ContactItem(ui.treeWidget);

		//获取群头像
		QString headPath = UseMySQL::instance()->getGroupHeadPixmap(id);
		QPixmap head(headPath);
		QPixmap mask(":/Resources/MainWindow/head_mask.png");//底片
		childWidget->setHeadPixmap(CommonUtils::getRoundPixmap(head, mask, childWidget->getHeadLabelSize()));

		//获取群名称
		QString groupName = UseMySQL::instance()->getGroupName(id);
		childWidget->setUserName(groupName);

		rootItem->addChild(childItem);
		ui.treeWidget->setItemWidget(childItem, 0, childWidget);//设置子项的部件
	}
}

void CCMainWindow::initSysTray()
{
	SysTrayIcon* icon = new SysTrayIcon(this);
	icon->show();
}

bool CCMainWindow::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == ui.searchLineEdit)
	{
		if (event->type() == QEvent::FocusIn)//获取焦点
		{
			updateSearchStyle(true);
			return false;
		}
		else if (event->type() == QEvent::FocusOut)//失去焦点
		{
			updateSearchStyle(false);
			return false;
		}
	}
	return QWidget::eventFilter(watched, event);
}

void CCMainWindow::onTreeItemClicked(QTreeWidgetItem* item)
{
	bool isRoot = item->data(0, Qt::UserRole).toInt() == 0;
	if (isRoot)
	{
		item->setExpanded(!item->isExpanded());
	}
}

void CCMainWindow::onTreeItemDoubleClicked(QTreeWidgetItem* item)
{
	bool isChild = item->data(0, Qt::UserRole).toInt() == 1;
	if (isChild)
	{
		WindowManager::instance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toInt());
	}
}

void CCMainWindow::onTreeItemExpanded(QTreeWidgetItem* item)
{
	bool isRoot = item->data(0, Qt::UserRole).toInt() == 0;
	if (isRoot)
	{
		RootContactItem* rootWidget = dynamic_cast<RootContactItem*>(ui.treeWidget->itemWidget(item, 0));
		if (rootWidget)
		{
			rootWidget->setExpanded(true);
		}
	}
}

void CCMainWindow::onTreeItemCollapsed(QTreeWidgetItem* item)
{
	bool isRoot = item->data(0, Qt::UserRole).toInt() == 0;
	if (isRoot)
	{
		RootContactItem* rootWidget = dynamic_cast<RootContactItem*>(ui.treeWidget->itemWidget(item, 0));
		if (rootWidget)
		{
			rootWidget->setExpanded(false);
		}
	}
}

void CCMainWindow::onQuit()
{
	TalkWindowShell* shell = WindowManager::instance()->getWindowShell();
	if (shell)
	{
		shell->close();
	}
	QApplication::quit();
}

void CCMainWindow::onButtonClicked()
{
	if (sender()->objectName() == "app_skin")
	{
		SkinWindow* skinWindow = new SkinWindow();
		skinWindow->show();
	}
}
