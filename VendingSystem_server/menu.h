#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class Menu;
class UserReg;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();

private slots:
    void on_newUser_btn_clicked();

    void back_btn_clicked();

    void add_btn_clicked();
    void prepareTheTable(void);

private:
    Ui::Menu *ui;
    Ui::UserReg *ui_reg;
    QWidget *w;
    QSqlDatabase db;

};


#endif // MENU_H
