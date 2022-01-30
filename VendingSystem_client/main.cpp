#include "advertisement.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Advertisement w;
    w.show();

    return a.exec();
}
