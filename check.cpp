#include <QtSql>
#include <QDebug>
#include <QMessageBox>
#include <QDataWidgetMapper>

#include "check.h"
#include "cashregiks483.h"

class QSqlRecord;
class QSqlError;
class QSqlQueryModel;

Check::Check(QObject *parent) :
    QObject(parent)
{
    cri = new CashRegIKS483();
    /*if (cri->Connect(portNum))
        qDebug() << "Connected to cash register";
    else
        qDebug() << "Cannot connect to cash register";
     */

    CheckId = 0;
    RejectMode = false;

    CheckHeaderModel = new QSqlQueryModel();
    CheckHeaderModel->setObjectName("CheckHeaderModel");

    HeaderQuery.prepare("select CheckNumber,Summa from CheckHeader where CheckId=:CheckID");
    HeaderQuery.bindValue("CheckID", CheckId);
    HeaderQuery.exec();
    CheckHeaderModel->setQuery(HeaderQuery);

    CheckDetailModel = new TableModel();
    CheckDetailModel->setObjectName("CheckDetailModel");
    DetailQuery.prepare("select article.ArticleName,checkdetail.Kol,checkdetail.PriceOut,article.ArticleNo,checkdetail.checkid,checkdetail.itemno,CheckDetail.Kol*CheckDetail.PriceOut as Summa "
                        "from checkdetail join article on article.ArticleNo=checkdetail.ArticleNo "
                        "where CheckId=:CheckId order by ItemNo");

    DetailQuery.bindValue(":CheckId",CheckId);
    DetailQuery.exec();

    CheckDetailModel->setQuery(DetailQuery);

    connect(CheckDetailModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(Refresh()));

    if(DetailQuery.lastError().isValid())
    {
        qDebug()<<DetailQuery.lastError();
    }
    CheckDetailModel->setHeaderData(0,Qt::Horizontal,tr("Наименование"));
    CheckDetailModel->setHeaderData(1,Qt::Horizontal,tr("Кол-во"));
    CheckDetailModel->setHeaderData(2,Qt::Horizontal,tr("Цена"));
    CheckDetailModel->setHeaderData(6,Qt::Horizontal,tr("Сумма"));
}

int Check::CreateNew(QString UserID)
{
    QSqlQuery quWork;
    //rewrite to procedure
    quWork.exec("select COALESCE(max(CheckId),0)+1 CheckId from CheckHeader");
    quWork.first();
    CheckId = quWork.value(0).toInt();
    //-----------------------------------------------------------
    if(quWork.lastError().isValid())
    {
      qDebug() << quWork.lastError();
    }
    quWork.prepare("INSERT INTO CheckHeader "
                   "(CheckId,CheckNumber,DateTimeClose,UserId,Status)"
                   "VALUES "
                   "(:CheckId,GetCheckNumber(),NOW(),:UserId, 0)");
    quWork.bindValue("CheckId",CheckId);
    quWork.bindValue("UserId",UserID);
    quWork.exec();
    if(quWork.lastError().isValid())
    {
      QMessageBox::critical(0,"Error","Ошибка создания чека:"+quWork.lastError().text());
    }
    setRejectMode(false);
    return CheckId;
}

QSqlQueryModel* Check::getHeaderModel()
{
    return CheckHeaderModel;
}

TableModel* Check::getDetailModel()
{
 return CheckDetailModel;
}

bool Check::Refresh()
{
    this->UpdateHeader();
    HeaderQuery.bindValue(":CheckID",CheckId);
    HeaderQuery.exec();

    CheckHeaderModel->setQuery(HeaderQuery);
    if(HeaderQuery.lastError().isValid())
        qDebug() << HeaderQuery.lastError();

    HeaderQuery.first();
    //qDebug()<<"refresh:"<<HeaderQuery.record();

    //qDebug()<<"refresh:"<<CheckId<<"^"<<DetailQuery.record().field("itemNo").value();
    int CurrentItemNo = DetailQuery.record().field("itemNo").value().toInt();
    DetailQuery.bindValue(":CheckId",CheckId);
    DetailQuery.exec();
    //DetailQuery.last();
    CheckDetailModel->setQuery(DetailQuery);
    if(DetailQuery.lastError().isValid())
        qDebug()<<DetailQuery.lastError();
    emit detailChanged(CurrentItemNo);

    return true;
}

