#include "parametersdialog.h"
#include "ui_parametersdialog.h"


ParametersDialog::ParametersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametersDialog)
{
    ui->setupUi(this);
}

ParametersDialog::~ParametersDialog()
{
    delete ui;
}

int ParametersDialog::scannerPort()
{
    return ui->scannerPort->value();
}

int ParametersDialog::cashregPort()
{
    return ui->cashregPort->value();
}

void ParametersDialog::setScannerPort(int value)
{
    ui->scannerPort->setValue(value);
}

void ParametersDialog::setCashregPort(int value)
{
    ui->cashregPort->setValue(value);
}
