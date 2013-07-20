#include "scannerwrapper.h"

ScannerWrapper::ScannerWrapper(int portNum)
{
    port = new AbstractSerial();

    port->setDeviceName("/dev/ttyS" + QString::number(portNum));

    if (port->open(AbstractSerial::ReadOnly))
       qDebug() << "Port opened on " << port->deviceName();
    else
       qDebug() << port->errorString();

    port->setBaudRate(AbstractSerial::BaudRate115200);
    port->setParity(AbstractSerial::ParityNone);
    port->setDataBits(AbstractSerial::DataBits8);
    port->setStopBits(AbstractSerial::StopBits1);

    connect(port, SIGNAL(readyRead()), this, SLOT(slotReadAll()));
}

ScannerWrapper::~ScannerWrapper()
{
    port->close();
    delete port;
}

void ScannerWrapper::slotReadAll()
{
    buf += port->readAll();

//    int in = buf.indexOf("\r");
    int nInd = buf.indexOf("\n");

//    if ((in != -1) && (nInd != -1))
    if (nInd != -1)

    {
        buf.resize(nInd-1);
        emit codeRead(QString(buf));

        buf.clear();
        port->reset();
    }
}
