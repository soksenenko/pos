#ifndef CASHREGIKS483_H
#define CASHREGIKS483_H

#include "cashregbase.h"

class CashRegIKS483 : public CashRegBase
{
public:
    CashRegIKS483();
    ~CashRegIKS483();

    int Connect(int portNum);
    void Disconnect();
    int SetCashier(uchar cashierNum, QString name, ushort password, uchar prog_name);
    int Sale(int amount, uchar status, int price, uchar naloggroup, QString goods, ulong art);
    int ArtXReport(ushort password);
    int DayClrReport(ushort password);
    int Avans(ulong avans);
    int Give(ulong summa);
    int Payment(uchar status, ulong payment, uchar checkclose, QString information);
    int ResetOrder();
    uchar myGetByteStatus() ;
    //устанавливает скидку
    int Discount(uchar type, unsigned int sum, uchar isdiscount, uchar * comment);

    QString ErrorMessage();


};

#endif // CASHREGIKS483_H
