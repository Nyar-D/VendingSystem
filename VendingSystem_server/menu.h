#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QtSql>
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>

#include "ui_menu.h"
#include "ui_user_reg.h"
#include "ui_user_edit.h"
#include "ui_good_add.h"
#include "ui_good_edit.h"

#include "dbhandler.h"
#include "table.h"

#define DBFILE "./UserDatabase.db"
#define DBNAME "UserDatabase.db"
#define DBCONNECTION "dbUser"

namespace Ui {
class Menu;
class UserReg;
class UserEdit;
class GoodAdd;
class GoodEdit;
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

    void on_user_refresh_btn_clicked();

    void on_batchDelete_btn_clicked();

    void show_adv_table(QSortFilterProxyModel *);
//    void show_QMessageBox(int, )
    void reg_back_btn_clicked();

    void reg_add_btn_clicked();

    void user_edit_btn_clicked();

    void user_edit_back_btn_clicked();

    void editUser_btn_clicked(const QModelIndex &);

    void deleteUser_btn_clicked(const QModelIndex &);

    void on_addGoods_btn_clicked();

    void good_back_btn_clicked();

    void good_add_btn_clicked();

    void on_good_nameSearch_btn_clicked();

    void on_good_nameSearchandBack_btn_clicked();

    void editGood_btn_clicked(const QModelIndex &);

    void deleteGood_btn_clicked(const QModelIndex &);

    void good_edit_btn_clicked();

    void good_edit_back_btn_clicked();


private:
    void goodTable_refresh(void);
    void prepareTheTable(void);

private:
    Ui::Menu *ui;   // 后台界面的主Ui
    Ui::UserReg *ui_userReg;    // 新增用户时跳转的注册界面
    Ui::UserEdit *ui_userEdit;  // 编辑用户时跳转的编辑界面
    Ui::GoodAdd *ui_goodAdd;
    Ui::GoodEdit *ui_goodEdit;





    QWidget *GeneralWidget; // 跳转界面时<通用>的QWidget,不用重复创建了,如果单线程就用这个.
    QSqlDatabase db;    // 数据库,常用open()和close()


    TableModel* model_user;  // 已经重写的表格类,目前只负责用户的表格,想要负责其他表格的话,在下面声明其他对象
    TableModel* model_good;


    QSortFilterProxyModel *sqlproxy_user;    // 负责表格列排序的代理类,具体用法可以看.cpp中实现,
                                        // 暂未尝试同时传入给两个表格,目前就一个用户表格使用
    QSortFilterProxyModel *sqlproxy_good;



    user_BD *m_userBD;   // 负责表格内嵌按钮的操作,不同表格声明不同对象
    good_BD *m_goodBD;


};


#endif // MENU_H
