#pragma once

#include <QWidget>
#include "ui_EmotionWindow.h"
#include "EmotionLabel.h"
class EmotionWindow : public QWidget
{
	Q_OBJECT

public:
	EmotionWindow(QWidget *parent = nullptr);
	~EmotionWindow();
private:
	void initControl();
	EmotionLabel* initLabel(int name);
protected:
	void paintEvent(QPaintEvent* event)override;
signals:
	void addEmotion(int name);

private:
	Ui::EmotionWindow ui;
	const int rowCount = 12;
	const int columnCount = 14;
};
