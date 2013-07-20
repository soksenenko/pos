#include "inputquerymoney.h"
#include "ui_inputquerymoney.h"

InputQueryMoney::InputQueryMoney(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputQueryMoney)
{
    ui->setupUi(this);
    ui->spValue->selectAll();
    ui->spValue->setFocus();
}

InputQueryMoney::~InputQueryMoney()
{
    delete ui;
}
double InputQueryMoney::getValue()
{
    return ui->spValue->value();
}
