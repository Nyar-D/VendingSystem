#ifndef ADVERTISEMENT_H
#define ADVERTISEMENT_H

#include <QMainWindow>
#include "shop.h"

namespace Ui {
class Advertisement;
}

class Advertisement : public QMainWindow
{
    Q_OBJECT

public:
    explicit Advertisement(QWidget *parent = 0);
    ~Advertisement();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Advertisement *ui;
    Shop *shop;
};

#endif // ADVERTISEMENT_H
