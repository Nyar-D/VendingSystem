#include "menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    prepareTheTable();  // 准备数据库
    ui->setupUi(this);
    Menu::on_refresh_btn_clicked();

    QThread *thDBHandler = new QThread;    //创建处理数据库的线程
    DBHandler *iDBHandler = new  DBHandler;
    iDBHandler->moveToThread(thDBHandler);
    connect(this, SIGNAL(GetAdvTable()), iDBHandler, SLOT(handle_getAdvTable()));
    connect(iDBHandler, SIGNAL(ShowAdvTable(QSortFilterProxyModel*)), this, SLOT(show_adv_table(QSortFilterProxyModel*)));
    thDBHandler->start();

    emit GetAdvTable();
//    thDBHandler->stop();
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_newUser_btn_clicked() // 用户管理界面的新增用户
{
    GeneralWidget = new QWidget();
    ui_reg = new Ui::UserReg();
    ui_reg->setupUi(GeneralWidget);
    connect(ui_reg->back_btn, SIGNAL(clicked(bool)), this, SLOT(reg_back_btn_clicked()));
    connect(ui_reg->add_btn, SIGNAL(clicked(bool)), this, SLOT(reg_add_btn_clicked()));
    GeneralWidget->show();
}

void Menu::reg_back_btn_clicked()   // 新增用户界面的返回按钮
{
    GeneralWidget->close();
    delete ui_reg;
    delete GeneralWidget;
}

void Menu::reg_add_btn_clicked()    // 新增用户界面的添加按钮
{
    QString name = ui_reg->name_le->text();
    QString username = ui_reg->username_le->text();
    QString password = ui_reg->password_le->text();
    QString contact = ui_reg->contact_le->text();

    QButtonGroup sexSelect; // 通过Ui的RadioButton获得性别选择数据
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
        // 向用户表添加用户
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
            GeneralWidget->close();
        }
        db.close();
        delete ui_reg;
        delete GeneralWidget;
    }

}


