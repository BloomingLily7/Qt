#pragma once

#include <QTextEdit>

class MsgTextEdit  : public QTextEdit
{
	Q_OBJECT

public:
	MsgTextEdit(QWidget *parent);
	~MsgTextEdit();

	void addEmotionHtml(int emotionName);
};
