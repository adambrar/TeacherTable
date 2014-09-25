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

    if( index.data(Qt::UserRole).toStringList().isEmpty() ||
            index.data(Qt::UserRole).toStringList().size() <= 1 ) {
        return;
    } else {
        QString data = index.data(Qt::UserRole).toStringList().at(MainTableOptions::ClassHighlight);

        QPen pen;
        pen.setWidth(4);

        if( data == "Highlight 1") {
            pen.setColor( QColor(4,86,232) );
            painter->setPen(pen);
            painter->drawRect(option.rect);
        } else if( data == "Highlight 2") {
            pen.setColor( QColor(13,9,255) );
            painter->setPen(pen);
            painter->drawRect(option.rect);
        } else if( data == "Highlight 3") {
            pen.setColor( QColor(98,4,232) );
            painter->setPen(pen);
            painter->drawRect(option.rect);
        } else if( data == "Highlight 4") {
            pen.setColor( QColor(206,4,255) );
            painter->setPen(pen);
            painter->drawRect(option.rect);
        }

    }
}
