#include <QPushButton>
#include "chosetypepayment.h"
#include "ui_chosetypepayment.h"
#include <QDebug>

ChoseTypePayment::ChoseTypePayment(double Summa, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoseTypePayment)
{
    ui->setupUi(this);

    QPushButton* dlgBtn = ui->buttonBox->button(QDialogButtonBox::Ok);
    dlgBtn->setEnabled(false);

    dlgBtn = ui->buttonBox->button(QDialogButtonBox::Cancel);
    dlgBtn->setText(tr("Отмена"));

    SummaToPay = Summa;
    PaymentType = 3;


    connect(ui->leSumma,SIGNAL(textChanged(QString)),this,SLOT(setSummaCharge(QString)));

    ui->leSumma->setText(QString::number(SummaToPay,'f',2));

    ui->lblSumma->setText(QString::number(SummaToPay,'f',2));
    ui->leSumma->setValidator(new QDoubleValidator(0,9999.99,2,this));
    ui->leSumma->selectAll();
    ui->leSumma->setFocus();

}

ChoseTypePayment::~ChoseTypePayment()
{
    delete ui;
}

void ChoseTypePayment::setSummaCharge(QString Summa)
{
    SummaPay=Summa.toDouble();
    double SummaCharge = SummaPay-SummaToPay;

    ui->lblChange->setText(QString::number(SummaCharge,'f',2));
    QPushButton* dlgBtn = ui->buttonBox->button(QDialogButtonBox::Ok);
    dlgBtn->setEnabled(SummaPay>=SummaToPay);
}


void ChoseTypePayment::enableOkButton()
{
    QPushButton* dlgBtn = ui->buttonBox->button(QDialogButtonBox::Ok);
    dlgBtn->setEnabled(SummaPay>=SummaToPay);
}

void ChoseTypePayment::on_comboBox_currentIndexChanged(int index)
{
    /*Форма оплат/выплат (0 – карточкой,1 – в кредит, 2 – чеком, 3 – наличными)*/
    switch (index)
    {
    case 0 : {PaymentType = 3; break;}
    case 1 : {PaymentType = 0; break;}
    default  : PaymentType = 99;
    }
}
