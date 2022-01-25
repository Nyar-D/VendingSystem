#include "advertise.h"
#include "ui_advertise.h"

advertise::advertise(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::advertise)
{
    ui->setupUi(this);
}

advertise::~advertise()
{
    delete ui;
}

void advertise::on_enter_btn_clicked()
{

}
