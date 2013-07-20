#include "tablemodel.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QModelIndex>
#include <QSqlTableModel>

TableModel::TableModel(QObject *parent) :
    QSqlQueryModel(parent)
{
}

QVariant TableModel::data(const QModelIndex &item, int role) const
{
    QVariant d = QSqlQueryModel::data(item, role);
    QVariant d2 = QSqlQueryModel::data(item,Qt::DisplayRole);

    if (role == Qt::TextAlignmentRole)
    {

        if(d2.type()==QVariant::String)
        {
            return Qt::AlignLeft;
        }
        else
        {
            return Qt::AlignRight;
        }

    }

    if (role == Qt::DisplayRole && d.type() == QVariant::Double)
    {
        if(item.column()==1)//KOl
        {

            return QVariant(QString::number(d.toFloat(), 'f', 3));
        }
        else
        {
            return QVariant(QString::number(d.toFloat(), 'f', 2));
        }
    }
    return d;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column() != 1)
        return false;

    int checkId = data(QSqlQueryModel::index(index.row(), 4), Qt::DisplayRole).toInt();
    int itemNo = data(QSqlQueryModel::index(index.row(), 5), Qt::DisplayRole).toInt();

    bool ok = setAmount(checkId, itemNo, value.toDouble());

    emit dataChanged(index, index);

    return ok;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == 1)
        flags |= Qt::ItemIsEditable;
    return flags;
}

bool TableModel::setAmount(int checkId, int itemNo, const double newAmount)
{
    QSqlQuery q;
    q.prepare("update CheckDetail set kol=:Kol where CheckId=:CheckId and ItemNo=:ItemNo");
    q.bindValue(":Kol", newAmount);
    q.bindValue(":CheckId", checkId);
    q.bindValue(":ItemNo", itemNo);
    return q.exec();
}
