#include "maintablewidget.h"
#include "commandclassmove.h"
#include "horizontalheaderview.h"
#include "maintableoptions.h"
#include "highlightitemdelegate.h"

#include <QTableWidget>
#include <QMouseEvent>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QPrinter>
#include <QPainter>
#include <QScrollBar>

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

QList<MainTableWidget*> MainTableWidget::createPrintableTable(int pageWidth)
{
   QList<MainTableWidget*> tempTables;

    int colsPerPage = (pageWidth - this->verticalHeader()->width()) / this->columnWidth(1);
    int numPages = this->columnCount() / colsPerPage;
    if(this->columnCount()%colsPerPage != 0)
        numPages++;

    for(int page=0; page<numPages; page++)
    {
        tempTables.append(new MainTableWidget);
        tempTables.at(page)->setColumnCount(colsPerPage);
        tempTables.at(page)->setRowCount(numBlocks());
        tempTables.at(page)->setVerticalHeaderLabels(this->m_VTableHeader);
        tempTables.at(page)->horizontalHeader()->setFixedHeight(this->horizontalHeader()->height());

        tempTables.at(page)->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tempTables.at(page)->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        int startCol = 1;
        if(page != 0)
            startCol = page*colsPerPage + 1;

        int finishCol = (page+1)*colsPerPage+1;
        if(finishCol > this->columnCount())
        {
            finishCol = this->columnCount();
            tempTables.at(page)->setColumnCount(finishCol - startCol);
        }

        int tempCol = 0;

        for(int col=startCol; col<finishCol; col++)
        {
            tempTables.at(page)->setHorizontalHeaderItem(tempCol, this->horizontalHeaderItem(col));
            tempTables.at(page)->setItem(0, tempCol, new QTableWidgetItem("wwwwwww"));
            tempTables.at(page)->resizeColumnToContents(tempCol);
            tempTables.at(page)->setItemDelegateForColumn(tempCol, new HighlightItemDelegate(tempTables.at(0)));

            for(int row=0; row<numBlocks(); row++)
            {
                QTableWidgetItem *tempItem = this->item(row,col)->clone();
                tempTables.at(page)->setItem(row, tempCol, new QTableWidgetItem(*tempItem->clone()));
            }
            tempCol++;
        }

        tempTables.at(page)->resizeRowsToContents();

        int height = this->getTableSize(tempTables.at(page)).height();

        int width = this->getTableSize(tempTables.at(page)).width();
        width -= tempTables.at(page)->verticalScrollBar()->width();

        tempTables.at(page)->resize(width, height);

        tempTables.at(page)->setFocusPolicy(Qt::NoFocus);
        if(tempTables.at(page)->columnCount()<1)
            tempTables.removeAt(page);
    }

    return tempTables;
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
    m_HTableHeader<<" \n \n \nA\nD\nD\n \nT\nE\nA\nC\nH\nE\nR\n \n ";
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
    int addCol = 0;

    for (int row = 0; row < m_pTableWidget->rowCount(); row++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem \
                (tr(" \n \n "));
        newItem->setTextAlignment(Qt::AlignCenter);

        newItem->setFlags(newItem->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsDropEnabled);
        newItem->setData(Qt::UserRole, (QVariant)QString("0"));

        m_pTableWidget->setItem(row, addCol, newItem);

        m_pTableWidget->resizeRowToContents(row);
    }


    m_pTableWidget->item(0,addCol)->setText("Double\nclick\ncells to");
    m_pTableWidget->item(1,addCol)->setText("create new\nclasses.\nClick");
    m_pTableWidget->item(2,addCol)->setText("Add Teacher\nto add\nteachers.");
    m_pTableWidget->item(3,addCol)->setText("Drag and\ndrop\ncells/columns");
    m_pTableWidget->item(4,addCol)->setText("to reorder.\nRight\nclick to");
    m_pTableWidget->item(5,addCol)->setText("access\noptions.\nClick");
    m_pTableWidget->item(6,addCol)->setText("block\nheaders\nto get");
    m_pTableWidget->item(7,addCol)->setText("grade\ntotals\nby row.");

    m_pTableWidget->resizeColumnToContents(addCol);

}

QSize MainTableWidget::getTableSize(MainTableWidget *table)
{
    int w = table->verticalHeader()->width()+4;
    w += (table->columnWidth(0)*table->columnCount());
    w += table->verticalScrollBar()->width();

    int h = table->horizontalHeader()->height()+4;
    h += (table->rowHeight(0)*table->rowCount());
    h += table->horizontalScrollBar()->height();

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

void MainTableWidget::HTableHeaderAppend(const QString name)
{
    this->m_HTableHeader.append(name);
}

void MainTableWidget::HTableHeaderRemoveLast()
{
    this->m_HTableHeader.removeLast();
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
