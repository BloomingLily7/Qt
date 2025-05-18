#include "EmotionLabel.h"
#include "define.h"
EmotionLabel::EmotionLabel(int name,QWidget *parent)
	: QClickLabel(parent), emotionName(name)
{
	setAlignment(Qt::AlignCenter);
	setObjectName("emotionLabelItem");
	setFixedSize(32, 32);

	QString imagePath = IMG_PATH(QString::number(name));
	this->setPixmap(QPixmap(imagePath));

	connect(this, &QClickLabel::clicked, [this]() 
		{
			emit emotionClicked(emotionName);
		});
}

EmotionLabel::~EmotionLabel()
{}
