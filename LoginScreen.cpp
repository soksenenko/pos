#include <QtGui>
#include <QtSql>
#include <QMessageBox>

#include "LoginScreen.h"
#include "ui_LoginScreen.h"

LoginScreen::LoginScreen(QString UserID,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginScreen)
{
    this->UserID=UserID;
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);
    ui->lbWrongPassword->setVisible(false);

    if(this->UserID=="")
    {
        ui->lbLock->setText(tr("Вход в систему"));
    }
    else
    {
        ui->lbLock->setText(tr("Система заблокировна"));
        ui->btnCancel->setVisible(false);
    }

}

LoginScreen::~LoginScreen()
{
    delete ui;
}

QString LoginScreen::getUserID()
{
    return UserID;
}

bool LoginScreen::checkPassword(QString password)
{
    //QMessageBox::information(this,"User",password);
    QSqlQuery quUsers;
    quUsers.prepare("select UserName,AccessLevel from Users where UserID="":UserID""");
    quUsers.bindValue(":UserID",password);
    quUsers.exec();
    if(quUsers.lastError().isValid())
    {
        QMessageBox::critical(this,tr("Ошибка"),quUsers.lastError().text());
        return false;
    }
    else
    {
        if(quUsers.size()==1)
        {
            UserID = password;
            return true;
        }
        else
        {
            ui->lbWrongPassword->setVisible(true);
            return false;
        }
    }
}

void LoginScreen::on_btnOk_clicked()
{
    if(checkPassword(ui->lePassword->text()))
    {
        emit accept();
    }        
}

void LoginScreen::closeEvent(QCloseEvent *event)
{
    if(UserID!="")
    {
        event->ignore();
    }
}

void LoginScreen::keyPressEvent(QKeyEvent *event)
{
    if((event->key() == Qt::Key_Escape) && (!UserID.isEmpty()) )
    {
        event->ignore();
    }
    else
    {
        QDialog::keyPressEvent(event);
    }

}

void LoginScreen::getPasswordFromBarCode(QString password)
{
    if(checkPassword(password))
    {
        emit accept();
    }
}


void LoginScreen::on_lePassword_textChanged(const QString)
{
    if(ui->lbWrongPassword->isVisible())
    {
        ui->lbWrongPassword->setVisible(false);
    }
}
