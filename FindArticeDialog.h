#ifndef FINDARTICEDIALOG_H
#define FINDARTICEDIALOG_H

#include <QtGui>
#include <QtSql>


namespace Ui {
    class FindArticeDialog;
}

class FindArticeDialog : public QDialog
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *, QEvent *);
public:
    explicit FindArticeDialog(QWidget *parent = 0);
    ~FindArticeDialog();
    QString getArticleNo();

private:
    Ui::FindArticeDialog *ui;
    QSqlDatabase db;
    QSqlQuery* quArticle;
    QSqlQueryModel* moArticle;
    QSignalMapper* signalMapper;
    QGridLayout *glKeyboard;
    QDataWidgetMapper *mapper;
    void CreateVirtualKeyboard();

private slots:
    void StringToFindChanged(QString);
    void VirtualKeyPressed(const QString);
};

#endif // FINDARTICEDIALOG_H
