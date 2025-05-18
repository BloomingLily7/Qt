#include "TitleBar.h"
#include <qpixmap.h>
#include "CommonUtils.h"
#include <QMouseEvent>

TitleBar::TitleBar(ButtonType type, QWidget *parent)
	:btnType(type), QWidget(parent)
{
	ui.setupUi(this);
	init();
}

TitleBar::~TitleBar()
{}


//���ñ�����ͼ��
void TitleBar::setTitleIcon(const QString & filePath)
{
	QPixmap pix(filePath);

	ui.iconLabel->setFixedSize(pix.size());
	ui.iconLabel->setPixmap(pix);
}

//���ñ���������
void TitleBar::setTitleContent(const QString& titleContent)
{
	ui.titleContent->setText(titleContent);
}

//���ñ��������
void TitleBar::setTitleWidth(int width)
{
	this->setFixedWidth(width);
}


//���洰�����ǰ��λ�úͳߴ�
void TitleBar::saveRestoreInfo(const QPoint& point, const QSize& size)
{
	restorePos = point;
	restoreSize = size;
}

//��ȡ�������ǰ��λ�úͳߴ�
void TitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point = restorePos;
	size = restoreSize;
}

//���ش����Ƿ�Ϊ���״̬
bool TitleBar::windowMax()
{
	if (btnType == MIN_MAX_CLOSE_BUTTON)
	{
		if (!ui.buttonMax->isVisible()) {
			return true;
		}
	}
	return false;
}

void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
	if (moveAble)
	{
		QPoint movePoint = event->globalPos() - startMovePos;	//����ƶ���ȫ�ֵ������ƶ���
		QPoint windowPoint = parentWidget()->pos();				//����������
		QPoint windowMovePoint = windowPoint + movePoint;		//��������Ҫ�ƶ�������
		parentWidget()->move(windowMovePoint);

		startMovePos = event->globalPos();
	}
	return QWidget::mouseMoveEvent(event);
}

void TitleBar::mousePressEvent(QMouseEvent* event)
{
	//if (btnType == MIN_MAX_CLOSE_BUTTON)
	//{
	//	if (ui.buttonMax->isVisible())
	//	{
	//		moveAble = true;
	//		startMovePos = event->globalPosition().toPoint();//�����¼�����ʱ��ȫ������
	//	}
	//}
	//else
	//{
	//	moveAble = true;
	//	startMovePos = event->globalPosition().toPoint();;//�����¼�����ʱ��ȫ������
	//}

	if (!windowMax())
	{
		moveAble = true;
		startMovePos = event->globalPos();

		event->accept();
	}

	//return QWidget::mousePressEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
	moveAble = false;	//�ƶ�����
	return QWidget::mouseReleaseEvent(event);
}

//���˫���¼���ʵ�ִ�����󻯡���ԭ
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (btnType == MIN_MAX_CLOSE_BUTTON)
	{
		if (ui.buttonMax->isVisible())
		{
			onButtonMaxClicked();
		}
		else
		{
			onButtonRestoreClicked();
		}
	}
	return QWidget::mouseDoubleClickEvent(event);
}

void TitleBar::paintEvent(QPaintEvent* event)
{
	//QWidget::paintEvent(event); QWidget��ͼ�¼��ǿյģ�һ��Ҫ�ȵ��ø����ͼ�¼����ٵ�����д�Ļ�ͼ�¼�
	if (width() != parentWidget()->width())
	{
		setFixedWidth(parentWidget()->width());
	}
}

//��Ӧ��ԭ��ť����ź�
void TitleBar::onButtonRestoreClicked()
{
	ui.buttonRestore->setVisible(false);
	ui.buttonMax->setVisible(true);

	emit buttonRestoreClicked();
}

//��Ӧ�رհ�ť����ź�
void TitleBar::onButtonMaxClicked()
{
	ui.buttonRestore->setVisible(true);
	ui.buttonMax->setVisible(false);

	emit buttonMaxClicked();
}

void TitleBar::init()
{
	CommonUtils::loadStyleSheet(this,"Title");
	setTitleButton();

	//��ʼ�����ݳ�Ա
	moveAble = ui.buttonMax->isVisible();

	initConnect();
}

void TitleBar::setTitleButton()
{
	switch (btnType)
	{
	case MIN_CLOSE_BUTTON:
		ui.buttonRestore->setVisible(false);
		ui.buttonMax->setVisible(false);
		break;
	case MIN_MAX_CLOSE_BUTTON:
		ui.buttonRestore->setVisible(false);
		break;
	case CLOSE_BUTTON:
		ui.buttonMax->setVisible(false);
		ui.buttonRestore->setVisible(false);
		ui.buttonMin->setVisible(false);
		break;
	default:
		break;
	}
}

void TitleBar::initConnect()
{
	connect(ui.buttonMin, &QPushButton::clicked, this, &TitleBar::buttonMinClicked);
	
	connect(ui.buttonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(ui.buttonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));

	connect(ui.buttonClose, &QPushButton::clicked, this, &TitleBar::buttonCloseClicked);
}
