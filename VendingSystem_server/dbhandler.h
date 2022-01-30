#ifndef DBHANDLER_H
#define DBHANDLER_H


#include <QDebug>
#include <QtSql>
#include <QFileInfo>
#include <QMessageBox>
#include "usertable.h"

#define DBFILE "./UserDatabase.db"
#define DBNAME "UserDatabase.db"
#define DBCONNECTION "dbUser"

class DBHandler : public QObject
{
    Q_OBJECT

public:
    DBHandler();
    ~DBHandler();

signals:
    void ShowAdvTable(QSortFilterProxyModel *);

private slots:
    void handle_getAdvTable();

protected:
    void PrepareAdvTable();
    void RefreshAdvTable();

    QSqlDatabase db;
    TableModel* model;
    QSortFilterProxyModel *sqlproxy;
};

#endif // DBHANDLER_H
