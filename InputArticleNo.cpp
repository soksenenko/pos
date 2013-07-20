#include "InputArticleNo.h"
#include "ui_InputArticleNo.h"

InputArticleNo::InputArticleNo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputArticleNo)
{
    ui->setupUi(this);
}

InputArticleNo::~InputArticleNo()
{
    delete ui;
}

QString InputArticleNo::getArticleNo()
{
    return ui->leArticleNo->text();
}
