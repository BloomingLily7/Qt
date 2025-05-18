#pragma once

#include "QClickLabel.h"

class EmotionLabel  : public QClickLabel
{
	Q_OBJECT

public:
	EmotionLabel(int name,QWidget *parent);
	~EmotionLabel();
signals:
	void emotionClicked(int emotionName);
private:
	int emotionName;
};
