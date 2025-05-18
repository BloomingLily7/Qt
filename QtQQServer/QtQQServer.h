#pragma once

#include <QtWidgets/QDialog>
#include "ui_QtQQServer.h"
#include "TcpServer.h"
#include <QUdpSocket>
#include <qstandarditemmodel.h>
class QtQQServer : public QDialog
{
    Q_OBJECT

public:
    QtQQServer(QWidget *parent = nullptr);
    ~QtQQServer();

private:
    void init();
	void initTcpServer();
    void initUdpSocket();
    void updateTableData(QString departmentID = QString(), QString employeeID = QString());

private slots:
    void onUdpBroadcast(QByteArray& arr);

private:
    Ui::QtQQServerClass ui;

	TcpServer* tcpServer;//tcp服务端
    const int tcpPort = 57777;//tcp端口

	QUdpSocket* udpSocket;//udp服务端
    const int udpPort = 59999;//udp端口

    QStandardItemModel* model;

};
