#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;    // 进入登录界面
    w.show();

    return a.exec();
}
