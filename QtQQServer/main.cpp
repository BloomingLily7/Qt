#include "QtQQServer.h"
#include <QtWidgets/QApplication>
#include "UseMySQL.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtQQServer w;
    w.show();

	



    return a.exec();
}
