#include "CommonUtils.h"
#include "define.h"
#include <qfile.h>
#include <qpainter.h>
CommonUtils::CommonUtils()
{}

CommonUtils::~CommonUtils()
{}

/*
	agr1����Ҫ������ʽ��Ĳ���
	arg2����ʽ���ļ���
	arg3����ѡ�Ķ�����ʽ
*/
void CommonUtils::loadStyleSheet(QWidget * widget, const QString & fileName, const QString & otherStyle)
{
	if (!widget) return;
	widget->setStyleSheet(QString());//���������ʽ

	const QString filePath = QRC_QSS(fileName);

	QFile file(filePath);

	if (file.open(QFile::ReadOnly)) {
		QString qss = file.readAll(); //��ȡ�ļ�����ʽ
		qss += otherStyle;			 //����������ʽ

		widget->setStyleSheet(qss);	//���ò�����ʽ
		file.close();
	}
}
//��ȡԲ��ͼƬ
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
	//����ԭͼ
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

