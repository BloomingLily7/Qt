#include "TitleBar.h"
#include <qpixmap.h>
#include "CommonUtils.h"
#include <QMouseEvent>

TitleBar::TitleBar(ButtonType type, QWidget *parent)
	:btnType(type), QWidget(parent)
{
	ui.setupUi(this);
	init();
}

TitleBar::~TitleBar()
{}


//设置标题栏图标
void TitleBar::setTitleIcon(const QString & filePath)
{
	QPixmap pix(filePath);

	ui.iconLabel->setFixedSize(pix.size());
	ui.iconLabel->setPixmap(pix);
}

//设置标题栏内容
void TitleBar::setTitleContent(const QString& titleContent)
{
	ui.titleContent->setText(titleContent);
}

//设置标题栏宽度
void TitleBar::setTitleWidth(int width)
{
	this->setFixedWidth(width);
}


//保存窗体最大化前的位置和尺寸
void TitleBar::saveRestoreInfo(const QPoint& point, const QSize& size)
{
	restorePos = point;
	restoreSize = size;
}

//获取窗体最大化前的位置和尺寸
void TitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point = restorePos;
	size = restoreSize;
}

//返回窗体是否为最大化状态
bool TitleBar::windowMax()
{
	if (btnType == MIN_MAX_CLOSE_BUTTON)
	{
		if (!ui.buttonMax->isVisible()) {
			return true;
		}
	}
	return false;
}

void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
	if (moveAble)
	{
		QPoint movePoint = event->globalPos() - startMovePos;	//鼠标移动的全局点坐标移动量
		QPoint windowPoint = parentWidget()->pos();				//父窗体坐标
		QPoint windowMovePoint = windowPoint + movePoint;		//父窗体需要移动的坐标
		parentWidget()->move(windowMovePoint);

		startMovePos = event->globalPos();
	}
	return QWidget::mouseMoveEvent(event);
}

void TitleBar::mousePressEvent(QMouseEvent* event)
{
	//if (btnType == MIN_MAX_CLOSE_BUTTON)
	//{
	//	if (ui.buttonMax->isVisible())
	//	{
	//		moveAble = true;
	//		startMovePos = event->globalPosition().toPoint();//返回事件发生时的全局坐标
	//	}
	//}
	//else
	//{
	//	moveAble = true;
	//	startMovePos = event->globalPosition().toPoint();;//返回事件发生时的全局坐标
	//}

	if (!windowMax())
	{
		moveAble = true;
		startMovePos = event->globalPos();

		event->accept();
	}

	//return QWidget::mousePressEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
	moveAble = false;	//移动结束
	return QWidget::mouseReleaseEvent(event);
}

//鼠标双击事件，实现窗体最大化、还原
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (btnType == MIN_MAX_CLOSE_BUTTON)
	{
		if (ui.buttonMax->isVisible())
		{
			onButtonMaxClicked();
		}
		else
		{
			onButtonRestoreClicked();
		}
	}
	return QWidget::mouseDoubleClickEvent(event);
}

void TitleBar::paintEvent(QPaintEvent* event)
{
	//QWidget::paintEvent(event); QWidget绘图事件是空的，一般要先调用父类绘图事件，再调用重写的绘图事件
	if (width() != parentWidget()->width())
	{
		setFixedWidth(parentWidget()->width());
	}
}

//响应还原按钮点击信号
void TitleBar::onButtonRestoreClicked()
{
	ui.buttonRestore->setVisible(false);
	ui.buttonMax->setVisible(true);

	emit buttonRestoreClicked();
}

//响应关闭按钮点击信号
void TitleBar::onButtonMaxClicked()
{
	ui.buttonRestore->setVisible(true);
	ui.buttonMax->setVisible(false);

	emit buttonMaxClicked();
}

void TitleBar::init()
{
	CommonUtils::loadStyleSheet(this,"Title");
	setTitleButton();

	//初始化数据成员
	moveAble = ui.buttonMax->isVisible();

	initConnect();
}

void TitleBar::setTitleButton()
{
	switch (btnType)
	{
	case MIN_CLOSE_BUTTON:
		ui.buttonRestore->setVisible(false);
		ui.buttonMax->setVisible(false);
		break;
	case MIN_MAX_CLOSE_BUTTON:
		ui.buttonRestore->setVisible(false);
		break;
	case CLOSE_BUTTON:
		ui.buttonMax->setVisible(false);
		ui.buttonRestore->setVisible(false);
		ui.buttonMin->setVisible(false);
		break;
	default:
		break;
	}
}

void TitleBar::initConnect()
{
	connect(ui.buttonMin, &QPushButton::clicked, this, &TitleBar::buttonMinClicked);
	
	connect(ui.buttonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(ui.buttonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));

	connect(ui.buttonClose, &QPushButton::clicked, this, &TitleBar::buttonCloseClicked);
}
