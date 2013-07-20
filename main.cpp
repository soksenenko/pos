#include <QtGui/QApplication>
#include <QTextCodec>
#include <QDebug>
#include "mainwindow.h"

#include "cashregiks483.h"

int main(int argc, char *argv[])
{
    //русский
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    //
    QApplication a(argc, argv);
    MainWindow w;
    if(w.showLoginScreen())
    {
        w.showMaximized();
        return a.exec();
    }
    return 1;

}
