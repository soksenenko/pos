#include <QtGui>

#include "FindArticeDialog.h"
#include "ui_FindArticeDialog.h"


FindArticeDialog::FindArticeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindArticeDialog)
{
    ui->setupUi(this);

    db = QSqlDatabase::database("CheckGenerator");


    quArticle = new QSqlQuery(db);
    if(!quArticle->exec("select ArticleName,ArticleNo "
                        "from Article "
                        "/*where exists(select * from BarCode  where BarCode.ArticleNo=Article.ArticleNo) */"
                        "order by ArticleName"))
    {
        qDebug()<<quArticle->lastError().text();
    }
    moArticle = new QSqlQueryModel();
    moArticle->setQuery(*quArticle);
    moArticle->setHeaderData(0, Qt::Horizontal, tr("Наименование"));
    moArticle->setHeaderData(1, Qt::Horizontal, tr("Код"));



    ui->tvArticle->setModel(moArticle);
    ui->tvArticle->resizeColumnsToContents();
    ui->tvArticle->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvArticle->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvArticle->setTabKeyNavigation(false);

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(moArticle);
    //mapper->addMapping(ui->lineEdit, 0);
    connect(ui->tvArticle->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            mapper, SLOT(setCurrentModelIndex(QModelIndex)));
   // mapper->setCurrentModelIndex(ui->tvArticle->selectionModel()->currentIndex());



    /*centered*/
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
    setWindowState(this->windowState()^Qt::WindowMaximized);
    setWindowFlags(Qt::WindowMaximizeButtonHint);
    /*hooking kew down*/
    ui->leSearchString->installEventFilter(this);
    connect(ui->leSearchString,SIGNAL(textChanged(QString)),this,SLOT(StringToFindChanged(QString)));


    ui->groupBox_2->setTitle("");

    CreateVirtualKeyboard();
    ui->pbOK->setFocusPolicy(Qt::NoFocus);
    ui->pbCancel->setFocusPolicy(Qt::NoFocus);
    ui->leSearchString->setFocusPolicy(Qt::NoFocus);



}

FindArticeDialog::~FindArticeDialog()
{
    delete quArticle;
    delete moArticle;
    delete ui;
}

bool FindArticeDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj->objectName()=="leSearchString")
        {
            if(keyEvent->key()==Qt::Key_Down)
            {
                ui->tvArticle->setFocus();
                return true;
            }
        }
        else
            if(obj->objectName().contains("QPushButton"))
            {
                int row,col;
                col = obj->property("col").toInt();
                row = obj->property("row").toInt();
                if(keyEvent->key()==Qt::Key_Down)
                {
                    row++;
                    QPushButton *button = ui->groupBox_2->findChild<QPushButton *>("QPushButton_"+QString::number(row)+"_"+QString::number(col));
                    if(button)
                    {
                        button->setFocus();
                        return true;
                    }
                };
                if(keyEvent->key()==Qt::Key_Up)
                {
                    row--;
                    QPushButton *button = ui->groupBox_2->findChild<QPushButton *>("QPushButton_"+QString::number(row)+"_"+QString::number(col));
                    if(button)
                    {
                        button->setFocus();
                        return true;
                    }
                }
                else
                    if(keyEvent->key()==Qt::Key_Tab)
                    {
                        ui->tvArticle->setFocus();
                        return true;
                    }
                    else
                        return false;
            }
            else
            {
                return false;
            }
    }
    return false;
}

void FindArticeDialog::StringToFindChanged(QString S)
{
    if(S.length()>0)
        quArticle->exec("select ArticleName,ArticleNo from Article where ArticleName like '%"+S+"%'");
    else
        quArticle->exec("select ArticleName,ArticleNo from Article order by ArticleName");
    if(quArticle->lastError().isValid())
    {
        qDebug()<<quArticle->lastError().text();
    }
    else
    {
        moArticle->setQuery(*quArticle);
    }
    QModelIndex newIndex = ui->tvArticle->model()->index(0,0);
    ui->tvArticle->setCurrentIndex(newIndex);
}

QString FindArticeDialog::getArticleNo()
{
    QString ArticleNo="";
    if(ui->tvArticle->currentIndex().isValid())
    {
        QModelIndex curRow = ui->tvArticle->model()->index(ui->tvArticle->currentIndex().row(),1/*ArticleNo*/);
        ArticleNo= moArticle->data(curRow).toString();
    }
    return ArticleNo;
}

void FindArticeDialog::VirtualKeyPressed(const QString S)
{
    if(S=="<-")
    {
        if(ui->leSearchString->text().length()>0)
        {
            QString sTmp = ui->leSearchString->text();
            sTmp.resize(sTmp.length()-1);

            ui->leSearchString->setText(sTmp);
        }
    }
    else
        if(S=="Пробел")
        {
            ui->leSearchString->setText(ui->leSearchString->text()+" ");
        }
        else
            if(S=="Очистить")
            {
                ui->leSearchString->setText("");
            }
            else
            {
                ui->leSearchString->setText(ui->leSearchString->text()+S);
            }
}

void FindArticeDialog::CreateVirtualKeyboard()
{
    glKeyboard = new QGridLayout();
    signalMapper = new QSignalMapper(this);
    QStringList slButtonText;
    slButtonText <<"А"<<"Б"<<"В"<<"Г"<<"Д"<<"Е"<<"Ж"<<"З"<<"И"<<"Й"<<"К"<<"Л"<<"М"<<"Н"<<"О";
    slButtonText <<"П"<<"Р"<<"С"<<"Т"<<"У"<<"Ф"<<"Х"<<"Ц"<<"Ч"<<"Ш"<<"Щ"<<"Ъ"<<"Ы"<<"Ь"<<"Э"<<"Ю"<<"Я";

    slButtonText<<"Пробел"<<"<-"<<"Очистить";

    int row, col;
    QWidget *prevTabOrder = ui->tvArticle;
    for (int i= 0;i<slButtonText.size();i++)
    {
        QPushButton *b = new QPushButton(this);
        b->setText(slButtonText.at(i));
        col=i%10; row=i/10;
        b->setProperty("row",row);
        b->setProperty("col",col);
        b->setObjectName("QPushButton_"+QString::number(row)+"_"+QString::number(col));
        b->installEventFilter(this);
        setTabOrder(prevTabOrder,b);
        prevTabOrder = b;
        connect(b, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(b, b->text());
        glKeyboard->addWidget(b,row,col);
        if(b->text()=="Очистить")
        {
            b->setShortcut(QKeySequence("Del"));
        }
        else
            if(b->text()=="<-")
            {
                b->setShortcut(QKeySequence("Backspace"));
            }
            else
            {
                b->setShortcut(QKeySequence(b->text()));
            }

    }

    connect(signalMapper, SIGNAL(mapped(const QString &)),
                 this, SLOT(VirtualKeyPressed(const QString &)));

    ui->groupBox_2->setLayout(glKeyboard);

    QPushButton* FirstButton = ui->groupBox_2->findChild<QPushButton *>("QPushButton_0_0");
    if (FirstButton)
    {
        FirstButton->setFocus();
    }

}

