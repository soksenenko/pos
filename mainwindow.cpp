#include <QtSql>
#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "check.h"
#include "tablemodel.h"
#include "parametersdialog.h"
#include "customdelegate.h"

#include "inputquerymoney.h"
#include "inputbarcode.h"
#include "LoginScreen.h"
#include "chosetypepayment.h"
#include "FindArticeDialog.h"
#include "InputArticleNo.h"

class QSqlQuery;
class QSqlQueryModel;
class QSqlRecord;
class QSqlField;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusLabel = new QLabel("", ui->statusBar);
    ui->statusBar->addWidget(statusLabel);

    userName = new QLabel("Незарегестрирован", ui->statusBar);
    ui->statusBar->addWidget(userName);

    settings = new QSettings("conf.ini", QSettings::IniFormat, this);
    int scannerPort = settings->value("ScannerPort").toInt();

    sw = new ScannerWrapper(scannerPort);

    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("pos");
    m_db.setUserName("pos");
    m_db.setPassword("basurman");
    if(!m_db.open())
        QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка подключения к БД :") + m_db.lastError().text());

    CurrentCheck = new Check();
    ui->tableView->setModel(CurrentCheck->getDetailModel());

    //sm = new QItemSelectionModel(ui->tableView->model());
    //ui->tableView->setSelectionModel(sm);
    ui->tableView->setItemDelegateForColumn(1, new CustomDelegate(ui->tableView));

    ui->tableView->setColumnWidth(0,400);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->hideColumn(4);//ItemNo
    ui->tableView->hideColumn(5);//CheckId
    ui->tableView->hideColumn(3);//ArticleNo
    ui->tableView->setFocus();

    mapper = new QDataWidgetMapperEx(this);
    mapper->setModel(CurrentCheck->getHeaderModel());
    mapper->addMapping(ui->leSumma, CurrentCheck->getHeaderModel()->record().indexOf("Summa"));
    mapper->addMapping(ui->leCheckNumber, CurrentCheck->getHeaderModel()->record().indexOf("CheckNumber"));

    /*************Labels on bottom****************/
    /*set color for labes on bottom*/
    QPalette bottomLabelPallete;
    bottomLabelPallete.setColor(QPalette::Window, Qt::white);
    bottomLabelPallete.setColor(QPalette::WindowText, Qt::blue);

    //ui->leArticleName->setAutoFillBackground(true);
    ui->leArticleName->setPalette(bottomLabelPallete);
    ui->leArticleName->setText("");

    ui->leArticleNo->setPalette(bottomLabelPallete);
    ui->leArticleNo->setText("");

    ui->lePrice->setPalette(bottomLabelPallete);
    ui->lePrice->setText("");

    ui->leQty->setPalette(bottomLabelPallete);
    ui->leQty->setText("");
    ui->leSummaRow->setPalette(bottomLabelPallete);
    ui->leSummaRow->setText("");

    /*map labels on bottopm panel to detail*/
    detailMapper = new QDataWidgetMapperEx(this);
    detailMapper->setModel(CurrentCheck->getDetailModel());
    //detailMapper->addMapping(ui->leArticleName,CurrentCheck->getDetailModel()->record().indexOf("ArticleName"),"text");
    detailMapper->addMapping(ui->leArticleNo,CurrentCheck->getDetailModel()->record().indexOf("ArticleNo"),"text");
    detailMapper->addMapping(ui->lePrice,CurrentCheck->getDetailModel()->record().indexOf("PriceOut"),"text");
    detailMapper->addMapping(ui->leQty,CurrentCheck->getDetailModel()->record().indexOf("Kol"),"text");
    detailMapper->addMapping(ui->leSummaRow,CurrentCheck->getDetailModel()->record().indexOf("Summa"),"text");
    /*connect mapper to model*/
    connect(CurrentCheck->getDetailModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            mapper, SLOT(setCurrentModelIndex(QModelIndex)));

    connect(sw, SIGNAL(codeRead(QString)), this, SLOT(codeReceived(QString)));

    connect(CurrentCheck, SIGNAL(detailChanged(const int)),this,SLOT(setSelectionByItemNo(const int)));

    connect(CurrentCheck, SIGNAL(detailChanged(const int)),
            this, SLOT(setCurrentIndexForHeader()));

    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            detailMapper, SLOT(setCurrentModelIndex(QModelIndex)));

    //изменение RejactMode вызывает установку Label-a
    connect(CurrentCheck,SIGNAL(RejectModeChanged(bool)),this,SLOT(setRejectModeLabel(bool)));
}

MainWindow::~MainWindow()
{
    delete sw;
    delete CurrentCheck;
    delete mapper;
    delete statusLabel;
    delete ui;
}

