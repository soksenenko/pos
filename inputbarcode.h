#ifndef INPUTBARCODE_H
#define INPUTBARCODE_H

#include <QDialog>

namespace Ui {
    class InputBarCode;
}

class InputBarCode : public QDialog
{
    Q_OBJECT

public:
    explicit InputBarCode(QWidget *parent = 0);
    ~InputBarCode();

    QString barCode() const;

private:
    Ui::InputBarCode *ui;

};

#endif // INPUTBARCODE_H
