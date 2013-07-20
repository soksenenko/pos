#ifndef PARAMETERSDIALOG_H
#define PARAMETERSDIALOG_H

#include <QDialog>

namespace Ui {
    class ParametersDialog;
}

class ParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersDialog(QWidget *parent = 0);
    ~ParametersDialog();

    int scannerPort();
    int cashregPort();

    void setScannerPort(int value);
    void setCashregPort(int value);

private:
    Ui::ParametersDialog *ui;
};

#endif // PARAMETERSDIALOG_H
