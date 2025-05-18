#include "UserLogin.h"
#include <QtWidgets/QApplication>

#include "TitleBar.h"
#include "ConfigIni.h"
#include "UseMySQL.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	a.setQuitOnLastWindowClosed(false);//设置最后一个窗口关闭时不退出应用程序

    UserLogin w;
    w.show();

	ConfigIni::getColor();

    

    return a.exec();
}