void MainWindow::codeReceived(QString code)
{

    if(code.startsWith("00"))//User
    {
        setUser(code);
        lock = false;
    }
    else
    {
        QString ArticelNo;
        double Kol=1;
        if((code.left(2))=="22")
        {
            ArticelNo =  code.mid(3,6);
            Kol =  code.mid(9,4).toDouble();
        }
        else
        {
            QSqlQuery quBarCode;
            quBarCode.prepare("select ArticleNo from BarCode where BarCode=:BarCode");
            quBarCode.bindValue(":BarCode",code);
            quBarCode.exec();
            if(quBarCode.lastError().isValid())
            {
                QMessageBox::critical(this,"Ошибка",quBarCode.lastError().text());
                return;
            }
            if(!quBarCode.first())
            {
                QMessageBox::information(0, tr(""),tr("Штрих-код не найден!"));
                return;
            }
            else
            {
                ArticelNo=quBarCode.record().field("ArticleNo").value().toString();
            }
        }

        int itemNo = CurrentCheck->InsertDetail(ArticelNo,this->CurrentUserID);

        if(itemNo>=0)
        {
            CurrentCheck->Refresh();
            setSelectionByItemNo(itemNo);
        }

    }
}

void MainWindow::setCurrentIndexForHeader()
{

    if(CurrentCheck->getDetailModel()->rowCount()==0)
        ;//mapper->setCurrentModelIndex(QModelIndex::QModelIndex ());
    else
        mapper->toFirst();
}

bool MainWindow::testCashRegister()
{
    if(!CurrentCheck->cri->isConnected())
    {
        if(int connectResult = CurrentCheck->cri->Connect(settings->value("CashregPort").toInt()) == -1)
            QMessageBox::critical(this, tr("Ошибка"),
                                  tr("Не удалось подключиться к кассовому аппарату!\nКод ошибки") + QString::number(connectResult));
    }
    return CurrentCheck->cri->isConnected();

}

void MainWindow::on_actionZ_triggered()
{
    if(testCashRegister())
    {
        int ZResult=CurrentCheck->cri->DayClrReport(0);
        if(ZResult < 0)
            QMessageBox::critical(this, tr("Ошибка"),
                                  tr("Ошибка Z-отчёта!\nКод ошибки") + QString::number(ZResult));
        CurrentCheck->cri->Disconnect();
    };
}

void MainWindow::on_actionX_triggered()
{    
    if(testCashRegister())
    {
        int XResult = CurrentCheck->cri->ArtXReport(0);
        if(XResult < 0)
            QMessageBox::critical(this, tr("Ошибка"),
                                  tr("Ошибка X-отчёта!\nКод ошибки") + QString::number(XResult));
        CurrentCheck->cri->Disconnect();
    };
}

void MainWindow::on_actionPorts_triggered()
{
    ParametersDialog d(this);

    d.setScannerPort(settings->value("ScannerPort").toInt());
    d.setCashregPort(settings->value("CashregPort").toInt());

    if (d.exec() == ParametersDialog::Accepted)
    {
        settings->setValue("ScannerPort", d.scannerPort());
        settings->setValue("CashregPort", d.cashregPort());
    }
}

void MainWindow::on_PrintCheck_triggered()
{
    ChoseTypePayment ctp(CurrentCheck->getHeaderModel()->record(0).field("Summa").value().toDouble());
    if(ctp.exec())
    {
        if(testCashRegister())
        {
            if(CurrentCheck->PrintCheck(ctp.PaymentType))
            {
                CurrentCheck->Close();
                //mapper->toFirst();
            }
        }
    }
}

void MainWindow::on_acInputBarCode_triggered()
{
    InputBarCode ibc(this);
    if (ibc.exec() == QDialog::Accepted)
        this->codeReceived(ibc.barCode());
}

void MainWindow::on_acRejectMode_triggered()
{

    CurrentCheck->setRejectMode(!CurrentCheck->getRejectMode());
    ui->acRejectMode->setChecked(CurrentCheck->getRejectMode());
}

void MainWindow::on_acInputMoney_triggered()
{
    InputQueryMoney inp(this);
    if(inp.exec() == QDialog::Accepted)
    {
        ulong a = static_cast<ulong>(floor(inp.getValue() * 100));
        //qDebug()<<a;
        if(testCashRegister())
            CurrentCheck->cri->Avans(a);
    }
}

