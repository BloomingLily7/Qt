#include "UserLogin.h"
#include "CommonUtils.h"
#include <qmessagebox.h>
#include "ConfigIni.h"
#include "UseMySQL.h"
#include "CCMainWindow.h"
UserLogin::UserLogin(QWidget *parent)
    : BasicWindow(parent)
{
    ui.setupUi(this);
    init();
}

UserLogin::~UserLogin()
{

}

void UserLogin::init()
{
    initTitleBar(MIN_CLOSE_BUTTON,":/Resources/MainWindow/qqlogoclassic.png");
    loadStyleSheet("UserLogin");
    initPicture();
	initUser();

	UseMySQL::instance();	//连接数据库
}

void UserLogin::initPicture()
{
	QLabel* head = new QLabel(this);
    head->setFixedSize(68, 68);
	head->move(width() / 2 - 34, height() / 2 - 90);

    QPixmap src(":/Resources/MainWindow/app/logo.ico");
	QPixmap mask(":/Resources/MainWindow/head_mask.png");
	head->setPixmap(CommonUtils::getRoundPixmap(src, mask, head->size()));
}

void UserLogin::initUser()
{
	QString account,password;
	if (ConfigIni::getUserProfile(account, password))//成功获取
	{
		ui.editUserAccount->setText(account);
		ui.editPassword->setText(password);
	}
}

void UserLogin::on_loginBtn_clicked()
{
	QString account = ui.editUserAccount->text();
	QString password = ui.editPassword->text();

	// 判断账号密码是否为空
	if (account.isEmpty() || password.isEmpty())
	{
		QMessageBox::information(this, "警告", "请输入账号密码");
		return;
	}

	// 判断账号密码是否正确
	if (!UseMySQL::instance()->verifyLogin(account, password))
	{
		// 登录成功，跳转到主界面
		QMessageBox::information(this, "提示", "账号密码错误");
		return;
	}

	// 记住密码
	if (ui.checkBox->isChecked())
	{
		//将密码写入到配置文件中
		ConfigIni::saveUserProfile(account, password);
	}
	// 登录成功，跳转到主界面
	this->close();
	CCMainWindow* mainWindow = new CCMainWindow(account);
	mainWindow->show();
}
