#ifndef CASHREGFABRIC_H
#define CASHREGFABRIC_H

#include "cashregbase.h"
#include "cashregiks483.h"

class CashRegFabric
{
public:
    static CashRegBase* CreateNew(QString device)
    {
        device = device.toLower();

        if (device == "iks483")
            return new CashRegIKS483();

        return NULL;
    }
};


#endif // CASHREGFABRIC_H
