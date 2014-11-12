#include "commandclassdelete.h"
#include "maintablewidget.h"

#include <QTableWidgetItem>
#include <QUndoStack>

CommandClassDelete::CommandClassDelete(int nRow, int nColumn, QTableWidgetItem *nClass, \
                                       MainTableWidget *nTableWidget, QObject*)
{
    this->row = nRow;
    this->col = nColumn;
    this->m_class = new QTableWidgetItem(*nClass);
    this->m_tableWidget = nTableWidget;

    this->oldBrush = new QBrush( nTableWidget->item(nRow, nColumn)->background() );

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

    if( m_class->data(Qt::UserRole).toStringList().isEmpty() )
    {
        m_tableWidget->item(row, col)-> \
                setBackgroundColor(this->m_tableWidget->TableOptions()->getGradeColor());
    } else {
        QString grade = m_class->data(Qt::UserRole).toStringList().at(MainTableOptions::ClassGrade);

        m_tableWidget->item(row, col)-> \
                setBackgroundColor(this->m_tableWidget->TableOptions()->getGradeColor(grade));
    }
    m_tableWidget->setCurrentCell(row, col);
}

//removes class item from cell and colors cell white
void CommandClassDelete::redo()
{
    m_tableWidget->setItem(row, col, new QTableWidgetItem(" \n \n "));

    m_tableWidget->item(row, col)-> \
            setBackgroundColor(this->m_tableWidget->TableOptions()->getGradeColor());

    m_tableWidget->setCurrentCell(row, col);
}
