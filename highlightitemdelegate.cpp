#include "highlightitemdelegate.h"
#include "maintableoptions.h"

#include <QPen>
#include <QPainter>

HighlightItemDelegate::HighlightItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

void HighlightItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, \
                                  const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    if( index.data(Qt::UserRole).toStringList().isEmpty() ) {
        return;
    } else {
        QString data = index.data(Qt::UserRole).toStringList().at(MainTableOptions::ClassHighlight);

        QPen pen;
        pen.setWidth(3);

        if( data == "None" ) {
            pen.setColor(painter->background().color());

        } else if( data == "Highlight 1") {
            pen.setColor( Qt::red);

        } else if( data == "Highlight 2") {
            pen.setColor( Qt::blue);

        } else if( data == "Highlight 3") {
            pen.setColor( Qt::yellow);

        } else if( data == "Highlight 4") {
            pen.setColor( Qt::green);

        }

        painter->setPen(pen);
        painter->drawRect(option.rect);
    }
}
