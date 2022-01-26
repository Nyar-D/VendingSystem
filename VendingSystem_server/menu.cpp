#include "menu.h"
#include "ui_menu.h"
#include "ui_userreg.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>


Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    prepareTheTable();
    ui->setupUi(this);
}

Menu::~Menu()
{
    delete ui;
    delete ui_reg;
    delete w;
}

void Menu::on_newUser_btn_clicked()
{
    w = new QWidget();
    ui_reg = new Ui::UserReg();
    ui_reg->setupUi(w);
    connect(ui_reg->back_btn, SIGNAL(clicked(bool)), this, SLOT(back_btn_clicked()));
    connect(ui_reg->add_btn, SIGNAL(clicked(bool)), this, SLOT(add_btn_clicked()));
    w->show();
}

void Menu::back_btn_clicked()
{
    w->close();
}

void Menu::add_btn_clicked()
{
    QString name = ui_reg->name_le->text();
    QString username = ui_reg->username_le->text();
    QString password = ui_reg->password_le->text();
    QString contact = ui_reg->contact_le->text();
    QButtonGroup sexSelect;
    sexSelect.addButton(ui_reg->man);
    sexSelect.addButton(ui_reg->woman);
    QString sex = sexSelect.checkedButton()->objectName();

    if(name.isEmpty() || username.isEmpty() || password.isEmpty() || contact.isEmpty())
    {
        ui_reg->tips_lb->setText("请检查输入！");
        return;
    }

    qDebug() << "------------add------------";
    qDebug() << "Name:" << name
             << "Sex:" << sex
             << "Contact" << contact;
    qDebug() << "---------------------------";

    /*
    if(!db.open())
    {
        qDebug()<<"no open";
        QMessageBox::critical(this, "Error", "无法打开数据库！");
        return;
    }
    else
    {
        qDebug()<<"open";
        QString insert_sql_user = "insert into user(ID,Name,Username,Password,Sex,Contact,RegDate) values(:ID,:Name,:Username,:Password,:Sex,:Contact,:RegDate)";
        QSqlQuery sql_query = QSqlQuery(db);
        sql_query.prepare(insert_sql_user);

        sql_query.bindValue(":ID",);
        sql_query.bindValue(":Name",);
        sql_query.bindValue(":Username");
        sql_query.bindValue(":Password");
        sql_query.bindValue(":Sex");
        sql_query.bindValue(":Contact");
        sql_query.bindValue(":RegDate");
    }*/






    w->close();
    QMessageBox::information(this, "Tip", "添加成功！");
}


void Menu::prepareTheTable(void)
{
    QString dbFilePath("./UserDatabase.db");
    QFileInfo fileinfo(dbFilePath);
    if(QSqlDatabase::contains("dbUser"))
    {
        qDebug() << "Connection:dbUser existed.";
        db = QSqlDatabase::database("dbUser");
    }
    else
    {
        qDebug() << "Connection:dbUser not existed.";
        db = QSqlDatabase::addDatabase("QSQLITE", "dbUser");
        db.setDatabaseName("UserDatabase.db");
        if(!fileinfo.isFile())
        {
            if(db.open())
            {
                QString create_sql_user = "create table user(ID int primary key,Name varchar(30),Username varchar(30),Password varchar(30),Sex int(1),Contact varchar(11),RegDate date)";
                QSqlQuery sql_query = QSqlQuery(db);
                sql_query.prepare(create_sql_user);
                if(!sql_query.exec())
                {
                    qDebug() << QObject::tr("User table failed to create!");
                    qDebug() << sql_query.lastError();
                }
                else
                {
                    qDebug() << "User table created!";
                }
            }
            else
            {
                QMessageBox::information(NULL, "Error", "Fail to connect the SQLite!",
                                         QMessageBox::Yes);
            }
        }
    }
    db.close();
}
