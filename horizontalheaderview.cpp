#include "horizontalheaderview.h"
#include "maintablewidget.h"

#include <QDropEvent>
#include <QMouseEvent>

HorizontalHeaderView::HorizontalHeaderView(MainTableWidget* m_pTableWidget, QWidget *parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    this->m_tableWidget = m_pTableWidget;
    this->pressedPoint = QPoint(0,0);
    this->releasedPoint = QPoint(0,0);
    this->setMouseTracking(false);
    this->blackedOutCols = 1;
}

void HorizontalHeaderView::mousePressEvent(QMouseEvent *event)
{
    this->pressedPoint = event->pos();

    QHeaderView::mousePressEvent(event);
}

void HorizontalHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
    this->releasedPoint = event->pos();
    int colWidth = this->m_tableWidget->columnWidth(0);

    QHeaderView::mouseReleaseEvent(event);

    if(this->releasedPoint != this->pressedPoint)
    {
        int fromIndex = this->visualIndexAt(this->pressedPoint.x());
        int toIndex = this->visualIndexAt(this->releasedPoint.x());

        if(event->pos().x() < colWidth/2)
        {
            this->m_tableWidget->horizontalHeader()->swapSections(1,0);
            toIndex = 1;
        }

        emit teacherMoved(fromIndex, toIndex);
    }
}

void HorizontalHeaderView::mouseMoveEvent(QMouseEvent *event)
{
    QTableWidgetItem *item = this->m_tableWidget->itemAt(this->pressedPoint);

    if(this->m_tableWidget->visualColumn(item->column()) == 0 || item == 0)
        return;

    QHeaderView::mouseMoveEvent(event);
}
