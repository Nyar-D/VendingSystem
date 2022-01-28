#include "advertisement.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString path("../vendingSys_adv");
    Advertisement w(path);
    w.show();

    return a.exec();
}
