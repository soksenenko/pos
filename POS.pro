#-------------------------------------------------
#
# Project created by QtCreator 2011-07-03T13:48:28
#
#-------------------------------------------------

QT       += core gui
QT       += sql
TARGET = POS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    check.cpp \
    cashregiks483.cpp \
    tablemodel.cpp \
    scannerwrapper.cpp \
    parametersdialog.cpp \
    inputquerymoney.cpp \
    customdelegate.cpp \
    inputbarcode.cpp \
    LoginScreen.cpp \
    qdatawidgetmapperex.cpp \
    chosetypepayment.cpp \
    FindarticeDialog.cpp \
    InputArticleNo.cpp

HEADERS  += mainwindow.h \
    check.h \
    cashregiks483.h \
    cashregbase.h \
    cashregfabric.h \
    tablemodel.h \
    scannerwrapper.h \
    parametersdialog.h \
    inputquerymoney.h \
    customdelegate.h \
    inputbarcode.h \
    LoginScreen.h \
    qdatawidgetmapperex.h \
    chosetypepayment.h \
    FindArticeDialog.h \
    InputArticleNo.h

FORMS    += mainwindow.ui \
    parametersdialog.ui \
    inputquerymoney.ui \
    inputbarcode.ui \
    LoginScreen.ui \
    chosetypepayment.ui \
    FindArticeDialog.ui \
    InputArticleNo.ui

INCLUDEPATH += $$PWD/3rdparty/qserialdevice/ \
    $$PWD/3rdparty/libiks483/

LIBS += -L$$PWD/3rdparty/qserialdevice/ -lqserialdevice -liks483
