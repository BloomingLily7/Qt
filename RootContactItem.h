#pragma once

#include <QLabel>
#include <qpropertyanimation.h>
class RootContactItem  : public QLabel
{
	Q_OBJECT

	Q_PROPERTY(int rotation READ getRotation WRITE setRotation)

public:
	RootContactItem(bool hasArrow = true, QWidget *parent = nullptr);
	~RootContactItem();
	void init();
	void setText(const QString& text);
	void setExpanded(bool expanded);
	
protected:
	void paintEvent(QPaintEvent* event) override;

private:
	int getRotation();
	void setRotation(int rotation);

private:
	int rotation;
	bool hasArrow;
	QString titleText;
	QPropertyAnimation* an;
};
