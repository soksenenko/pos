#ifndef SCANNERWRAPPER_H
#define SCANNERWRAPPER_H
#include "qserialdevice/abstractserial.h"

class ScannerWrapper : public QObject
{
    Q_OBJECT

public:
    explicit ScannerWrapper(int portNum);
    ~ScannerWrapper();

private:
    AbstractSerial *port;
    QByteArray buf;

signals:
    void codeRead(QString code);

private slots:
    void slotReadAll();
};

#endif // SCANNERWRAPPER_H
