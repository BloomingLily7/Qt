#include "UseMySQL.h"
#include <qmessagebox.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qdebug.h>
#include <qsqlquerymodel.h>
#include <qsqlrecord.h>
Q_GLOBAL_STATIC(UseMySQL, theInstance)

UseMySQL::UseMySQL(QObject *parent)
	: QObject(parent)
{
	connectMySQL();
}

UseMySQL::~UseMySQL()
{}

UseMySQL* UseMySQL::instance()
{
	return theInstance();
}

QString UseMySQL::getDepartmentID(QString name)
{
	QSqlQuery sql(QString("SELECT departmentID FROM tab_department WHERE departmentName = '%1'").arg(name), base);
	if (sql.next())
	{
		QString res = sql.value(0).toString();
		return res;
	}
	return QString();
}

bool UseMySQL::getDepartmentInfo(QString id, QStringList& fileds, QVector<QVector<QString>>& info)
{
	QSqlQueryModel model;
	QString companyID = getDepartmentID(QString::fromLocal8Bit("公司群"));
	if (id == companyID)
	{
		model.setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees"),base);
	}
	else
	{
		model.setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees WHERE departmentID = %1").arg(id), base);
	}
	int rowCount = model.rowCount();
	int columnCount = model.columnCount();
	if (rowCount <= 0 || columnCount <= 0) return false;
	
	for (int j = 0; j < columnCount;j++)
	{
		fileds.push_back(model.record(0).fieldName(j));//获取字段名称
	}

	for (int i = 0; i < rowCount;i++)
	{
		QVector<QString> rowTemp;
		for (int j = 0; j < columnCount; j++)
		{
			//每次获取第i行的第j列数据
			rowTemp.push_back(model.record(i).value(j).toString());
		}
		info.push_back(rowTemp);
	}
	return true;
}

bool UseMySQL::getEmployeeInfo(QString id, QStringList& fileds, QVector<QString>& info)
{
	QSqlQueryModel model;
	model.setQuery(QString::fromLocal8Bit("SELECT * FROM tab_employees WHERE employeeID = %1").arg(id), base);

	int rowCount = model.rowCount();
	int columnCount = model.columnCount();
	if (rowCount <= 0 || columnCount <= 0) return false;

	QSqlRecord record = model.record(0);//获取第一行数据
	for (int j = 0;j < columnCount; j++)
	{
		fileds.push_back(record.fieldName(j));//获取字段名称
		info.push_back(record.value(j).toString());//获取字段值
	}
	return true;
}

QString UseMySQL::getDepartmentName(QString id)
{
	QSqlQuery sql(QString("SELECT departmentName FROM tab_department WHERE departmentID = '%1'").arg(id), base);
	if (sql.next())
	{
		QString res = sql.value(0).toString();
		return res;
	}
	return QString();
}

QString UseMySQL::addEmployee(QString departmentID, QString employeeName, QString picture)
{
	QSqlQuery sql(base);
	if(!sql.exec(QString("SELECT MAX(employeeID) FROM tab_accounts"))) return QString();
	sql.first();//指向第一条可用记录
	int employeeID = sql.value(0).toInt() + 1;

	if (!sql.exec(QString("INSERT INTO tab_accounts VALUES(%1,'%2')").arg(employeeID).arg(employeeName)))	return QString();

	if (!sql.exec(QString("INSERT INTO tab_employees(departmentID,employeeID,employeeName,picture,status) \
						  VALUES(%1,%2,'%3','%4',1)").arg(departmentID).arg(employeeID).arg(employeeName).arg(picture)))
	{
		sql.exec(QString("DELETE FROM tab_accounts WHERE employeeID = %1").arg(employeeID));
		return QString();
	}
	return QString::number(employeeID);
}

bool UseMySQL::deleteEmployee(QString employeeID)
{
	QSqlQuery sql(base);
	sql.exec(QString("SELECT * FROM tab_employees WHERE employeeID = %1").arg(employeeID));
	if (!sql.next()) return false;

	sql.exec(QString("UPDATE tab_employees SET status = 0 WHERE employeeID =  %1").arg(employeeID));
	return true;
}



bool UseMySQL::connectMySQL()
{
	qDebug() << QSqlDatabase::drivers();

	base = QSqlDatabase::addDatabase("QMYSQL","main");
	base.setDatabaseName("qtqq");
	base.setHostName("localhost");
	base.setUserName("root");
	base.setPassword("123456");
	base.setPort(3306);

	if (base.open())
	{
		QMessageBox::information(nullptr, "MYSQL", QString::fromLocal8Bit("连接成功"),QMessageBox::Yes);
		return true;
	}
	else
	{
		QMessageBox::critical(nullptr, "MYSQL", "连接失败: " + base.lastError().text());
		return false;
	}
}
