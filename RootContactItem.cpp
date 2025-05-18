#include "RootContactItem.h"
#include <qpainter.h>
RootContactItem::RootContactItem(bool hasArrow,QWidget *parent)
	: QLabel(parent), hasArrow(hasArrow), rotation(0), an(nullptr)
{
	init();
}

RootContactItem::~RootContactItem()
{}

void RootContactItem::init()
{
	setFixedHeight(32);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	an = new QPropertyAnimation(this, "rotation",this);//初始化属性动画，绑定rotation属性
	an->setDuration(30);//动画持续时间
	an->setEasingCurve(QEasingCurve::InQuad);//动画曲线
}

void RootContactItem::setText(const QString& text)
{
	titleText = text;
	update();//文本改变,更新界面
}

void RootContactItem::setExpanded(bool expanded)
{
	if (expanded)
	{
		an->setEndValue(90);//展开时旋转90度
	}
	else
	{
		an->setEndValue(0);//收起时旋转0度
	}
	an->start();//开始动画
}

void RootContactItem::paintEvent(QPaintEvent* event)
{
	QLabel::paintEvent(event);//调用父类的绘制事件
	//绘制文本
	QPainter p1(this);
	p1.setRenderHint(QPainter::TextAntialiasing, true);//抗锯齿
	QFont font;
	font.setPointSize(10);
	p1.setFont(font);
	p1.drawText(24, 0, width() - 24, height(),Qt::AlignLeft|Qt::AlignVCenter,titleText);

	//绘制箭头
	if (hasArrow)
	{
		QPixmap arrow(":/Resources/MainWindow/arrow.png");

		QPixmap pixmap(arrow.size());//创建画布
		pixmap.fill(Qt::transparent);//填充透明色

		QPainter p2(&pixmap);
		p2.setRenderHint(QPainter::SmoothPixmapTransform, true);//抗锯齿

		p2.translate(pixmap.width() / 2, pixmap.height() / 2);//平移到中心

		p2.rotate(rotation);//旋转
		
		p2.drawPixmap(0 - pixmap.width() / 2,0 - pixmap.height() / 2,arrow);

		p1.drawPixmap(6, (this->height() - pixmap.height()) / 2, pixmap);//绘制箭头
	}
	
}

int RootContactItem::getRotation()
{
	return rotation;
}

void RootContactItem::setRotation(int rotation)
{
	this->rotation = rotation;
	update();//旋转角度更新,更新界面
}
