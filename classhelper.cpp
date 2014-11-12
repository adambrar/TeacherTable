#include "classhelper.h"
#include "commandclassadd.h"
#include "commandclassdelete.h"
#include "commandclassedit.h"
#include "commandclasshighlight.h"
#include "commandclassmove.h"
#include "commandclasspaste.h"
#include "maintablewidget.h"

#include <QTableWidgetItem>
#include <QUndoStack>
#include <QMessageBox>

ClassHelper::ClassHelper(MainTableWidget *m_pTableWidget, QUndoStack *m_pUndoStack, QObject *parent) :
    QObject(parent)
{
    this->m_tableWidget = m_pTableWidget;
    this->m_undoStack = m_pUndoStack;
}

void ClassHelper::setClass(QTableWidgetItem *item, int nRow, int nColumn)
{
    QTableWidgetItem *newItem = new QTableWidgetItem(*item);
    this->m_undoStack->push( new CommandClassAdd(nRow, nColumn, newItem, m_tableWidget) );
}

void ClassHelper::deleteClass()
{
    if(this->m_tableWidget->currentColumn() < 1 || \
            this->m_tableWidget->currentItem()->data(Qt::UserRole).toStringList().size() <= 1)
    {
        return;
    } else {
        this->m_undoStack->push( new CommandClassDelete( \
                           this->m_tableWidget->currentRow(), \
                           this->m_tableWidget->currentColumn(), \
                           this->m_tableWidget->currentItem(), \
                           this->m_tableWidget) );
    }
}

void ClassHelper::moveClass(QTableWidgetItem *movedItem, \
                           int toRow, int toCol, \
                           int fromRow, int fromCol)
{
    if(movedItem->text() != QString(" \n \n "))
    {
        m_undoStack->push( new CommandClassMove( \
                        toRow, toCol, \
                        fromRow, fromCol, \
                        this->m_tableWidget, movedItem ) );
    }

}

void ClassHelper::editClass(QTableWidgetItem *item, int nRow, int nColumn)
{
    QTableWidgetItem *newItem = new QTableWidgetItem(*item);
    this->m_undoStack->push( new CommandClassEdit(nRow, nColumn, newItem, this->m_tableWidget) );
}

void ClassHelper::copyClass()
{
    if(this->m_tableWidget->currentColumn() < 1 || \
            this->m_tableWidget->currentItem()->data(Qt::UserRole).toStringList().size() <= 1)
        return;

    this->m_tableWidget->resetCopiedItemNumber();
    this->m_tableWidget->setCopiedItem( this->m_tableWidget->currentItem() );
}

void ClassHelper::pasteClass()
{
    if(this->m_tableWidget->currentColumn() < 1 || \
            this->m_tableWidget->getCopiedItem()->text() == " \n \n ")
        return;
    else
        this->m_undoStack->push( new CommandClassPaste(this->m_tableWidget->currentRow(), \
                                                       this->m_tableWidget->currentColumn(), \
                                                       this->m_tableWidget) );
}

void ClassHelper::highlightClass(int row, int column, QString highlighter)
{
    this->m_undoStack->push( new CommandClassHighlight(row, column, highlighter, \
                                                       this->m_tableWidget) );
}

void ClassHelper::showClass()
{
    if(this->m_tableWidget->currentColumn() < 1)
        return;

    QTableWidgetItem *clickedItem = this->m_tableWidget->currentItem();

    if( clickedItem->data(Qt::UserRole).toStringList().size() <= 1)
        return;

    QString displayedMessage = \
            clickedItem->data(Qt::UserRole).toList().at(MainTableOptions::ClassNotes).toString();

    if(displayedMessage.length() < 1)
        QMessageBox::information( this->m_tableWidget, "Notes", "No notes for this class." );
    else
        QMessageBox::information( this->m_tableWidget, "Notes", displayedMessage );
}
