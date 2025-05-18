#include "EmotionWindow.h"
#include "CommonUtils.h"
#include <QPainter>
EmotionWindow::EmotionWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//Qt::SubWindow一般用作子窗口，Qt::Tool一般用于独立窗口
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);//设置窗口无边框子窗口
	setAttribute(Qt::WA_TranslucentBackground, true);//设置窗口透明

	initControl();
}

EmotionWindow::~EmotionWindow()
{}

void EmotionWindow::initControl()
{
	CommonUtils::loadStyleSheet(this,"EmotionWindow");

	//168个表情
	for (int row = 0;row < rowCount;row++)
	{
		for (int column = 0;column < columnCount;column++)
		{
			ui.gridLayout->addWidget(initLabel(row * columnCount + column),row,column);
		}
	}
}

EmotionLabel* EmotionWindow::initLabel(int name)
{
	EmotionLabel* label = new EmotionLabel(name, this);
	connect(label,&EmotionLabel::emotionClicked,this,&EmotionWindow::addEmotion);
	return label;
}

void EmotionWindow::paintEvent(QPaintEvent* event)
{
	QWidget::paintEvent(event);//保留默认绘制行为
	QStyleOption opt;
	opt.initFrom(this);//获取当前控件的样式

	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}
