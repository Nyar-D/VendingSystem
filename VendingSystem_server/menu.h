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

#define DBFILE "./UserDatabase.db"
#define DBNAME "UserDatabase.db"
#define DBCONNECTION "dbUser"

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
    void back_btn_clicked();

    void add_btn_clicked();

    void editUser_btn_clicked(const QModelIndex &);

    void deleteUser_btn_clicked(const QModelIndex &);

private:

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
