#include "advertisement.h"
#include "ui_advertisement.h"

Advertisement::Advertisement(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Advertisement)
{
    ui->setupUi(this);
}

Advertisement::~Advertisement()
{
    delete ui;
}

void Advertisement::on_pushButton_clicked()
{
    shop = new Shop();
    this->hide();
    shop->show();
}
