#include "maintablewidget.h"
#include "commandclassmove.h"
#include "horizontalheaderview.h"
#include "maintableoptions.h"

#include <QTableWidget>
#include <QMouseEvent>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>

MainTableWidget::MainTableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    this->copiedItemNumber = 0;

    this->hHeaderView = new HorizontalHeaderView(this);
    this->setHorizontalHeader( hHeaderView );
    this->blackoutColumns = 1;
    this->tableOptions = new MainTableOptions;
}

MainTableWidget::~MainTableWidget()
{
    delete this->hHeaderView;
}

void MainTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    QTableWidgetItem *item = this->itemAt(event->pos());

    if(item == 0 || this->visualColumn(item->column()) < this->blackoutColumns)
            return;

    this->movingItem = new QTableWidgetItem( *itemAt( event->pos() ));
    this->fromIndex = indexAt(event->pos());

    QTableWidget::mouseMoveEvent(event);
}

void MainTableWidget::dropEvent(QDropEvent *event)
{
    QModelIndex toIndex = this->indexAt(event->pos());
    QTableWidgetItem *item = this->itemAt(event->pos());
    this->hide();
    this->show();

    if(item == 0 || this->visualColumn(item->column()) < this->blackoutColumns)
        return;

    emit classMoved(movingItem, toIndex.row(), toIndex.column(), \
                    fromIndex.row(), fromIndex.column());

    setCurrentCell(toIndex.row(), toIndex.column());
}

void MainTableWidget::setUndoStack(QUndoStack *m_undoStack)
{
    undoStack = m_undoStack;
}

void MainTableWidget::initTableWidget( QTableWidget *m_pTableWidget )
{
    m_pTableWidget->setRowCount(numBlocks()+numExtraBlocks());
    m_pTableWidget->setColumnCount(1);

    m_HTableHeader.clear();
    m_HTableHeader<<"A\nd\nd\n \nT\ne\na\nc\nh\ne\nr";
    m_VTableHeader.clear();
    m_VTableHeader<<"  A "<<"  B "<<"  C "<<"  D "<<"  E "<<"  F "<<"  G "<<"  H "<<" "<<" "<<" ";
    m_pTableWidget->setHorizontalHeaderLabels(m_HTableHeader);
    m_pTableWidget->setVerticalHeaderLabels(m_VTableHeader);
    m_pTableWidget->horizontalHeader()->setSectionsClickable(true);

    m_pTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    m_pTableWidget->horizontalHeader()->setDragEnabled(true);
    m_pTableWidget->horizontalHeader()->setSectionsMovable(true);
    m_pTableWidget->horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);
    m_pTableWidget->horizontalHeader()->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableWidget->horizontalHeader()->setSelectionBehavior(QAbstractItemView::SelectColumns);

    m_pTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_pTableWidget->verticalHeader()->setSectionsClickable(true);

    m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);
    m_pTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableWidget->setShowGrid(true);

    m_pTableWidget->setGeometry(QApplication::desktop()->screenGeometry());
    m_pTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_pTableWidget->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    m_pTableWidget->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    m_pTableWidget->setDragEnabled(true);
    m_pTableWidget->setDragDropMode(QAbstractItemView::InternalMove);
    m_pTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
}

void MainTableWidget::insertInstructions( QTableWidget *m_pTableWidget )
{
    //insert data
    int lastCol = m_pTableWidget->columnCount() - 1;

    for (int row = 0; row < m_pTableWidget->rowCount(); row++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem \
                (tr(" \n \n "));
        newItem->setTextAlignment(Qt::AlignCenter);

        newItem->setFlags(newItem->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsDropEnabled);
        newItem->setData(Qt::UserRole, (QVariant)QString("0"));

        m_pTableWidget->setItem(row, lastCol, newItem);

        m_pTableWidget->resizeRowToContents(row);
    }


    m_pTableWidget->item(0,lastCol)->setText("Double\nclick\ncells");
    m_pTableWidget->item(1,lastCol)->setText("to\ncreate\nnew");
    m_pTableWidget->item(2,lastCol)->setText("classes.\n \nClick");
    m_pTableWidget->item(3,lastCol)->setText("New\nTeacher\nto");
    m_pTableWidget->item(4,lastCol)->setText("add\nteachers.\n ");
    m_pTableWidget->item(5,lastCol)->setText("Drag\nand\nDrop");
    m_pTableWidget->item(6,lastCol)->setText("wwwwwww");

    m_pTableWidget->resizeColumnToContents(lastCol);
    m_pTableWidget->item(6,lastCol)->setText("cells\nto\nreorder");

}

QSize MainTableWidget::getTableSize( QTableWidget *m_pTableWidget )
{
    int w = m_pTableWidget->verticalHeader()->width() + 4;
    w += m_pTableWidget->columnWidth(0) * 8;

    int h = m_pTableWidget->horizontalHeader()->height() + 4;
    h += m_pTableWidget->rowHeight(0) * (numBlocks() + numExtraBlocks() + 1);

    return QSize(w, h);
}

QStringList MainTableWidget::HTableHeader() const
{
    return m_HTableHeader;
}

void MainTableWidget::setHTableHeader(const QStringList &HTableHeader)
{
    m_HTableHeader = HTableHeader;
}

QStringList MainTableWidget::VTableHeader() const
{
    return m_VTableHeader;
}

void MainTableWidget::setVTableHeader(const QStringList &VTableHeader)
{
    m_VTableHeader = VTableHeader;
}

QTableWidgetItem *MainTableWidget::getCopiedItem() const
{
    return copiedItem;
}

void MainTableWidget::setCopiedItem(QTableWidgetItem *value)
{
    copiedItem = new QTableWidgetItem(*value);
}

HorizontalHeaderView *MainTableWidget::getHHeaderView() const
{
    return hHeaderView;
}

QTableWidgetItem* MainTableWidget::emptyCell() const
{
    QTableWidgetItem *item = new QTableWidgetItem("       \n \n ");
    item->setTextAlignment(Qt::AlignCenter);
    item->setBackgroundColor( QColor(255,255,255) );
    return item;
}
