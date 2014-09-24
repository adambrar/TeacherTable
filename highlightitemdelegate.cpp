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
        pen.setWidth(3);

        if( data == "Highlight 1") {
            pen.setColor( QColor(220,20,60) );
            painter->setPen(pen);
            painter->drawRect(option.rect);
        } else if( data == "Highlight 2") {
            pen.setColor( QColor(25,25,112) );
            painter->setPen(pen);
            painter->drawRect(option.rect);
        } else if( data == "Highlight 3") {
            pen.setColor( QColor(30,144,255) );
            painter->setPen(pen);
            painter->drawRect(option.rect);
        } else if( data == "Highlight 4") {
            pen.setColor( QColor(255,215,0) );
            painter->setPen(pen);
            painter->drawRect(option.rect);
        }

    }
}
