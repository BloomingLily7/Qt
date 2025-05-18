#pragma once

#include <QObject>

class NotifyManager  : public QObject
{
	Q_OBJECT

public:
	NotifyManager(QObject *parent = nullptr);
	~NotifyManager();

	static NotifyManager* instance();

signals:
	void skinChanged(const QColor& color);
public:
	void notifySkinChanged(const QColor& color);

};
