#include "qdatawidgetmapperex.h"
#include <QModelIndex>
#include <QDebug>
#include <QtGui>
#include <QList>

QDataWidgetMapperEx::QDataWidgetMapperEx(QObject *parent) :
    QDataWidgetMapper(parent)
{
}

void QDataWidgetMapperEx::setCurrentModelIndex(const QModelIndex &index)
{
    if(index.isValid())
    {
        QDataWidgetMapper::setCurrentModelIndex(index);
    }
    else
    {
        for(int i=0;i<this->model()->columnCount();++i)
        {
            QLabel* l= qobject_cast<QLabel *>(this->mappedWidgetAt(i));
            if(l)
            {
                l->clear();
            }
            else
            {
                QLineEdit* le= qobject_cast<QLineEdit *>(this->mappedWidgetAt(i));
                if(le)
                {
                    le->clear();
                }
            }
        }
    }
}
