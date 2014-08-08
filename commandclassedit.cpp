#include "commandclassedit.h"
#include "mainwindow.h"

#include <QTableWidget>

CommandClassEdit::CommandClassEdit(int nRow, int nCol, QTableWidgetItem *toClass, \
                                   QTableWidget *nTableWidget, QObject*)
{
    row = nRow;
    col = nCol;
    m_toClass = new QTableWidgetItem(*toClass);

    m_tableWidget = nTableWidget;

    if(nTableWidget->item(nRow, nCol) == 0)
    {
        m_fromClass = new QTableWidgetItem(" \n \n ");
    } else {
        m_fromClass = new QTableWidgetItem( *(nTableWidget->item(nRow, nCol)) );
    }
    setText( QString("Class edit at %1, %2").arg(nRow).arg(nCol) );
}

CommandClassEdit::~CommandClassEdit()
{
    delete this->m_toClass;
    delete this->m_fromClass;
}

void CommandClassEdit::undo()
{
    m_tableWidget->setItem( row, col, new QTableWidgetItem(*m_fromClass) );

    QString grade = m_fromClass->data(Qt::UserRole).toStringList().at(1);

    if(grade == "8")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,0,0,100));
    else if(grade == "9")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,255,0,100));
    else if(grade == "10")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(0,255,0,100));
    else if(grade == "11")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(0,255,255,100));
    else if(grade == "12")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(0,0,255,100));
    else
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,0,255));

}

void CommandClassEdit::redo()
{
    m_tableWidget->setItem( row, col, new QTableWidgetItem(*m_toClass) );

    QString grade = m_toClass->data(Qt::UserRole).toStringList().at(1);

    if(grade == "8")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,0,0,100));
    else if(grade == "9")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,255,0,100));
    else if(grade == "10")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(0,255,0,100));
    else if(grade == "11")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(0,255,255,100));
    else if(grade == "12")
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(0,0,255,100));
    else
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,0,255));

}
