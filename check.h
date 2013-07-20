#ifndef CHECK_H
#define CHECK_H

#include <QSqlQuery>
#include <QSqlQueryModel>

#include "tablemodel.h"
#include "cashregbase.h"
#include "cashregiks483.h"

class Check : public QObject
{
    Q_OBJECT
public:
    explicit Check(QObject *parent = 0);
    int CreateNew(QString UserID);
    int InsertDetail(QString ArticleNo,QString UserID);
    bool PrintCheck(uchar PaymentType);
    bool IsEmpty();
    bool Close();
    void Clear();

    QSqlQueryModel* getHeaderModel();
    TableModel* getDetailModel();

    bool getRejectMode();
    CashRegBase *cri;

public slots:
    bool Refresh();
    void setRejectMode(bool _RejectMode);

signals:
    void RejectModeChanged(bool _RejectMode);
    void detailChanged(const int ItemNo);


private:
    int CheckId;
    bool RejectMode;
    QSqlQuery HeaderQuery;
    QSqlQuery DetailQuery;
    QSqlQueryModel* CheckHeaderModel;
    TableModel* CheckDetailModel;
    bool UpdateHeader();

};

#endif // CHECK_H
