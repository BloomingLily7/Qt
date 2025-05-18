#pragma once
#include <QProxyStyle>

class CustomProxyStyle : public QProxyStyle
{
public:
	//绘制各种基础的ui元素
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = Q_NULLPTR) const override
	{
		if (element == PE_FrameFocusRect)//绘制元素是焦点矩形
		{
			return;
		}
		else
		{
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};