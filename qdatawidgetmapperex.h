#ifndef QDATAWIDGETMAPPEREX_H
#define QDATAWIDGETMAPPEREX_H

#include <QDataWidgetMapper>

class QDataWidgetMapperEx : public QDataWidgetMapper
{
    Q_OBJECT
public:
    explicit QDataWidgetMapperEx(QObject *parent = 0);
    
signals:
    
public slots:
    void setCurrentModelIndex(const QModelIndex &index);
    
};

#endif // QDATAWIDGETMAPPEREX_H
