#include "QtQQServer.h"
#include "UseMySQL.h"
QtQQServer::QtQQServer(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
	init();
}

QtQQServer::~QtQQServer()
{}

void QtQQServer::init()
{
    initTcpServer();
    initUdpSocket();
    updateTableData(QString::number(2001));
}

void QtQQServer::initTcpServer()
{
    tcpServer = new TcpServer(tcpPort,this);
	tcpServer->run();

    connect(tcpServer,&TcpServer::signalTcpMsgData,this,&QtQQServer::onUdpBroadcast);
}

void QtQQServer::initUdpSocket()
{
	udpSocket = new QUdpSocket(this);
}

void QtQQServer::updateTableData(QString departmentID, QString employeeID)
{
	//获取公司群id
	QString companyID = UseMySQL::instance()->getDepartmentID(QString::fromLocal8Bit("公司群"));

    QStringList fileds;
	QVector<QVector<QString>> info;

    if (!departmentID.isEmpty() && departmentID != companyID)
    {
		UseMySQL::instance()->getDepartmentInfo(departmentID, fileds, info);
    }
}

void QtQQServer::onUdpBroadcast(QByteArray& arr)
{
    udpSocket->writeDatagram(arr, QHostAddress::Broadcast, udpPort);
}
