#pragma once

#include <QTcpServer>

class TcpServer  : public QTcpServer
{
	Q_OBJECT

public:
	TcpServer(int tcpPrt, QObject *parent = nullptr);
	~TcpServer();

	bool run();
protected:
	void incomingConnection(qintptr socketDescriptor) override;
	
private slots:
	void readSocket();
signals:
	void signalTcpMsgData(QByteArray& arr);

private:
	int tcpPort;

};
