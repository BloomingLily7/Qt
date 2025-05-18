#include "TcpServer.h"
#include <qdebug.h>
#include <qtcpsocket.h>
TcpServer::TcpServer(int tcpPrt, QObject *parent)
	: QTcpServer(parent), tcpPort(tcpPrt)
{}

TcpServer::~TcpServer()
{}

bool TcpServer::run()
{
	if (this->listen(QHostAddress::AnyIPv4, tcpPort))
	{
		qDebug() << "服务端监听端口:" << tcpPort << endl;
		return true;
	}
	else
	{
		qDebug() << "服务端监听失败:" << tcpPort << endl;
		return false;
	}
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	qDebug() << QString("新的连接：%1").arg(socketDescriptor);

	QTcpSocket* socket = new QTcpSocket;
	socket->setSocketDescriptor(socketDescriptor);

	connect(socket,&QTcpSocket::readyRead,this,&TcpServer::readSocket);
	connect(socket,&QTcpSocket::disconnected,socket,&QTcpSocket::deleteLater);
}

void TcpServer::readSocket()
{
	QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());//获取发送信号的对象

	if (socket)
	{
		QByteArray buf =  socket->readAll();
		if (!buf.isEmpty())
		{
			qDebug() << QString("[send IP：%1] Data: %2").arg(socket->peerAddress().toString()).arg(QString(buf))<<endl;
			emit signalTcpMsgData(buf);
		}
	}
}
