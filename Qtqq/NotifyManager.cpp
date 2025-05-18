#include "NotifyManager.h"
#include "ConfigIni.h"
Q_GLOBAL_STATIC(NotifyManager, theInstance)
NotifyManager::NotifyManager(QObject *parent)
	: QObject(parent)
{}

NotifyManager::~NotifyManager()
{}

NotifyManager* NotifyManager::instance()
{
	return theInstance();
}

void NotifyManager::notifySkinChanged(const QColor& color)
{
	emit skinChanged(color);
	ConfigIni::saveColor(color);
}
