#ifndef ADVERTISE_H
#define ADVERTISE_H

#include <QWidget>
#include "shop.h"

namespace Ui {
class advertise;
}

class advertise : public QWidget
{
    Q_OBJECT

public:
    explicit advertise(QWidget *parent = 0);
    ~advertise();

private slots:
    void on_enter_btn_clicked();

private:
    Ui::advertise *ui;
    Shop *shop;
};

#endif // ADVERTISE_H
