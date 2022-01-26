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
