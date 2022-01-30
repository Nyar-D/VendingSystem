#include "usertable.h"

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    if(role == Qt::CheckStateRole && index.column() == 0)
    {
        check_state_map[index.row()].second = (value == Qt::Checked ? Qt::Checked : Qt::Unchecked);
        check_state_map[index.row()].first = index.data(Qt::DisplayRole);
    }
    return true;
}


QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
        {
            if(check_state_map.contains(index.row()))
                return check_state_map[index.row()].second == Qt::Checked ? Qt::Checked : Qt::Unchecked;
            return Qt::Unchecked;
        }
    }
    return QSqlQueryModel::data(index, role);
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;
    if(index.column() == 0)
        return Qt::ItemIsEnabled |  Qt::ItemIsUserCheckable;
    return Qt::ItemIsEnabled;
}


ButtonDelegate::ButtonDelegate(QObject *parent):
    QItemDelegate(parent)
{

}


void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPair<QStyleOptionButton*, QStyleOptionButton*>* buttons = m_btns.value(index);
    if(!buttons)
    {
        QStyleOptionButton* button1 = new QStyleOptionButton();
        button1->text = "编辑";
        button1->state |= QStyle::State_Enabled;

        QStyleOptionButton* button2 = new QStyleOptionButton();
        button2->text = "删除";
        button2->state |= QStyle::State_Enabled;

        buttons = new QPair<QStyleOptionButton*, QStyleOptionButton*>(button1, button2);
        (const_cast<ButtonDelegate *>(this))->m_btns.insert(index, buttons);
    }

    buttons->first->rect = option.rect.adjusted(4, 4, -(option.rect.width() / 2 + 4), -4);
    buttons->second->rect = option.rect.adjusted(buttons->first->rect.width() + 12, 4, -4, -4);
    painter->save();
    if(option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
    }
    painter->restore();
    QApplication::style()->drawControl(QStyle::CE_PushButton, buttons->first, painter);
    QApplication::style()->drawControl(QStyle::CE_PushButton, buttons->second, painter);
}


bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* e = (QMouseEvent *)event;

        if(m_btns.contains(index))
        {
            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
            if(btns->first->rect.contains(e->x(), e->y()))
            {
                btns->first->state |= QStyle::State_Sunken;
            }
            else if(btns->second->rect.contains(e->x(), e->y()))
            {
                btns->second->state |= QStyle::State_Sunken;
            }
        }
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* e = (QMouseEvent *)event;

        if(m_btns.contains(index))
        {
            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
            if(btns->first->rect.contains(e->x(), e->y()))
            {
                btns->first->state &= (~QStyle::State_Sunken);
                editUser(index);
            }
            else if(btns->second->rect.contains(e->x(), e->y()))
            {
                btns->second->state &= (~QStyle::State_Sunken);
                deleteUser(index);
            }
        }
    }
}

void ButtonDelegate::editUser(const QModelIndex &index)
{
    qDebug() << "edit-index:" << index;
}

void ButtonDelegate::deleteUser(const QModelIndex &index)
{
    qDebug() << "delete-index:" << index << "ID:";
}



