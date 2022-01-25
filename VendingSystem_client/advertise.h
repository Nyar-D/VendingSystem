#ifndef ADVERTISE_H
#define ADVERTISE_H

#include <QWidget>

namespace Ui {
class advertise;
}

class advertise : public QWidget
{
    Q_OBJECT

public:
    explicit advertise(QWidget *parent = 0);
    ~advertise();

private:
    Ui::advertise *ui;
};

#endif // ADVERTISE_H