int Check::InsertDetail(QString ArticleNo, QString UserID)
{
    if (CheckId == 0)
        CreateNew(UserID);

    /*вызов SaleBarCode*/
    QSqlQuery quInsertDetail, quResult;
    quInsertDetail.prepare("CALL SaleArticleNo(:_CheckID,:_ArticleNo,:_Kol,:_RejectMode,@result,@ItemNo)");
    quInsertDetail.bindValue(":_CheckId", CheckId);
    quInsertDetail.bindValue(":_ArticleNo", ArticleNo);
    quInsertDetail.bindValue(":_Kol", 1.0);
    quInsertDetail.bindValue(":_RejectMode", RejectMode);
    quInsertDetail.exec();

    if (quInsertDetail.lastError().isValid())
    {
        qDebug() << "quInsertDetail"<<quInsertDetail.lastError().text();
        return -1;
    }

    /*получение out параметров из предыдущего вызова*/
    quResult.exec("select @result,@ItemNo");
    quResult.first();

    switch (quResult.value(0).toInt())
    {
    case -1:
        QMessageBox::information(0, tr(""),tr("Штрих-код не найден!"));
        break;
    case -2:
        QMessageBox::information(0, tr(""),tr("Возврат не может превышать исходного количества!"));
        break;
    }
    if (quResult.value(0).toInt()>=0)
    {

        this->UpdateHeader();
        return quResult.value(1).toInt();
    }
    else
    {
        return quResult.value(0).toInt();
    }
}
bool Check::PrintCheck(uchar PaymentType/*Форма оплат/выплат (0 – карточкой,1 – в кредит, 2 – чеком, 3 – наличными)*/)
{
    if (this->IsEmpty())
    {
        cri->ResetOrder();
       // qDebug() << tr("Чек пуст!");
        return false;
    }
    else
    {
        QSqlQuery quCheckHeader;
        quCheckHeader.prepare("SELECT SummaDiscount FROM CheckHeader WHERE CheckId=:CheckId");
        quCheckHeader.bindValue(":CheckId",CheckId);
        quCheckHeader.exec();
        quCheckHeader.first();

        QSqlQuery quCheckDetail;
        QSqlQueryModel PrintCheckModel;
        quCheckDetail.prepare("select Article.ArticleName,SUM(checkdetail.Kol) Kol,checkdetail.PriceOut,Article.ArticleNo,MIN(ItemNo) ItemNo "
                              "from checkdetail join article on article.ArticleNo=checkdetail.ArticleNo "
                              "where CheckId=:CheckId "
                              "group by Article.ArticleNo,Article.ArticleName,Checkdetail.PriceOut "
                              "HAVING SUM(Kol)>0 "
                              "order by MIN(ItemNo)");

        quCheckDetail.bindValue(":CheckId",CheckId);
        quCheckDetail.exec();
        PrintCheckModel.setQuery(quCheckDetail);

        QSqlQuery quGetCashNo, quResult;
        quGetCashNo.prepare("call GetCashNo(:_ArticleNo,@ArticleName,@CashNo)");

        for (int i = 0; i < PrintCheckModel.rowCount(); ++i)
        {
           QSqlRecord r = PrintCheckModel.record(i);

           int kol = static_cast<int>(round(r.field("Kol").value().toDouble()*1000));

           double p = round(r.field("PriceOut").value().toDouble()* 100);
           int priceout = static_cast<int>(p);

           //QString articlename = r.field("ArticleName").value().toString();

           quGetCashNo.bindValue("_ArticleNo",r.field("ArticleNo").value());
           quGetCashNo.exec();

           quResult.exec("select @ArticleName,@CashNo");
           quResult.first();

           int CashNo = quResult.value(1).toInt();
           QString articlename = quResult.value(0).toString();
           //qDebug()<<"Kol"<<kol<<"KolD"<<r.field("Kol").value().toDouble()<<"KOlf"<<round(r.field("Kol").value().toDouble()*1000);

           //qDebug() << kol << p <<  priceout << articlename << CashNo;
           int SaleResult = cri->Sale(kol, 3, priceout, 0, articlename, CashNo);

           if(SaleResult<0)
           {
               cri->ResetOrder();
               //qDebug()<<"reset order GetByteStatus"<<cri->myGetByteStatus();
               cri->Disconnect();
               QMessageBox::critical(0,tr("Ошибка"),tr("Ошибка пробития\n Код ошибки ")+QString::number(SaleResult));
               return false;
           }
        };
        //скидка
        if(quCheckHeader.record().field("SummaDiscount").value().toDouble()>0)
        {

            QString comment = "";
            double d = floor(quCheckHeader.record().field("SummaDiscount").value().toDouble()*100);
            int SummaDiscount =  static_cast<int>(d);
            cri->Discount(3,d,1,(uchar *)comment.data());

        };
        //

       int PaymentResult = cri->Payment(PaymentType, 0, 1,"");
       if(PaymentResult<0)
       {
           cri->ResetOrder();
           cri->Disconnect();
           QMessageBox::critical(0,tr("Ошибка"),tr("Ошибка закрытия чека\n Код ошибки ")+QString::number(PaymentResult));
           return false;
       }

       cri->Disconnect();

       return true;
    }
}

bool Check::IsEmpty()
{
    if(CheckDetailModel->rowCount()==0)
        return true;
    else
        return false;
}

bool Check::UpdateHeader()
{
    QSqlQuery quWork;
    quWork.prepare("CALL UpdateCheckHeader(:_CheckID)");
    quWork.bindValue(":_CheckID",CheckId);
    quWork.exec();
    if (quWork.lastError().isValid())
    {
        QMessageBox::critical(0,"Ошибка",quWork.lastError().text());
    }
    return true;
}
bool Check::Close()
{
    QSqlQuery quWork;
    quWork.prepare("UPDATE CheckHeader SET Status=10 WHERE CheckId=:_CheckID");
    quWork.bindValue(":_CheckID",CheckId);
    quWork.exec();

    CheckId = 0;
    setRejectMode(false);
    this->Refresh();
    return true;
}
void Check::setRejectMode(bool _RejectMode)
{
    if(_RejectMode!=this->RejectMode)
    {
        this->RejectMode=_RejectMode;
        emit RejectModeChanged(this->RejectMode);
    };
}

bool Check::getRejectMode()
{
    return this->RejectMode;
}

void Check::Clear()
{
    QSqlQuery quWork;
    quWork.prepare("DELETE FROM CheckDetail WHERE CheckID=:CheckID");
    quWork.bindValue("CheckID",CheckId);
    quWork.exec();
    this->Refresh();
}
