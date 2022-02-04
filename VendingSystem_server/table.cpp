#include "table.h"

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    if(role == Qt::CheckStateRole && index.column() == 0)   // 负责勾选框点击时的操作
    {
        // 哈希表的QPair第一项存放ID,第二项存放勾选状态
        check_state_map[index.row()].first = index.data(Qt::DisplayRole);
        check_state_map[index.row()].second = (value == Qt::Checked ? Qt::Checked : Qt::Unchecked);
    }
    return true;
}

// 通过TableModel类data方法可以获得表格中存放的数据,使用时传入QModelIndex下表即可
// 当时不会写,未处理完善,用了QPair,感觉不是很完美,但是能用
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0) // 用户勾选第一列的勾选框时
        {
            if(check_state_map.contains(index.row())) // 如果哈希表中存放了下表的行,即键值
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
    if(index.column() == 0) // 设置表格属性的,可以是否启用,是否勾选框可勾选,是否可选择,是否可编辑
        return Qt::ItemIsEnabled |  Qt::ItemIsUserCheckable;
    return Qt::ItemIsEnabled;
}


user_BD::user_BD(QObject *parent):
    QItemDelegate(parent)
{

}



void user_BD::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // 哈希表中存放的按钮对
    QPair<QStyleOptionButton*, QStyleOptionButton*>* buttons = m_btns.value(index);
    // 如果哈希表中没有按钮,就new一对
    if(!buttons)
    {
        QStyleOptionButton* button1 = new QStyleOptionButton();
        button1->text = "编辑";
        button1->state |= QStyle::State_Enabled;

        QStyleOptionButton* button2 = new QStyleOptionButton();
        button2->text = "删除";
        button2->state |= QStyle::State_Enabled;

        buttons = new QPair<QStyleOptionButton*, QStyleOptionButton*>(button1, button2);
        (const_cast<user_BD *>(this))->m_btns.insert(index, buttons);// 不知道
    }
    // 绘制内嵌按钮的形状位置和动画
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

// 控制按钮的鼠标点击事件
bool user_BD::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* e = (QMouseEvent *)event;
        // 传入鼠标点击的下标与点击的范围
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
        // 鼠标点击的事件
        if(m_btns.contains(index))
        {
            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
            if(btns->first->rect.contains(e->x(), e->y()))
            {
                btns->first->state &= (~QStyle::State_Sunken);
                // 触发信号给编辑用户
                emit sig_editUser(index);
            }
            else if(btns->second->rect.contains(e->x(), e->y()))
            {
                btns->second->state &= (~QStyle::State_Sunken);
                // 触发信号给删除用户
                emit sig_deleteUser(index);
            }
        }
    }
}


good_BD::good_BD(QObject *parent):
    user_BD(parent)
{

}


bool good_BD::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* e = (QMouseEvent *)event;
        // 传入鼠标点击的下标与点击的范围
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
        // 鼠标点击的事件
        if(m_btns.contains(index))
        {
            QPair<QStyleOptionButton*, QStyleOptionButton*>* btns = m_btns.value(index);
            if(btns->first->rect.contains(e->x(), e->y()))
            {
                btns->first->state &= (~QStyle::State_Sunken);
                // 触发信号给编辑用户
                emit sig_editGood(index);
            }
            else if(btns->second->rect.contains(e->x(), e->y()))
            {
                btns->second->state &= (~QStyle::State_Sunken);
                // 触发信号给删除用户
                emit sig_deleteGood(index);
            }
        }
    }
}




