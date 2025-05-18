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
	//��������ƶ����ͷ�
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void mouseDoubleClickEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event) override;	//��д��ͼ�¼�

signals:

	void buttonMinClicked();	//��С����ť���
	void buttonRestoreClicked();	//��ԭ��ť���
	void buttonMaxClicked();	//��󻯰�ť���
	void buttonCloseClicked();	//�رհ�ť���

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
	QPoint startMovePos;	//���忪ʼ�ƶ�������

	QPoint restorePos;	//��ԭλ��
	QSize restoreSize;	//��ԭ��С

};
