#ifndef CHOSETYPEPAYMENT_H
#define CHOSETYPEPAYMENT_H

#include <QDialog>

namespace Ui {
class ChoseTypePayment;
}

class ChoseTypePayment : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChoseTypePayment(double Summa,QWidget *parent = 0);
    ~ChoseTypePayment();
    unsigned char PaymentType;

private:
    Ui::ChoseTypePayment *ui;
    double SummaToPay,SummaPay;
private slots:
    void setSummaCharge(QString Summa);
    void enableOkButton();
    void on_comboBox_currentIndexChanged(int index);
};

#endif // CHOSETYPEPAYMENT_H
