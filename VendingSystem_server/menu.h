#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QtSql>
#include "ui_menu.h"
#include "ui_userreg.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>
#include "dbhandler.h"
#include "usertable.h"

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

signals:
    void GetAdvTable();
    void threadExit();

private slots:
    void on_newUser_btn_clicked();

    void on_refresh_btn_clicked();

    void on_batchDelete_btn_clicked();

    void show_adv_table(QSortFilterProxyModel *);
//    void show_QMessageBox(int, )

private:
    void back_btn_clicked();
    void add_btn_clicked();
    void prepareTheTable(void);

private:
    Ui::Menu *ui;
    Ui::UserReg *ui_reg;
    QWidget *userRegWidget;
    QSqlDatabase db;
    TableModel* model;
    QSortFilterProxyModel *sqlproxy;
    ButtonDelegate *m_buttonDelegate;
};


#endif // MENU_H
