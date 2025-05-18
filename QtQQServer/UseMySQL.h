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

private:
	bool connectMySQL();
private:
	QSqlDatabase base;

};
