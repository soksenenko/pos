#ifndef INPUTARTICLENO_H
#define INPUTARTICLENO_H

#include <QDialog>

namespace Ui {
class InputArticleNo;
}

class InputArticleNo : public QDialog
{
    Q_OBJECT
    
public:
    explicit InputArticleNo(QWidget *parent = 0);
    ~InputArticleNo();
    QString getArticleNo();
private:
    Ui::InputArticleNo *ui;
};

#endif // INPUTARTICLENO_H
