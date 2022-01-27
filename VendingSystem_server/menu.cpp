#include "menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    prepareTheTable();
    ui->setupUi(this);
    Menu::on_refresh_btn_clicked();
}

Menu::~Menu()
{
    delete ui;
    delete ui_reg;
    delete userRegWidget;
    delete model;
}

void Menu::on_newUser_btn_clicked()
{
    userRegWidget = new QWidget();
    ui_reg = new Ui::UserReg();
    ui_reg->setupUi(userRegWidget);
    connect(ui_reg->back_btn, SIGNAL(clicked(bool)), this, SLOT(back_btn_clicked()));
    connect(ui_reg->add_btn, SIGNAL(clicked(bool)), this, SLOT(add_btn_clicked()));
    userRegWidget->show();
}

void Menu::back_btn_clicked()
{
    userRegWidget->close();
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
    if(!QString::compare(sex, "man"))
        sex = "男";
    else
        sex = "女";

    if(name.isEmpty() || username.isEmpty() || password.isEmpty() || contact.isEmpty() || contact.size() != 11)
    {
        ui_reg->tips_lb->setText("请检查输入！");
        return;
    }

    qDebug() << "------------add------------";
    qDebug() << "Name:" << name
             << "Sex:" << sex
             << "Contact" << contact;
    qDebug() << "---------------------------";


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

        sql_query.bindValue(":Name",name);
        sql_query.bindValue(":Username", username);
        sql_query.bindValue(":Password", password);
        sql_query.bindValue(":Sex", sex);
        sql_query.bindValue(":Contact", contact);
        sql_query.bindValue(":RegDate", QDate::currentDate().toString("yyyy-MM-dd"));

        if(!sql_query.exec())
        {
            qDebug() << QObject::tr("User table failed to insert!");
            qDebug() << sql_query.lastError();
            QMessageBox::critical(this, "Error", "添加失败！");

        }
        else
        {
            qDebug() << "User insert!";
            Menu::on_refresh_btn_clicked();
            QMessageBox::information(this, "Tip", "添加成功！");
            userRegWidget->close();
        }
        db.close();
    }

}


void Menu::prepareTheTable(void)
{
    QString dbFilePath(USERDBFILE);
    QFileInfo fileinfo(dbFilePath);
    if(QSqlDatabase::contains(USERDBCONNECTION))
    {
        qDebug() << "Connection:dbUser existed.";
        db = QSqlDatabase::database(USERDBCONNECTION);
    }
    else
    {
        qDebug() << "Connection:dbUser not existed.";
        db = QSqlDatabase::addDatabase("QSQLITE", USERDBCONNECTION);
        db.setDatabaseName(USERDBNAME);
        if(!fileinfo.isFile())
        {
            if(db.open())
            {
                QString create_sql_user = "create table user(ID INTEGER PRIMARY KEY AUTOINCREMENT,Name varchar(24),Username varchar(24),Password varchar(24),Sex varchar(5),Contact varchar(11),RegDate date)";
                QSqlQuery sql_query = QSqlQuery(db);
                sql_query.prepare(create_sql_user);
                if(!sql_query.exec())
                {
                    qDebug() << QObject::tr("User table failed to create!");
                    qDebug() << sql_query.lastError();
                    QFile::remove(USERDBFILE);
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

void Menu::on_refresh_btn_clicked()
{
    qDebug() << "refresh";
    if(!db.open())
    {
        qDebug()<<"no open";
        QMessageBox::critical(this, "Error", "无法打开数据库！");
        return;
    }
    else
    {
        model = new QSqlQueryModel(ui->user_table);
        model->setQuery("select * from user", db);
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("姓名"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("用户名"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("密码"));
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("性别"));
        model->setHeaderData(5,Qt::Horizontal,QObject::tr("联系方式"));
        model->setHeaderData(6,Qt::Horizontal,QObject::tr("注册日期"));
        sqlproxy = new QSortFilterProxyModel(this);
        sqlproxy->setSourceModel(model);
        ui->user_table->setModel(sqlproxy);
    }
}
