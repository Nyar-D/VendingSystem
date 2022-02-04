#ifndef TABLEMODEL
#define TABLEMODEL

#include <QMap>
#include <QVariant>
#include <QSqlQueryModel>
#include <QItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDebug>

class TableModel : public QSqlQueryModel
{
public:
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QMap<int,QPair<QVariant, Qt::CheckState> > check_state_map;
};

class user_BD : public QItemDelegate
{
    Q_OBJECT

public:
    explicit user_BD(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool virtual editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    typedef QMap<QModelIndex, QPair<QStyleOptionButton*,QStyleOptionButton*>* > collButtons;
    collButtons m_btns;
signals:
    void sig_editUser(const QModelIndex &);
    void sig_deleteUser(const QModelIndex &);


};

class good_BD : public user_BD
{
    Q_OBJECT

public:
    explicit good_BD(QObject *parent = 0);
    bool virtual editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void sig_editGood(const QModelIndex &);
    void sig_deleteGood(const QModelIndex &);

};










#endif // TABLEMODEL

