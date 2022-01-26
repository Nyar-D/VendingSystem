#ifndef SHOP_H
#define SHOP_H

#include <QMainWindow>

namespace Ui {
class Shop;
}

class Shop : public QMainWindow
{
    Q_OBJECT

public:
    explicit Shop(QWidget *parent = 0);
    ~Shop();

private slots:
    void on_prevPage_btn_clicked();

    void on_nextPage_btn_clicked();

private:
    Ui::Shop *ui;
};

#endif // SHOP_H
