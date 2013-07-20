#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QSqlQueryModel>
#include <QSqlQuery>

class TableModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = 0);

    QVariant data(const QModelIndex &item, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    bool setAmount(int checkId, int itemNo, const double amount);

signals:
    void needRefresh();
};

#endif // TABLEMODEL_H
