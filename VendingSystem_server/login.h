#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "menu.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_reset_btn_clicked();

    void on_login_btn_clicked();

private:
    Ui::Login *ui;
    Menu *menu;
};

#endif // LOGIN_H
