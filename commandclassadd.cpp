#include "commandclassadd.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QUndoCommand>

CommandClassAdd::CommandClassAdd(int nRow, int nCol, QTableWidgetItem *nClass, \
                                 QTableWidget *nTableWidget, QObject*)
{
    this->m_tableWidget = nTableWidget;
    this->m_toClass = new QTableWidgetItem(*nClass);
    this->row = nRow;
    this->col = nCol;

    // check for widget in cell and store if exists
    if(nTableWidget->item(nRow, nCol) == 0)
    {
        this->m_fromClass = new QTableWidgetItem(" \n \n ");
    } else {
        this->m_fromClass = new QTableWidgetItem( *(nTableWidget->item(nRow, nCol)) );
    }

    setText( QString("Class added at %1, %2").arg(nCol).arg(nRow));
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

    QString grade = m_toClass->data(Qt::UserRole).toStringList().at(1);

    if(grade == "8")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,0,0));
    else if(grade == "9")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,255,0));
    else if(grade == "10")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(0,255,0));
    else if(grade == "11")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(0,255,255));
    else if(grade == "12")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(0,0,255));
    else if(grade == "xx - xx")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,255,255));
    else
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,0,255));

}
