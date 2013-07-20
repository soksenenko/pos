#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>
#include <QSettings>
#include <QtGui>
#include "scannerwrapper.h"
#include "check.h"
#include "qdatawidgetmapperex.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void codeReceived(QString code);

    bool testCashRegister();

    void on_actionZ_triggered();

    void on_actionX_triggered();

    void on_actionPorts_triggered();

    void on_PrintCheck_triggered();

    void on_acInputBarCode_triggered();

    void on_acRejectMode_triggered();

    void on_acInputMoney_triggered();

    void on_acGive_triggered();

    void on_acLogin_triggered();

    void on_actionGetStatus_triggered();


    void on_acClearCheck_triggered();

    void setCurrentIndexForHeader();


    void on_acFindArticle_triggered();

    void on_acInputCode_triggered();

private:
    Ui::MainWindow *ui;
    Check *CurrentCheck;
    QSqlDatabase m_db;
    QDataWidgetMapperEx *mapper;
    QDataWidgetMapperEx *detailMapper;
    QSettings *settings;
    bool lock;
    QLabel *statusLabel, *userName;
    enum UserRole {Undefined = -1,Administrator = 1, Cashier = 2 };
    UserRole CurrentUserRole;
    QString CurrentUserID;

public:
    ScannerWrapper *sw;
    bool showLoginScreen();

public slots:
    void setRejectModeLabel(bool RejectMode);
    void setUser(QString UserID);
    void setSelectionByItemNo(const int ItemNo);

 protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
