#ifndef SHOP_H
#define SHOP_H

#include <QWidget>

namespace Ui {
class Shop;
}

class Shop : public QWidget
{
    Q_OBJECT

public:
    explicit Shop(QWidget *parent = 0);
    ~Shop();

private:
    Ui::Shop *ui;
};

#endif // SHOP_H