void Menu::prepareTheTable(void) // 准备数据库
{
    QString dbFilePath(DBFILE); // 数据库路径
    QFileInfo fileinfo(dbFilePath);
    if(QSqlDatabase::contains(DBCONNECTION))    // 我是这样理解:查看有没有其他人创建数据库连接,有就拿来自己用
    {
        qDebug() << "Connection:dbUser existed.";
        db = QSqlDatabase::database(DBCONNECTION);
    }
    else    // 没有就创建一个连接供后面使用时直接open()
    {
        qDebug() << "Connection:dbUser not existed.";
        db = QSqlDatabase::addDatabase("QSQLITE", DBCONNECTION);
        db.setDatabaseName(DBNAME);
        // 目录下没有数据库文件就新建文件并创建user表
        if(!fileinfo.isFile())
        {
            if(db.open())
            {
                // 创建user表
                QString create_sql_user = "create table user(ID INTEGER PRIMARY KEY AUTOINCREMENT,Name varchar(24),Username varchar(24),Password varchar(24),Sex varchar(5),Contact varchar(11),RegDate date)";
                QSqlQuery sql_query = QSqlQuery(db);
                sql_query.prepare(create_sql_user);
                if(!sql_query.exec())
                {
                    qDebug() << QObject::tr("User table failed to create!");
                    qDebug() << sql_query.lastError();
                    QFile::remove(DBFILE);  // create没有成功时也会新建数据库文件,这样会导致可能没有用户表就进行其他语句
                                            // 保证一定有用户表就删掉数据库文件下次运行再创建
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
    return;
}

// 刷新表格,一般在执行完各种语句后都要刷新一遍
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
        // 优化代码,防止new的资源没有释放掉又再次new
        if(!model)
            delete model;
        if(!m_buttonDelegate)
            delete m_buttonDelegate;
        if(!sqlproxy)
            delete sqlproxy;

        model = new TableModel();   // 继承下来重写的类,负责表格内嵌勾选框
        m_buttonDelegate = new ButtonDelegate(ui->user_table);  // 继承下来重写的类,负责表格内嵌按钮
        // 连接操作,使得点击表格内嵌按钮时,触发信号进行处理
        connect(m_buttonDelegate, SIGNAL(sig_editUser(QModelIndex)), this, SLOT(editUser_btn_clicked(QModelIndex)));
        connect(m_buttonDelegate, SIGNAL(sig_deleteUser(QModelIndex)), this, SLOT(deleteUser_btn_clicked(QModelIndex)));
        model->setQuery("select * from user", db);
        // 设置表头
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("姓名"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("用户名"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("密码"));
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("性别"));
        model->setHeaderData(5,Qt::Horizontal,QObject::tr("联系方式"));
        model->setHeaderData(6,Qt::Horizontal,QObject::tr("注册日期"));
        // 插入新的一列用于显示内嵌按钮
        model->insertColumn(7);
        model->setHeaderData(7,Qt::Horizontal,QObject::tr("操作"));
        // 显示内嵌按钮
        ui->user_table->setItemDelegateForColumn(7, m_buttonDelegate);
        // 用于排序表格列的排序过滤器代理模型
        sqlproxy = new QSortFilterProxyModel();
        sqlproxy->setSourceModel(model);
        ui->user_table->setModel(sqlproxy);
        // 设置表格每列拉伸填充画面
        ui->user_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        db.close();
    }
}

// 用户表格的批量删除按钮
void Menu::on_batchDelete_btn_clicked()
{
    // 存放表格内嵌勾选框的ID号,勾选状态的哈希表
    QMap<int,QPair<QVariant, Qt::CheckState> > checkedMap = model->check_state_map;
    // 将哈希表中的数据提取出来组成List
    QList<QPair<QVariant, Qt::CheckState> > valuesList = checkedMap.values();
    // 未勾选时点击"批量删除"的提示框
    if(!checkedMap.size())
    {
        QMessageBox::information(this, "Tip", "请选择要删除的用户！");
        return;
    }
    else
    {
        // 由于勾选后哈希表会写入数据,取消勾选后哈希表不会删除数据,但会将状态设置成未勾选
        for(int i = 0; i < checkedMap.size(); i++)
        {
            // 遍历出来有勾选的就停止遍历进行删除,否则就提示
            if(valuesList.at(i).second == Qt::Checked)
                break;
            else
            {
                QMessageBox::information(this, "Tip", "请选择要删除的用户！");
                return;
            }
        }
    }

    // 双向选择的逻辑
    QMessageBox msg;
    msg.setText("确定要删除所选择的用户吗？");
    msg.setWindowTitle("Warning");
    QPushButton *yes_btn = msg.addButton("确认", QMessageBox::AcceptRole);
    QPushButton *no_btn = msg.addButton("取消", QMessageBox::RejectRole);
    msg.exec();
    if(msg.clickedButton() == yes_btn)
    {
        qDebug() << "yes";
        if(!db.open())
        {
            qDebug()<<"no open";
            QMessageBox::critical(this, "Error", "无法打开数据库！");
            return;
        }
        else
        {
            qDebug()<<"open";
            QVariantList GroupIDs;
            QString delete_sql_user = "delete from user where ID=?";
            QSqlQuery sql_query = QSqlQuery(db);
            sql_query.prepare(delete_sql_user);

            for(int i = 0; i< valuesList.size(); i++)
            {
                // 将所有状态为勾选的ID追加写进GroupIDs
                if(valuesList.at(i).second == Qt::Checked)
                    GroupIDs.append(valuesList.at(i).first.toInt());
            }

            qDebug() << "sql:" << GroupIDs;
            sql_query.addBindValue(GroupIDs);
            // 可行的批量删除,用SQLite一条语句删除多个用户试过没用
            if(!sql_query.execBatch())
            {
                qDebug() << QObject::tr("User table failed to delete!");
                qDebug() << sql_query.lastError();
                QMessageBox::critical(this, "Error", "删除失败！");

            }
            else
            {
                qDebug() << "User delete!";
                Menu::on_refresh_btn_clicked();
                QMessageBox::information(this, "Tip", "删除成功！");
            }
            db.close();
        }
    }
    else if(msg.clickedButton() == no_btn)  // 点击"批量删除"后又点击"取消"会清空所有勾选状态
    {
        qDebug() << "no";
        model->check_state_map.clear();
        return;
    }
    return;
}

void Menu::show_adv_table(QSortFilterProxyModel *sqlproxy)
{
    qDebug()<<"Menu: show_adv_table";
    ui->adv_table->setModel(sqlproxy);
}

void Menu::editUser_btn_clicked(const QModelIndex &index)   // 表格界面的用户编辑按钮
{
    qDebug() << "edit-index:" << index;
    QModelIndex map_index = sqlproxy->mapToSource(index);   // 映射到原数据可以使得排序后的表格所内嵌的按钮ID号是该行的
    GeneralWidget = new QWidget();
    ui_edit = new Ui::UserEdit();
    ui_edit->setupUi(GeneralWidget);
    connect(ui_edit->back_btn, SIGNAL(clicked(bool)), this, SLOT(edit_back_btn_clicked()));
    connect(ui_edit->edit_btn, SIGNAL(clicked(bool)), this, SLOT(edit_btn_clicked()));
    // 该语句可通过index(QModelIndex类)访问表格中其他行列
    // 下面是通过编辑按钮的下标访问其他数据
    QString user_id = model->data(map_index.sibling(map_index.row(), 0), Qt::DisplayRole).toString();
    QString user_name = model->data(map_index.sibling(map_index.row(), 1), Qt::DisplayRole).toString();
    QString user_username = model->data(map_index.sibling(map_index.row(), 2), Qt::DisplayRole).toString();
    QString user_password = model->data(map_index.sibling(map_index.row(), 3), Qt::DisplayRole).toString();
    QString user_contact = model->data(map_index.sibling(map_index.row(), 5), Qt::DisplayRole).toString();
    QString user_sex = model->data(map_index.sibling(map_index.row(), 4), Qt::DisplayRole).toString();
    // 让用户编辑界面显示编辑之前的用户数据
    ui_edit->id_le->setText(user_id);
    ui_edit->name_le->setText(user_name);
    ui_edit->username_le->setText(user_username);
    ui_edit->password_le->setText(user_password);
    ui_edit->contact_le->setText(user_contact);
    if(user_sex == "男")
    {
        ui_edit->man->setChecked(true);
    }
    else if(user_sex == "女")
    {
        ui_edit->woman->setChecked(true);
    }

    GeneralWidget->show();
}


void Menu::deleteUser_btn_clicked(const QModelIndex &index) //表格界面的用户删除按钮
{
    qDebug() << "delete-index:" << index;
    QModelIndex map_index = sqlproxy->mapToSource(index);   // 映射到原数据可以使得排序后的表格所内嵌的按钮ID号是该行的
    QMessageBox msg;    // 双向选择的逻辑
    msg.setText("确定要删除所选择的用户吗？");
    msg.setWindowTitle("Warning");
    QPushButton *yes_btn = msg.addButton("确认", QMessageBox::AcceptRole);
    QPushButton *no_btn = msg.addButton("取消", QMessageBox::RejectRole);
    msg.exec();
    if(msg.clickedButton() == yes_btn)
    {
        qDebug() << "yes";

        if(!db.open())
        {
            qDebug()<<"no open";
            QMessageBox::critical(this, "Error", "无法打开数据库！");
            return;
        }
        else
        {
            qDebug()<<"open";
            // 该语句可通过index(QModelIndex类)访问表格中其他行列,下面是通过删除按钮的下标访问ID号
            QString delete_id = model->data(map_index.sibling(map_index.row(), 0), Qt::DisplayRole).toString();
            qDebug() << delete_id;
            QString delete_sql_user = "delete from user where ID=:ID";
            QSqlQuery sql_query = QSqlQuery(db);
            sql_query.prepare(delete_sql_user);
            sql_query.bindValue(":ID",delete_id);
            if(!sql_query.exec())
            {
                qDebug() << QObject::tr("User table failed to delete!");
                qDebug() << sql_query.lastError();
                QMessageBox::critical(this, "Error", "删除失败！");
            }
            else
            {
                qDebug() << "User delete!";
                Menu::on_refresh_btn_clicked();
                QMessageBox::information(this, "Tip", "删除成功！");
            }
            db.close();
            return;
        }
    }
    else if(msg.clickedButton() == no_btn)
    {
        qDebug() << "no";
        return;
    }
    return;
}

void Menu::edit_btn_clicked()   // 用户编辑界面的编辑按钮
{
    qDebug() << "edit_btn_clicked"; // 通过Ui的LineEdit获得用户数据
    QString name = ui_edit->name_le->text();
    QString username = ui_edit->username_le->text();
    QString password = ui_edit->password_le->text();
    QString contact = ui_edit->contact_le->text();
    QString id = ui_edit->id_le->text();

    QButtonGroup sexSelect; // 通过Ui的RadioButton获得性别选择数据
    sexSelect.addButton(ui_edit->man);
    sexSelect.addButton(ui_edit->woman);
    QString sex = sexSelect.checkedButton()->objectName();
    if(!QString::compare(sex, "man"))
        sex = "男";
    else
        sex = "女";

    if(name.isEmpty() || username.isEmpty() || password.isEmpty() || contact.isEmpty() || contact.size() != 11)
    {
        ui_edit->tips_lb->setText("请检查输入！");
        return;
    }

    qDebug() << "------------edit------------";
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
        qDebug()<<"open";   // 相关update语句负责修改用户数据
        QString update_sql_user = "update user set Name=:Name, Username=:Username, Password=:Password, Sex=:Sex, Contact=:Contact where ID=:ID";
        QSqlQuery sql_query = QSqlQuery(db);
        sql_query.prepare(update_sql_user);

        sql_query.bindValue(":Name",name);
        sql_query.bindValue(":Username", username);
        sql_query.bindValue(":Password", password);
        sql_query.bindValue(":Sex", sex);
        sql_query.bindValue(":Contact", contact);
        sql_query.bindValue(":ID", id);

        if(!sql_query.exec())
        {
            qDebug() << QObject::tr("User table failed to edit!");
            qDebug() << sql_query.lastError();
            QMessageBox::critical(this, "Error", "编辑失败！");

        }
        else
        {
            qDebug() << "User edit!";
            Menu::on_refresh_btn_clicked();
            QMessageBox::information(this, "Tip", "编辑成功！");
            GeneralWidget->close();
        }
        db.close();
        delete ui_edit;
        delete GeneralWidget;
    }

}

void Menu::edit_back_btn_clicked()  // 用户编辑界面的返回按钮
{
    qDebug() << "edit_back_btn_clicked";
    GeneralWidget->close();
    delete ui_edit;
    delete GeneralWidget;
}
