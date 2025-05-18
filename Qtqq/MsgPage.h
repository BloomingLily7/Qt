#pragma once

#include <QObject>
#include <qwebenginepage.h>
#include <qwebengineview.h>
#include <qjsonvalue.h>
class MsgHtmlObj  : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString msgRHtmlTmpl MEMBER msgRight NOTIFY msgHtmlChanged)
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER msgLeft NOTIFY msgHtmlChanged)

public:
	MsgHtmlObj(QObject *parent,QString headPath = QString());
	~MsgHtmlObj();

signals:
	void msgHtmlChanged(const QString& msgHtml);//抑制警告

private:
	void initMsgHtml();
	QString getMsgHtml(const QString& fileName);

private:
	QString msgRight;
	QString msgLeft;

	QString leftHeadPath;//左侧头像路径
};


class MsgWebPage :public QWebEnginePage
{
	Q_OBJECT
public:
	MsgWebPage(QObject* parent) :QWebEnginePage(parent)
	{
	}
protected:
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame) override
	{
		if (url.scheme() == "qrc")//仅允许导航到qrc资源路径中的.html文件
		{
			return true;
		}
		return false;
	}
};

class MsgWebView :public QWebEngineView
{
	Q_OBJECT
public:
	MsgWebView(QWidget* parent = nullptr);
	~MsgWebView();

	void appendMsg(const QJsonArray& jsonMsg,QString obj);

signals:
	void signalSendMsg(QJsonValue data);
private:
	void registerChannelObject();//注册通道对象
	QString parseMsgFromJson(const QJsonArray& arr);
private:
	MsgHtmlObj* rightObj;//发送消息的对象
	QWebChannel* channel;//与网页通信的通道
};
