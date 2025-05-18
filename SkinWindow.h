#pragma once

#include "BasicWindow.h"
#include "ui_SkinWindow.h"
#include "QClickLabel.h"
class SkinWindow : public BasicWindow
{
	Q_OBJECT

public:
	SkinWindow(QWidget *parent = nullptr);
	~SkinWindow();
private:
	void initControl();
	QClickLabel* initLabel(const QColor& color);

private:
	Ui::SkinWindowClass ui;
};
