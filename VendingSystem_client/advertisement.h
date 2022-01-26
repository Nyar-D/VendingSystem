#ifndef ADVERTISEMENT_H
#define ADVERTISEMENT_H

#include <QMainWindow>

namespace Ui {
class Advertisement;
}

class Advertisement : public QMainWindow
{
    Q_OBJECT

public:
    explicit Advertisement(QWidget *parent = 0);
    ~Advertisement();

private:
    Ui::Advertisement *ui;
};

#endif // ADVERTISEMENT_H
