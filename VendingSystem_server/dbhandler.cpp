#include "dbhandler.h"

DBHandler::DBHandler()
{

}

DBHandler::~DBHandler()
{

}

void DBHandler::handle_getAdvTable()
{
    qDebug()<<"DBHandleThread: handle database";
    PrepareAdvTable();
    RefreshAdvTable();
}

void DBHandler::PrepareAdvTable()
{
    QString dbFilePath(DBFILE);
    QFileInfo fileinfo(dbFilePath);
    if(QSqlDatabase::contains(DBCONNECTION))
    {
        qDebug() << "Connection: dbUser existed.";
        db = QSqlDatabase::database(DBCONNECTION);

        QSqlQuery query(db);

        if(query.exec("select * from sqlite_master where name='adv'") && query.record().isEmpty())
        {
            qDebug()<<"DBHandleThread: Adv table no existed";
            if(db.open())
            {
                QString create_adv_table = "create table adv(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, PlanName varchar(20) NOT NULL, Period date NOT NULL, Title varchar(20) NOT NULL, Description varchar(40), Type varchar(10) NOT NULL, AuditStatus varchar(3) NOT NULL, Enable INTEGER NOT NULL)";
                QSqlQuery sql_query = QSqlQuery(db);
                sql_query.prepare(create_adv_table);
                if(!sql_query.exec())
                {
                    qDebug() << QObject::tr("Adv table failed to create!");
                    qDebug() << sql_query.lastError();
                    QFile::remove(DBFILE);
                }
                else
                    qDebug() << "DBHandleThread: Adv table created!";
            }
            else
                QMessageBox::information(NULL, "Error", "Fail to open the db!", QMessageBox::Yes);
        }
        else
            qDebug()<<"DBHandleThread: adv table existed";
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", DBCONNECTION);
        db.setDatabaseName(DBNAME);
        qDebug() << "Connection: dbUser created success.";
        if(!fileinfo.isFile())
        {
            if(db.open())
            {
                QString create_sql_user = "create table adv(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, PlanName varchar(20) NOT NULL, Period date NOT NULL, Title varchar(20) NOT NULL, Description varchar(40), Type varchar(10) NOT NULL, AuditStatus varchar(3) NOT NULL, Enable INTEGER NOT NULL)";
                QSqlQuery sql_query = QSqlQuery(db);
                sql_query.prepare(create_sql_user);
                if(!sql_query.exec())
                {
                    qDebug() << QObject::tr("Advertisement table failed to create!");
                    qDebug() << sql_query.lastError();
                    QFile::remove(DBFILE);
                }
                else
                {
                    qDebug() << "Advertisement table created!";
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

void DBHandler::RefreshAdvTable()
{
    qDebug() << "DBHandleThread: adv_table refresh";

//    qDebug()<<"wait 5s";
//    QThread::sleep(5);
//    qDebug()<<"5s end";
    if(!db.open())
    {
        qDebug()<<"no open";
        QMessageBox::critical(NULL, "Error", "Fail to connect the SQLite!",
                              QMessageBox::Yes);
        return;
    }
    else
    {
        model = new TableModel();
        model->setQuery("select * from adv", db);
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("序号"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("计划名称"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("投放周期"));
        model->setHeaderData(3,Qt::Horizontal,QObject::tr("广告标题"));
        model->setHeaderData(4,Qt::Horizontal,QObject::tr("广告描述"));
        model->setHeaderData(5,Qt::Horizontal,QObject::tr("广告类型"));
        model->setHeaderData(5,Qt::Horizontal,QObject::tr("审核状态"));
        model->setHeaderData(6,Qt::Horizontal,QObject::tr("是否启用"));
        sqlproxy_adv = new QSortFilterProxyModel(this);
        sqlproxy_adv->setSourceModel(model);

        emit ShowAdvTable(sqlproxy_adv);
    }
}
