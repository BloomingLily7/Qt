#pragma once

#include <QObject>
#include <QSqlDatabase>
//#include <qstandarditemmodel.h>
class UseMySQL  : public QObject
{
	Q_OBJECT

public:
	UseMySQL(QObject *parent = nullptr);
	~UseMySQL();
	static UseMySQL* instance();

	QString getDepartmentID(QString name);

	bool getDepartmentInfo(QString id,QStringList& fileds,QVector<QVector<QString>>& info);

	bool getEmployeeInfo(QString id, QStringList& fileds, QVector<QString>& info);//��ȡԱ����Ϣ

	QString getDepartmentName(QString id);//��ȡ��������

	QString addEmployee(QString departmentID,QString employeeName,QString picture);

	bool deleteEmployee(QString employeeID);

private:
	bool connectMySQL();
private:
	QSqlDatabase base;

};
