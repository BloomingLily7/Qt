#include "BasicWindow.h"
#include <qstyleoption.h>
#include <QPainter>
#include <qscreen.h>
#include <QMouseEvent>
#include <ConfigIni.h>
#include <CommonUtils.h>
#include "NotifyManager.h"
BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent), moveAble(false), titleBar(nullptr)
{
	setWindowFlags(Qt::FramelessWindowHint);

	skinColor = ConfigIni::getColor();

	//改变皮肤颜色
	connect(NotifyManager::instance(), &NotifyManager::skinChanged,this,&BasicWindow::onSkinChanged);
}

BasicWindow::~BasicWindow()
{}

void BasicWindow::paintEvent(QPaintEvent * event)
{
	QDialog::paintEvent(event);	//保留QDialog默认绘制行为,但是Qdialog是空的

	QStyleOption opt;
	opt.initFrom(this);

	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);	//手动调用样式表相关绘制方法，确保样式表生效
}

void BasicWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (moveAble)
	{
		QPoint movePoint = event->globalPos() - startMovePos;	//鼠标移动的全局点坐标移动量
		QPoint windowPoint = this->pos();				//父窗体坐标
		QPoint windowMovePoint = windowPoint + movePoint;		//父窗体需要移动的坐标
		this->move(windowMovePoint);

		startMovePos = event->globalPos();

		event->accept();
	}
	//return QWidget::mouseMoveEvent(event);
}

void BasicWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (!titleBar)//没有标题栏
		{
			moveAble = true;
			startMovePos = event->globalPos();
		}
		else
		{
			if (!titleBar->windowMax())
			{
				moveAble = true;
				startMovePos = event->globalPos();
			}
		}
	}
	return QDialog::mousePressEvent(event);
}

void BasicWindow::mouseReleaseEvent(QMouseEvent* event)
{
	moveAble = false;	//移动结束
	event->accept();
	//return QWidget::mouseReleaseEvent(event);
}

void BasicWindow::onMin()
{
	if(Qt::Tool == (windowFlags() & Qt::Tool))	//工具窗口则隐藏
	{
		hide();
	}
	else
	{
		showMinimized();	//非工具窗口最小化
	}
}

void BasicWindow::onRestore()
{
	QPoint pos;
	QSize size;
	titleBar->getRestoreInfo(pos, size);
	setGeometry(QRect(pos, size));
}

void BasicWindow::onMax()
{
	titleBar->saveRestoreInfo(this->pos(), this->size());

	QRect rect = QApplication::screens()[0]->availableGeometry();

	setGeometry(rect);
}

void BasicWindow::onClose()
{
	//关闭窗口时，判断是否是登录窗口
	QObject* parent = sender()->parent();
	if (parent)
	{
		if (parent->objectName() == "UserLoginClass")
		{
			//onQuit();
			QApplication::quit();
			return;
		}
	}
	close();
}

void BasicWindow::onHide()
{
	hide();
}

void BasicWindow::onShow()
{
	show();
	activateWindow();//设置为活动窗口
}

//void BasicWindow::onQuit()
//{
//	QApplication::quit();
//}

void BasicWindow::onSkinChanged(const QColor& color)
{
	skinColor = color;
	loadStyleSheet(styleFileName);//重新加载样式表
}

void BasicWindow::initTitleBar(ButtonType buttonType, const QString & filePath, const QString & titleContent)
{
	titleBar = new TitleBar(buttonType,this);
	titleBar->setTitleIcon(filePath);
	titleBar->setTitleContent(titleContent);


	titleBar->move(0, 0);	

	connect(titleBar,&TitleBar::buttonMinClicked,this,&BasicWindow::onMin);
	connect(titleBar,&TitleBar::buttonMaxClicked,this,&BasicWindow::onMax);
	connect(titleBar,&TitleBar::buttonRestoreClicked,this,&BasicWindow::onRestore);
	connect(titleBar,&TitleBar::buttonCloseClicked,this,&BasicWindow::onClose);
	
}

void BasicWindow::loadStyleSheet(const QString& fileName)
{
	styleFileName = fileName;

	int red = skinColor.red();
	int green = skinColor.green();
	int blue = skinColor.blue();

	QString styleSheet = QString("QWidget[titleskin=true] \
										 {background-color:rgb(%1,%2,%3);} \
								  QWidget[bottomskin=true] \
										 {background-color:rgba(%1,%2,%3,50);}"
	).arg(red).arg(green).arg(blue);

	CommonUtils::loadStyleSheet(this, fileName, styleSheet);
}