void MainWindow::on_acGive_triggered()
{
    InputQueryMoney inp(this);
    if(inp.exec() == QDialog::Accepted)
    {
        ulong summa = static_cast<ulong>(floor(inp.getValue() * 100));
        qDebug()<<summa;
        if(testCashRegister())
            CurrentCheck->cri->Give(summa);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!CurrentCheck->IsEmpty())
    {
        if(CurrentUserRole==Cashier)
        {
            QMessageBox::information(this,tr("Закрытие"),tr("Чек не закрыт!"),QMessageBox::Ok);
            event->ignore();
        }
        else
            if(CurrentUserRole==Administrator)
            {
                int q = QMessageBox::question(this,tr("Подтвердите"),tr("Чек не пустой! Удалить чек и закрыть программу?"),QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel);
                if(q==QMessageBox::Yes)
                {
                    //CurrentCheck->DeleteAll();
                    event->accept();
                }
                else
                {
                    event->ignore();
                }
            };
    };

}

void MainWindow::setUser(QString UserID)
{
    CurrentUserID = UserID;
    QSqlQuery quUsers;
    quUsers.prepare("select UserName,AccessLevel from Users where UserID=:UserID");
    quUsers.bindValue(":UserID",UserID);
    quUsers.exec();
    if(quUsers.lastError().isValid())
    {
        QMessageBox::critical(this,tr("Ошибка"),quUsers.lastError().text());
    }
    else
    {
        quUsers.first();
        QSqlRecord rec = quUsers.record();//.field("UserName").value().toString();

        //userName->setText(quUsers.value(rec.indexOf("UserName")).toString());
        userName->setText(rec.field("UserName").value().toString());
        switch (rec.field("AccessLevel").value().toInt())
        {
        case 1:
            this->CurrentUserRole = Administrator;
            break;
        case 2:
            this->CurrentUserRole = Cashier;
            break;
        default:
            this->CurrentUserRole = Undefined;
        };

        ui->actionZ->setEnabled(this->CurrentUserRole == Administrator);
        ui->actionPorts->setEnabled(this->CurrentUserRole == Administrator);
        ui->acGive->setEnabled(this->CurrentUserRole == Administrator);
        ui->acInputMoney->setEnabled(this->CurrentUserRole == Administrator);
        ui->acClearCheck->setEnabled(this->CurrentUserRole == Administrator);
    }
}

void MainWindow::setRejectModeLabel(bool RejectMode)
{
    if (RejectMode)
    {
        statusLabel->setText("Режим возврата");
    }
    else
    {
        statusLabel->setText("");
    };
}
bool MainWindow::showLoginScreen()
{
    LoginScreen ls(this->CurrentUserID);

    disconnect(sw, SIGNAL(codeRead(QString)), this, SLOT(codeReceived(QString)));

    connect(sw, SIGNAL(codeRead(QString)), &ls, SLOT(getPasswordFromBarCode(QString)));
    int lsExCode = ls.exec();
    disconnect(sw, SIGNAL(codeRead(QString)), &ls, SLOT(getPasswordFromBarCode(QString)));

    connect(sw, SIGNAL(codeRead(QString)), this, SLOT(codeReceived(QString)));

    if(lsExCode)
    {
        setUser(ls.getUserID());
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::on_acLogin_triggered()
{
    showLoginScreen();
}

void MainWindow::on_actionGetStatus_triggered()
{
    if(testCashRegister())
    {

        uchar c = CurrentCheck->cri->myGetByteStatus();
    }


}


void MainWindow::on_acClearCheck_triggered()
{
    CurrentCheck->Clear();
    if(testCashRegister())
    {
        CurrentCheck->cri->ResetOrder();
    }
}


void MainWindow::on_acFindArticle_triggered()
{
    FindArticeDialog fd;
    if(fd.exec())
    {
        int itemNo = CurrentCheck->InsertDetail(fd.getArticleNo(),this->CurrentUserID);

        if(itemNo>=0)
        {
            CurrentCheck->Refresh();
            setSelectionByItemNo(itemNo);
        }
    }
}

void MainWindow::on_acInputCode_triggered()
{
    InputArticleNo iac(this);

    if (iac.exec() == QDialog::Accepted)
    {
        QSqlQuery quArticle;
        quArticle.prepare("select BarCode from BarCode where ArticleNo=:ArticleNo limit 1");
        quArticle.bindValue("ArticleNo",iac.getArticleNo());
        if(!quArticle.exec())
        {
            qDebug()<<"exec error"<<quArticle.lastError().text();
        }
        quArticle.first();
        QString barCode = quArticle.record().field("BarCode").value().toString();
        codeReceived(barCode);
    }
}
void MainWindow::setSelectionByItemNo(const int ItemNo)
{
    for(int i=0; i<ui->tableView->model()->rowCount();++i)
    {
        if(ui->tableView->model()->index(i,5/*itemNo*/).data().toInt()==ItemNo)
        {
            QModelIndex newIndex = ui->tableView->model()->index(i, 1/*Qty*/);
            ui->tableView->setCurrentIndex(newIndex);
            ui->tableView->setFocus();
            break;
        }
    }
}
