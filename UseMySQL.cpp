#include "UseMySQL.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <qsqlquerymodel.h>
#include "define.h"
Q_GLOBAL_STATIC(UseMySQL,theInstance)
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

bool UseMySQL::verifyLogin(const QString& account, const QString& password)
{
	QSqlQuery sql(QString("select code from tab_accounts where employeeID = %1").arg(account),base);
	if (sql.next())//如果有结果
	{
		QString code = sql.value(0).toString();
		if (code == password)
		{
			return true;
		}
		else
		{
			QMessageBox::information(nullptr, "tip", "password error", QMessageBox::Yes);
			return false;
		}
	}
	else
	{
		QMessageBox::information(nullptr, "tip", "account not exist", QMessageBox::Yes);
		return false;
	}
}

QString UseMySQL::getUserName(const QString& account)
{
	QSqlQuery sql(QString("SELECT employeeName FROM tab_employees WHERE employeeID = %1").arg(account),base);
	if (sql.next())
	{
		QString res = sql.value(0).toString();
		return res;
	}
	return QString();
}

QString UseMySQL::getUserSign(const QString& account)
{
	QSqlQuery sql(QString("SELECT employee_sign FROM tab_employees WHERE employeeID = %1").arg(account), base);
	if (sql.next())
	{
		QString res = sql.value(0).toString();
		return res;
	}
	return QString();
}

QString UseMySQL::getUserHeadPixmap(const QString& account)
{
	QSqlQuery sql(QString("SELECT picture FROM tab_employees WHERE employeeID = %1").arg(account),base);
	if (sql.next())
	{
		QString res = sql.value(0).toString();
		return res;
	}
	return QString();
}

QList<int> UseMySQL::getUserGroup(const QString& account)
{
	QList<int> result;
	QSqlQuery sql(base);
	//公司群
	if (sql.exec(QString("SELECT departmentID FROM tab_department WHERE departmentName = '公司群'")))
	{
		qDebug() << sql.lastError();
		if (sql.next()) result.append(sql.value(0).toInt());
	}
	//部门群
	if (sql.exec(QString("SELECT departmentID FROM tab_employees WHERE employeeID = '%1'").arg(account)))
	{
		qDebug() << sql.lastError();
		if (sql.next()) result.append(sql.value(0).toInt());
	}
	return result;
}

QString UseMySQL::getGroupHeadPixmap(const int groupID)
{
	QSqlQuery sql(QString("SELECT picture FROM tab_department WHERE departmentID = %1").arg(groupID), base);
	if (sql.next())
	{
		QString res = sql.value(0).toString();
		return res;
	}
	return QString();
}

QString UseMySQL::getGroupName(const int groupID)
{
	QSqlQuery sql(QString("SELECT departmentName FROM tab_department WHERE departmentID = %1").arg(groupID), base);
	if (sql.next())
	{
		QString res = sql.value(0).toString();
		return res;
	}
	return QString();
}

QString UseMySQL::getUserDepartment(const QString& account)
{
	QSqlQuery sql(QString("SELECT departmentName FROM tab_department WHERE departmentID =  \
						 (SELECT departmentID FROM tab_employees WHERE employeeID = %1) ").arg(account), base);
	if (sql.next())
	{
		QString res = sql.value(0).toString();
		return res;
	}
	return QString();
}

QList<int> UseMySQL::getUserList()
{
	QSqlQueryModel sql;//查询模型
	sql.setQuery(QString("SELECT employeeID FROM tab_employees WHERE status = 1;"),base);

	QList<int> result;
	int rowCount = sql.rowCount();
	if (rowCount)
	{
		for (int i = 0; i < rowCount; i++)
		{
			QModelIndex index = sql.index(i, 0);//获取第i行第0列的索引
			//获取索引对应的值
			result.append(sql.data(index).toInt());
		}
	}

	return result;
}

bool UseMySQL::getRegisterObjectInfo(const QString& id,QList<QPair<QString, QString>>& infoList)
{
	//获取公司群id
	QSqlQuery sql(QString("SELECT departmentID FROM tab_department WHERE departmentName = '公司群'"), base);
	
	if (!sql.next())
	{
		return false;
	}
	QString companyID = sql.value(0).toString();

	//判断聊天窗口类型
	QSqlQueryModel model;
	if (id == companyID)
	{
		model.setQuery("SELECT employeeID,picture FROM tab_employees WHERE status = 1",base);
	}
	else
	{
		if (id.length() <= GROUP_ID_LENGTH)//其他群聊
		{
			model.setQuery(QString("SELECT employeeID,picture FROM tab_employees WHERE status = 1 AND departmentID = %1").arg(id),base);
		}
		else//单聊
		{
			model.setQuery(QString("SELECT employeeID,picture FROM tab_employees WHERE status = 1 AND employeeID = %1").arg(id), base);
		}
	}
	int rowCount = model.rowCount();
	if (rowCount <= 0) return false;
	for (int i = 0;i < rowCount;i++)
	{
		QModelIndex accountIndex = model.index(i, 0);//获取第i行第0列的索引
		QModelIndex pictureIndex = model.index(i, 1);//获取第i行第1列的索引

		//获取索引对应的值
		QString account = model.data(accountIndex).toString();
		QString picture = model.data(pictureIndex).toString();

		infoList.append(qMakePair<QString,QString>(account, picture));
	}
	return true;
}

bool UseMySQL::connectMySQL()
{
	qDebug() << QSqlDatabase::drivers();

	base = QSqlDatabase::addDatabase("QMYSQL","main");
	base.setDatabaseName("qtqq");//数据库名称
	base.setHostName("localhost");//主机名
	base.setUserName("root");
	base.setPassword("123456");
	base.setPort(3306);//端口号

	if (base.open())
	{
		QMessageBox::information(nullptr, "tip", "succeed",QMessageBox::Yes);
		return true;
	}
	else
	{
		QMessageBox::information(nullptr, "tip", "failed", QMessageBox::Cancel);
		qDebug() << base.lastError().text();
		return false;
	}
}
