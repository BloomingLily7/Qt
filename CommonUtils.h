#pragma once
#include <qwidget.h>
class CommonUtils 
{
protected:
	CommonUtils();
	~CommonUtils();

public:
	static void loadStyleSheet(QWidget* widget, const QString& fileName, const QString& otherStyle = QString());
	static QPixmap getRoundPixmap(QPixmap src, QPixmap mask, QSize maskSize = QSize(0, 0));

	
};
