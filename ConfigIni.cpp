#include "ConfigIni.h"
#include <QSettings>
#include <qapplication.h>
#include <QFile>
#include "define.h"
//������ɫֵ��ini�ļ�
void ConfigIni::saveColor(const QColor& color)
{
	const QString path = INI_PATH("color.ini");
	QSettings set(path,QSettings::IniFormat);

	//�����õ���ɫֵ���浽ini�ļ���
	set.setValue("COLOR/red", color.red());
	set.setValue("COLOR/green", color.green());
	set.setValue("COLOR/blue", color.blue());
}
//��ȡini�ļ��е���ɫֵ
QColor ConfigIni::getColor()
{
	const QString path = INI_PATH("color.ini");

	if (!QFile::exists(path))
	{
		saveColor(QColor(22, 154, 218));	//����ļ��������򴴽�һ��Ĭ�ϵ�ini�ļ�
	}
	QSettings set(path, QSettings::IniFormat);

	//��ȡini�ļ��е���ɫֵ
	int red = set.value("COLOR/red").toInt();
	int green = set.value("COLOR/green").toInt();
	int blue = set.value("COLOR/blue").toInt();

	return QColor(red,green,blue);
}

//�����û����õ�ini�ļ�
void ConfigIni::saveUserProfile(const QString& account, const QString& password)
{
	const QString path = INI_PATH("user.ini");
	QSettings set(path, QSettings::IniFormat);
	//�����õ��˺����뱣�浽ini�ļ���
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
