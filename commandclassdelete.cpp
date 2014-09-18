#include "commandclassdelete.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QUndoStack>

CommandClassDelete::CommandClassDelete(int nRow, int nColumn, QTableWidgetItem *nClass, \
                                       QTableWidget *nTableWidget, QObject*)
{
    this->row = nRow;
    this->col = nColumn;
    this->m_class = new QTableWidgetItem(*nClass);
    this->m_tableWidget = nTableWidget;

    setText( QString("Class deleted at %1, %2").arg(nRow).arg(nColumn));
}

CommandClassDelete::~CommandClassDelete()
{
    delete m_class;
}

//readds class item to cell and recolors it
void CommandClassDelete::undo()
{
    m_tableWidget->setItem(row, col, new QTableWidgetItem(*m_class));

    if( m_class->data(Qt::UserRole).toString().isEmpty() )
    {
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,255,255));
        return;
    }

    QString grade = m_class->data(Qt::UserRole).toStringList().at(1);

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
            setBackgroundColor(QColor(0,0,255,100));
    else if(grade.contains("8"))
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,0,255));
    else if(grade.contains("9"))
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,50,255));
    else if(grade.contains("10"))
        m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,100,255));

}

//removes class item from cell and colors cell white
void CommandClassDelete::redo()
{
    m_tableWidget->setItem(row, col, new QTableWidgetItem(" \n \n "));
    m_tableWidget->item(row, col)-> \
            setBackgroundColor(QColor(255,255,255));

}
