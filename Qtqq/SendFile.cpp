#include "SendFile.h"
#include <qfiledialog.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qjsonobject.h>
#include <qfileinfo.h>
#include "WindowManager.h"
SendFile::SendFile(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	init();
}

SendFile::~SendFile()
{}

void SendFile::init()
{
	initTitleBar(CLOSE_BUTTON, ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("SendFile");

	this->move(50, 400);

	connect(this, SIGNAL(signalSendFile(QJsonValue)), WindowManager::instance()->getWindowShell(), SLOT(sendMsgToServer(QJsonValue)));
}

void SendFile::on_openBtn_clicked()
{
	filePath = QFileDialog::getOpenFileName(this,QString::fromUtf8("选择文件"),"/", QString::fromUtf8("所有文件(*.*)"));
	ui.lineEdit->setText(filePath);
}

void SendFile::on_sendBtn_clicked()
{
	if (filePath.isEmpty())return;

	QFile file(filePath);
	if (!file.open(QFile::ReadOnly))
	{
		QMessageBox::information(this, QString::fromUtf8("提示"), QString::fromUtf8("打开文件%1失败").arg(filePath));
		ui.lineEdit->clear();
		filePath = "";
		return;
	}
	//封装json对象
	QJsonObject jsonObj;
	QString fileName = QFileInfo(filePath).fileName();
	jsonObj.insert(FILE_NAME, fileName);
	QByteArray fileContent = file.readAll();
	jsonObj.insert(FILE_LENGTH, QString::number(fileContent.length()));
	jsonObj.insert(FILE_CONTENT, QString(fileContent));

	emit signalSendFile(jsonObj);
	file.close();
	this->close();
}
