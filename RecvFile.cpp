#include "RecvFile.h"
#include <qfiledialog.h>
#include <QFile>
#include <qtextstream.h>
#include <qmessagebox.h>
RecvFile::RecvFile(const QJsonObject& data,QWidget *parent)
	: BasicWindow(parent), fileObj(data)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	init();
}

RecvFile::~RecvFile()
{}

void RecvFile::setLabelMsg(const QString & msg)
{
	ui.label->setText(msg);
}

void RecvFile::init()
{
	initTitleBar(CLOSE_BUTTON, ":/Resources/MainWindow/qqlogoclassic.png");
	loadStyleSheet("ReceviveFile");
	this->move(200, 400);

}

void RecvFile::on_cancelBtn_clicked()
{
	this->close();
}

void RecvFile::on_okBtn_clicked()
{
	if (!fileObj.isEmpty())
	{
		QString fileName = fileObj[FILE_NAME].toString();
		QString fileLength = fileObj[FILE_LENGTH].toString();
		QString fileContent = fileObj[FILE_CONTENT].toString();

		if (fileLength.toInt() == fileContent.toUtf8().length())
		{
			//获取文件保存路径
			QString dir = QFileDialog::getExistingDirectory(this, QString::fromUtf8("选择文件保存路径"), "/");
			if (!dir.isEmpty())
			{
				QString filePath = dir + "/" + fileName;
				QFile file(filePath);
				if (file.open(QFile::WriteOnly))
				{
					QTextStream stream(&file);
					stream << fileContent;
					QMessageBox::information(this, QString::fromUtf8("提示"), QString::fromUtf8("接收文件%1成功").arg(fileName));
					file.close();
					return;
				}
			}
		}
	}
	QMessageBox::information(this, QString::fromUtf8("提示"), QString::fromUtf8("接收文件失败"));
	this->close();
	return;
}
