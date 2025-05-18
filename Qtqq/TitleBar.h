#pragma once

#include <QWidget>
#include "ui_TitleBar.h"
#include "define.h"
class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(ButtonType type, QWidget *parent);
	~TitleBar();

	
	void setTitleIcon(const QString& filePath);
	void setTitleContent(const QString& titleContent);
	void setTitleWidth(int width);

	void saveRestoreInfo(const QPoint &point, const QSize &size);
	void getRestoreInfo(QPoint& point, QSize& size);

	bool windowMax();

protected:
	//鼠标点击、移动、释放
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void mouseDoubleClickEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;	//重写绘图事件

signals:

	void buttonMinClicked();	//最小化按钮点击
	void buttonRestoreClicked();	//还原按钮点击
	void buttonMaxClicked();	//最大化按钮点击
	void buttonCloseClicked();	//关闭按钮点击

private slots:
	void onButtonRestoreClicked();
	void onButtonMaxClicked();

private:
	void init();

	void setTitleButton();

	void initConnect();


private:
	Ui::TitleBarClass ui;
	ButtonType btnType;

	bool moveAble;
	QPoint startMovePos;	//窗体开始移动的坐标

	QPoint restorePos;	//还原位置
	QSize restoreSize;	//还原大小

};
