#ifndef INPUTQUERYMONEY_H
#define INPUTQUERYMONEY_H

#include <QDialog>

namespace Ui {
    class InputQueryMoney;
}

class InputQueryMoney : public QDialog
{
    Q_OBJECT

public:
    explicit InputQueryMoney(QWidget *parent = 0);
    ~InputQueryMoney();
    double getValue();

private:
    Ui::InputQueryMoney *ui;
};

#endif // INPUTQUERYMONEY_H
