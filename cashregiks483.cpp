#ifdef __cplusplus
extern "C"
{
    #include "libiks483.h"
}
#endif

#include "cashregiks483.h"
#include <QTextCodec>

CashRegIKS483::CashRegIKS483()
{
    m_isConnected = false;
    //qDebug() << "CashRegIKS483::CashRegIKS483()";
}

CashRegIKS483::~CashRegIKS483()
{
    Disconnect();
    //qDebug() << "CashRegIKS483::~CashRegIKS483";
}

int CashRegIKS483::Connect(int portNum)
{
    //qDebug()<<"CashRegIKS483::Connect. Port="<<portNum;
    uchar *port = (uchar *)QString("/dev/ttyS" + QString::number(portNum)).toAscii().data();
    int fd = FPInit(port, PORTSPEED, TIMEOUT);
    //qDebug()<<"CashRegIKS483::Connect. fd="<<fd;
    if (fd!=-1)
    {
        m_isConnected = true;
        return fd;
    }
    else
        return -1;
    delete port;
}

void CashRegIKS483::Disconnect()
{
    FPClose();
    m_isConnected = false;
}

int CashRegIKS483::SetCashier(uchar cashierNum, QString name, ushort password, uchar prog_name)
{
    return FPSetCashier(cashierNum, (uchar *)name.toAscii().data(), password, prog_name);
}

int CashRegIKS483::Sale(int amount, uchar status, int price, uchar naloggroup, QString goods, ulong art)
{
    QTextCodec *codec = QTextCodec::codecForName("IBM 866");
    goods.replace("№","N");

    QByteArray encodedString = codec->fromUnicode(goods);
    //qDebug()<<"amount"<<amount<<"price"<<price<<"NGroup"<<naloggroup<<"art"<<art;
    return FPSale(amount, status, price, naloggroup,(uchar *)encodedString.data(), art);
}

int CashRegIKS483::ArtXReport(ushort password)
{
    return FPDayReport(password);
}

int CashRegIKS483::DayClrReport(ushort password)
{
    return FPDayClrReport(password);
}

int CashRegIKS483::Avans(ulong avans)
{
    return FPAvans(avans);    
}

int CashRegIKS483::Give(ulong summa)
{
    return FPGive(summa);
}

int CashRegIKS483::Payment(uchar status, ulong payment, uchar checkclose, QString information)
{
    return FPPayment(status, payment, checkclose, (uchar *)information.data());
}

int CashRegIKS483::ResetOrder()
{
    return FPResetOrder();
}
uchar CashRegIKS483::myGetByteStatus()
{
    return GetByteStatus();
}

//устанавливает скидку
int CashRegIKS483::Discount(uchar type, unsigned int sum, uchar isdiscount, uchar * comment)
{
    return FPDiscount(type, sum, isdiscount, comment);
}

// возвращает сообщение об ошибке принтера
QString CashRegIKS483::ErrorMessage()
{
    const uchar * str=FPErrorMessage();
    return QString((char *)str);
}
