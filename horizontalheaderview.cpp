#include "horizontalheaderview.h"
#include "maintablewidget.h"

#include <QDropEvent>
#include <QMouseEvent>

HorizontalHeaderView::HorizontalHeaderView(MainTableWidget* m_pTableWidget, QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    this->m_tableWidget = m_pTableWidget;
    this->pressedPoint = QPoint(0,0);
    this->setMouseTracking(false);
    this->blackedOutCols = 1;
    this->pressedLogicalIndex = this->pressedVisualIndex = 0;
}

void HorizontalHeaderView::mousePressEvent(QMouseEvent *event)
{
    this->pressedPoint = event->pos();
    this->pressedLogicalIndex = this->logicalIndexAt(event->pos());
    this->pressedVisualIndex = this->visualIndex(this->pressedLogicalIndex);

    QHeaderView::mousePressEvent(event);
}

void HorizontalHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
    QHeaderView::mouseReleaseEvent(event);

    if(event->pos() != this->pressedPoint)
    {
        int toVisualIndex = this->visualIndex(this->pressedLogicalIndex);

        if(toVisualIndex < this->blackedOutCols)
        {
            this->m_tableWidget->horizontalHeader()->moveSection(toVisualIndex, this->blackedOutCols);
            toVisualIndex = this->blackedOutCols;
        }

        if(this->pressedVisualIndex < this->blackedOutCols)
        {
            this->m_tableWidget->horizontalHeader()->moveSection(toVisualIndex, this->pressedVisualIndex);
            return;
        }

        emit teacherMoved(this->pressedVisualIndex, toVisualIndex);
    }
}

void HorizontalHeaderView::mouseMoveEvent(QMouseEvent *event)
{
    if(pressedVisualIndex < this->blackedOutCols)
        return;

    QHeaderView::mouseMoveEvent(event);
}
