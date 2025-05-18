#pragma once

#include <QObject>
#include <QSqlDatabase>
class UseMySQL  : public QObject
{
	Q_OBJECT

public:
	UseMySQL(QObject *parent = nullptr);
	~UseMySQL();
	
	static UseMySQL* instance();//单例模式

public:

	bool verifyLogin(const QString& account, const QString& password);//验证登录

	QString getUserName(const QString& account);//获取用户名

	QString getUserSign(const QString& account);//获取用户签名

	QString getUserHeadPixmap(const QString& account);//获取用户头像

	QList<int> getUserGroup(const QString& account);//获取用户群组

	QString getGroupHeadPixmap(const int groupID);//获取群组头像

	QString getGroupName(const int groupID);//获取群组名称

	QString getUserDepartment(const QString& account);//获取用户部门

	QList<int> getUserList();

	bool getRegisterObjectInfo(const QString& id,QList<QPair<QString,QString>>& infoList);

private:
	bool connectMySQL();
private:
	QSqlDatabase base;
};
