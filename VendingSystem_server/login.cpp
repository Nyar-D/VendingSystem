#include "login.h"


Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
    delete menu;
}

void Login::on_reset_btn_clicked()
{
    ui->username_le->clear();
    ui->password_le->clear();
}

void Login::on_login_btn_clicked()
{
    QString usernameInput = ui->username_le->text();
    QString passwordInput = ui->password_le->text();

//    if(usernameInput.isEmpty() || passwordInput.isEmpty())
//    {
//        ui->tips_lb->setText("请输入用户名或密码！");
//        return;
//    }

//    if(usernameInput == "admin" && passwordInput == "123456")
//    {
        menu = new Menu();
        this->hide();
        menu->show();
//    }
//    else
//    {
//        ui->tips_lb->setText("用户名或密码错误！");
//        return;
//    }
}
