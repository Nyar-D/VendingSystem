#include "advertise.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    advertise w;
    w.show();

    return a.exec();
}
