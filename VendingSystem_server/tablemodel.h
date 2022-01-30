#ifndef TABLEMODEL
#define TABLEMODEL

#include <QMap>
#include <QVariant>
#include <QSqlQueryModel>


class TableModel : public QSqlQueryModel
{
public:
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QMap<int,QPair<QVariant, Qt::CheckState> > check_state_map;
};

#endif // TABLEMODEL

