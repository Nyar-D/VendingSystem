#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QtSql>

#define USERDBFILE "./UserDatabase.db"
#define USERDBNAME "UserDatabase.db"
#define USERDBCONNECTION "dbUser"

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

    void on_refresh_btn_clicked();

private:
    Ui::Menu *ui;
    Ui::UserReg *ui_reg;
    QWidget *userRegWidget;
    QSqlDatabase db;
    QSqlQueryModel* model;
};


#endif // MENU_H
