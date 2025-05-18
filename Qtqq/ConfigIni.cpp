#include "ConfigIni.h"
#include <QSettings>
#include <qapplication.h>
#include <QFile>
#include "define.h"
//保存颜色值到ini文件
void ConfigIni::saveColor(const QColor& color)
{
	const QString path = INI_PATH("color.ini");
	QSettings set(path,QSettings::IniFormat);

	//将设置的颜色值保存到ini文件中
	set.setValue("COLOR/red", color.red());
	set.setValue("COLOR/green", color.green());
	set.setValue("COLOR/blue", color.blue());
}
//读取ini文件中的颜色值
QColor ConfigIni::getColor()
{
	const QString path = INI_PATH("color.ini");

	if (!QFile::exists(path))
	{
		saveColor(QColor(22, 154, 218));	//如果文件不存在则创建一个默认的ini文件
	}
	QSettings set(path, QSettings::IniFormat);

	//读取ini文件中的颜色值
	int red = set.value("COLOR/red").toInt();
	int green = set.value("COLOR/green").toInt();
	int blue = set.value("COLOR/blue").toInt();

	return QColor(red,green,blue);
}

//保存用户配置到ini文件
void ConfigIni::saveUserProfile(const QString& account, const QString& password)
{
	const QString path = INI_PATH("user.ini");
	QSettings set(path, QSettings::IniFormat);
	//将设置的账号密码保存到ini文件中
	set.setValue("USER/account", account);
	set.setValue("USER/password", password);
}

bool ConfigIni::getUserProfile(QString& account,  QString& password)
{
	const QString path = INI_PATH("user.ini");

	if (!QFile::exists(path))
	{
		return false;
	}
	QSettings set(path, QSettings::IniFormat);
	account = set.value("USER/account").toString();
	password = set.value("USER/password").toString();
	return true;
}
