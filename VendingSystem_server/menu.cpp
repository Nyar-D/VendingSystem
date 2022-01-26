#include "menu.h"
#include "ui_menu.h"
#include "ui_userreg.h"
#include <QDebug>
#include <QMessageBox>

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{


    ui->setupUi(this);
}

Menu::~Menu()
{
    delete ui;
    delete ui_reg;
    delete w;
}

void Menu::on_newUser_btn_clicked()
{
    w = new QWidget();
    ui_reg = new Ui::UserReg();
    ui_reg->setupUi(w);
    connect(ui_reg->back_btn, SIGNAL(clicked(bool)), this, SLOT(back_btn_clicked()));
    connect(ui_reg->add_btn, SIGNAL(clicked(bool)), this, SLOT(add_btn_clicked()));
    w->show();
}

void Menu::back_btn_clicked()
{
    w->close();
}

void Menu::add_btn_clicked()
{
    if(ui_reg->username_le->text().isEmpty()
            ||ui_reg->password_le->text().isEmpty()
            ||ui_reg->contact_le->text().isEmpty())
    {
        ui_reg->tips_lb->setText("请检查输入！");
        return;
    }
    qDebug() << "------add------";
    qDebug() << "Name:" << ui_reg->username_le->text()
             << "Sex:" << ui_reg->man->objectName()
             << "Contact" << ui_reg->contact_le->text();
    w->close();
    QMessageBox::information(this, "Tips", "添加成功！");
}
