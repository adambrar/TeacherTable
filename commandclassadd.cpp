#include "commandclassadd.h"
#include "maintableoptions.h"
#include "maintablewidget.h"

#include <QTableWidgetItem>
#include <QUndoCommand>

CommandClassAdd::CommandClassAdd(int nRow, int nCol, QTableWidgetItem *nClass, \
                                 MainTableWidget *nTableWidget, QObject*)
{
    this->m_tableWidget = nTableWidget;
    this->m_toClass = nClass->clone();
    this->row = nRow;
    this->col = nCol;

    // check for widget in cell and store if exists
    if(nTableWidget->item(nRow, nCol) == 0)
    {
        this->m_fromClass = new QTableWidgetItem(" \n \n ");
    } else {
        this->m_fromClass = new QTableWidgetItem( *(nTableWidget->item(nRow, nCol)) );
    }

    setText( QString("Class added at %1, %2").arg(nRow).arg(nCol));
}

CommandClassAdd::~CommandClassAdd()
{
    delete this->m_toClass;
    delete this->m_fromClass;
}

void CommandClassAdd::undo()
{
    m_tableWidget->setItem( row, col, new QTableWidgetItem(*m_fromClass) );

}

void CommandClassAdd::redo()
{
    m_tableWidget->setItem(row, col, new QTableWidgetItem(*m_toClass) );

    QString grade = m_toClass->data(Qt::UserRole).toStringList().at(MainTableOptions::ClassGrade);

    m_tableWidget->item(row, col)-> \
            setBackgroundColor(this->m_tableWidget->TableOptions()->getGradeColor(grade));

}
