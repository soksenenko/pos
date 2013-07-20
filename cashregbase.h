#ifndef CASHREGBase_H
#define CASHREGBase_H

#include <QDebug>

class CashRegBase
{
public:
    CashRegBase()
    {
        //qDebug() << "CashRegBase::CashRegBase()";
    }
    virtual ~CashRegBase()
    {
        //qDebug() << "CashRegBase::~CashRegBase()";
    }

    virtual int Connect(int portNum) = 0;
    virtual void Disconnect() = 0;
    virtual int SetCashier(uchar cashierNum, QString name, ushort password, uchar prog_name) = 0;
    virtual int Sale(int amount, uchar status, int price, uchar naloggroup, QString goods, ulong art) = 0;
    virtual int ArtXReport(ushort password) = 0;
    virtual int DayClrReport(ushort password) = 0;
    virtual int Avans(ulong avans) = 0;
    virtual int Give(ulong summa) = 0;
    virtual int Payment(uchar status, ulong payment, uchar checkclose, QString information) = 0;
    virtual int ResetOrder() = 0;
    virtual uchar myGetByteStatus() = 0;
    virtual int Discount(uchar type, unsigned int sum, uchar isdiscount, uchar * comment)=0;
    virtual QString ErrorMessage() = 0;

    bool isConnected() const { return m_isConnected; }


protected:
    bool m_isConnected;
};

#endif // CASHREGBase_H
