#ifndef HIGHLIHGTITEMDELEGATE_H
#define HIGHLIHGTITEMDELEGATE_H

#include <QItemDelegate>

class HighlightItemDelegate : public QItemDelegate
{
public:
    explicit HighlightItemDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

public slots:

};

#endif // HIGHLIHGTITEMDELEGATE_H
