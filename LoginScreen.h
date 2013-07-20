#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QDialog>

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QDialog
{
    Q_OBJECT
    
protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

public:
    explicit LoginScreen(QString UserID, QWidget *parent = 0);
    ~LoginScreen();
    QString getUserID();
    bool checkPassword(QString password);

private slots:

    void on_btnOk_clicked();
    void getPasswordFromBarCode(QString password);


    void on_lePassword_textChanged(const QString);

private:
    Ui::LoginScreen *ui;
    QString UserID;


};

#endif // LOGINSCREEN_H
