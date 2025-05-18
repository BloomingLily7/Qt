#include "CommonUtils.h"
#include "define.h"
#include <qfile.h>
#include <qpainter.h>
CommonUtils::CommonUtils()
{}

CommonUtils::~CommonUtils()
{}

/*
	agr1：需要设置样式表的部件
	arg2：样式表文件名
	arg3：可选的定制样式
*/
void CommonUtils::loadStyleSheet(QWidget * widget, const QString & fileName, const QString & otherStyle)
{
	if (!widget) return;
	widget->setStyleSheet(QString());//清除部件样式

	const QString filePath = QRC_QSS(fileName);

	QFile file(filePath);

	if (file.open(QFile::ReadOnly)) {
		QString qss = file.readAll(); //读取文件内样式
		qss += otherStyle;			 //若有其他样式

		widget->setStyleSheet(qss);	//设置部件样式
		file.close();
	}
}
//获取圆形图片
QPixmap CommonUtils::getRoundPixmap(QPixmap src, QPixmap mask, QSize maskSize)
{
	if (maskSize.isEmpty())
	{
		maskSize = mask.size();
	}
	else
	{
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	//缩放原图
	src = src.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	QImage image(maskSize, QImage::Format_ARGB32_Premultiplied);
	QPainter p(&image);

	p.setCompositionMode(QPainter::CompositionMode_Source);
	p.fillRect(image.rect(), Qt::transparent);
	p.setCompositionMode(QPainter::CompositionMode_SourceOver);
	p.drawPixmap(0, 0, mask);
	p.setCompositionMode(QPainter::CompositionMode_SourceIn);
	p.drawPixmap(0, 0, src);
	return QPixmap::fromImage(image);
}

