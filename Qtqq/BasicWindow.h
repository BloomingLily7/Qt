#pragma once

#include <QDialog>


#include "TitleBar.h"

class BasicWindow : public QDialog
{
	Q_OBJECT

protected:
	BasicWindow(QWidget *parent);
	virtual ~BasicWindow();

protected:
	void paintEvent(QPaintEvent* event) override;

	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;


public slots:;
	void onMin();
	void onRestore();
	void onMax();
	void onClose();

	void onHide();
	void onShow();
	//void onQuit();

	void onSkinChanged(const QColor& color);


protected:
	void initTitleBar(ButtonType buttonType = MIN_CLOSE_BUTTON, const QString& filePath = QString(), const QString& titleContent = QString());
	void loadStyleSheet(const QString& fileName);
protected:
	TitleBar* titleBar;


	bool moveAble;
	QPoint startMovePos;

	QColor skinColor;//皮肤颜色

	QString styleFileName;//样式表文件名
};
