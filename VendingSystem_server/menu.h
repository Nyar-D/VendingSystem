#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QtSql>
#include "ui_menu.h"
#include "ui_userreg.h"
#include "ui_useredit.h"
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
class UserEdit;
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
    void reg_back_btn_clicked();

    void reg_add_btn_clicked();

    void edit_btn_clicked();

    void edit_back_btn_clicked();

    void editUser_btn_clicked(const QModelIndex &);

    void deleteUser_btn_clicked(const QModelIndex &);

private:

    void prepareTheTable(void);

private:
    Ui::Menu *ui;   // 后台界面的主Ui
    Ui::UserReg *ui_reg;    // 新增用户时跳转的注册界面
    Ui::UserEdit *ui_edit;  // 编辑用户时跳转的编辑界面
    QWidget *GeneralWidget; // 跳转界面时<通用>的QWidget,不用重复创建了,如果单线程就用这个.
    QSqlDatabase db;    // 数据库,常用open()和close()
    TableModel* model;  // 已经重写的表格类,目前只负责用户的表格,想要负责其他表格的话,在下面声明其他对象


    QSortFilterProxyModel *sqlproxy;    // 负责表格列排序的代理类,具体用法可以看.cpp中实现,
                                        // 暂未尝试同时传入给两个表格,目前就一个用户表格使用
    ButtonDelegate *m_buttonDelegate;   // 负责表格内嵌按钮的操作,不同表格声明不同对象
};


#endif // MENU_H
