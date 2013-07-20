//#include <cfloat>

#include <QDoubleSpinBox>

#include "customdelegate.h"

CustomDelegate::CustomDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget* CustomDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox *dsb = new QDoubleSpinBox(parent);

    dsb->setMinimum(-99999);
    dsb->setMaximum(99999);
    dsb->setButtonSymbols(QDoubleSpinBox::NoButtons);
    dsb->setSingleStep(0);
    dsb->setDecimals(3);

    return dsb;
}

void CustomDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    double value = index.model()->data(index, Qt::EditRole).toDouble();

    QDoubleSpinBox *dsb = static_cast<QDoubleSpinBox *>(editor);
    dsb->setValue(value);
}

void CustomDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *dsb = static_cast<QDoubleSpinBox *>(editor);
    dsb->interpretText();
    double value = dsb->value();

    model->setData(index, value, Qt::EditRole);
}

void CustomDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
