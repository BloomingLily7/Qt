#include "QtQQServer.h"
#include "UseMySQL.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
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
    updateTableData();
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

	if (!departmentID.isEmpty() && departmentID != companyID) //部门id不为空且不等于公司群id
    {
		UseMySQL::instance()->getDepartmentInfo(departmentID, fileds, info);

    }
    else if (!employeeID.isEmpty()) //员工id不为空
    {
        info.resize(1);
		UseMySQL::instance()->getEmployeeInfo(employeeID, fileds, info[0]);
    }
    else
    {
        UseMySQL::instance()->getDepartmentInfo(companyID, fileds, info);
    }

    if (model)
    {
		model->deleteLater();
		model = nullptr;
    }
	int rows = info.size();
	int columns = info[0].size();
	model = new QStandardItemModel(rows, columns, this);
	model->setHorizontalHeaderLabels(getHeaderLabelsText());
	for (int i = 0;i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			QString str = info[i][j];
			if (fileds[j] == "departmentID")
			{
				model->setItem(i, j, new QStandardItem(UseMySQL::instance()->getDepartmentName(str)));
				continue;
			}
			else if (fileds[j] == "status")
			{
				model->setItem(i, j, new QStandardItem(str.toInt() ? QString::fromLocal8Bit("有效"): QString::fromLocal8Bit("注销")));
				continue;
			}
			model->setItem(i, j, new QStandardItem(str));
		}
	}
	ui.tableView->setModel(model);
	ui.tableView->verticalHeader()->setVisible(false);
}

QStringList QtQQServer::getHeaderLabelsText()
{
    QStringList list;
	list << QString::fromLocal8Bit("编号")
		<< QString::fromLocal8Bit("部门")
		<< QString::fromLocal8Bit("工号")
		<< QString::fromLocal8Bit("姓名")
		<< QString::fromLocal8Bit("签名")
		<< QString::fromLocal8Bit("头像")
		<< QString::fromLocal8Bit("状态");
    return list;
}

void QtQQServer::on_selectPictureBtn_clicked()
{
	picturePath = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("选择头像"),"/","*.png;;*.jpg");
	if (picturePath.isEmpty())
	{
		return;
	}
	QPixmap pixmap(picturePath);
	ui.headLabel->setPixmap(pixmap); 
}

void QtQQServer::on_addEmployeeBtn_clicked()
{
	QString emplyeeName = ui.employeeNameEdit->text();
	if (emplyeeName.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入员工姓名"));
		ui.employeeNameEdit->setFocus();
		return;
	}
	if (picturePath.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选择员工头像"));
		return;
	}
	picturePath.replace("/","\\\\");
	QString departmentName = ui.employeeDepBox->currentText();
	QString departmentID = UseMySQL::instance()->getDepartmentID(departmentName);
	if (departmentID.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("[departmentID error]添加失败"));
		return;
	}
	QString employeeID = UseMySQL::instance()->addEmployee(departmentID, emplyeeName, picturePath);
	if (employeeID.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("[employeeID error]添加失败"));
		return;
	}
	QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("添加%1成功").arg(employeeID));

	picturePath.clear();
	ui.employeeNameEdit->clear();
	ui.headLabel->setText(QString::fromLocal8Bit("员工存照"));
	ui.employeeDepBox->setCurrentIndex(0);
	updateTableData(QString(), employeeID);
}

void QtQQServer::on_queryDepBtn_clicked()
{
	ui.queryIDEdit->clear();
	QString departmentName = ui.queryDepBox->currentText();
	if (departmentName == QString::fromLocal8Bit("全公司"))
	{
		updateTableData();
		return;
	}
	QString departmentID = UseMySQL::instance()->getDepartmentID(departmentName);
	if (departmentID.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("[departmentID error]查询失败"));
		return;
	}
	updateTableData(departmentID);
}

void QtQQServer::on_queryEmployeeBtn_clicked()
{
	ui.queryDepBox->setCurrentIndex(0);
	QString employeeID = ui.queryIDEdit->text();
	if (employeeID.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入员工ID"));
		ui.queryIDEdit->setFocus();
		return;
	}
	updateTableData(QString(), employeeID);
}

void QtQQServer::on_deleteEmployeeBtn_clicked()
{
	ui.queryDepBox->setCurrentIndex(0);
	ui.queryIDEdit->clear();

	QString employeeID = ui.deleteIDEdit->text();
	if (employeeID.isEmpty())
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入员工ID"));
		ui.deleteIDEdit->setFocus();
		return;
	}

	if (!UseMySQL::instance()->deleteEmployee(employeeID))
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输正确入员工ID"));
		ui.deleteIDEdit->setFocus();
		return;
	}
	QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("注销%1成功").arg(employeeID));
	updateTableData(QString(), employeeID);
}

void QtQQServer::onUdpBroadcast(QByteArray& arr)
{
    udpSocket->writeDatagram(arr, QHostAddress::Broadcast, udpPort);
}
