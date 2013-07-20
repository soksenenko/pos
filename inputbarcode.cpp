#include "inputbarcode.h"
#include "ui_inputbarcode.h"

InputBarCode::InputBarCode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputBarCode)
{
    ui->setupUi(this);
    ui->barCode->setFocus();
}

InputBarCode::~InputBarCode()
{
    delete ui;
}

QString InputBarCode::barCode() const
{
    return ui->barCode->text();
}
