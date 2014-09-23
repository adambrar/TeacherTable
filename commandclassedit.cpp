#include "commandclassedit.h"
#include "maintablewidget.h"

#include <QTableWidget>

CommandClassEdit::CommandClassEdit(int nRow, int nCol, QTableWidgetItem *toClass, \
                                   MainTableWidget *nTableWidget, QObject*)
{
    this->row = nRow;
    this->col = nCol;
    this->m_toClass = toClass->clone();

    this->m_tableWidget = nTableWidget;

    if(nTableWidget->item(nRow, nCol) == 0)
    {
        this->m_fromClass = new QTableWidgetItem(" \n \n ");
    } else {
        this->m_fromClass = new QTableWidgetItem( *(nTableWidget->item(nRow, nCol)) );
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

    QString grade = m_fromClass->data(Qt::UserRole).toStringList().at(MainTableOptions::ClassGrade);

    m_tableWidget->item(row, col)->setBackgroundColor(this->m_tableWidget->TableOptions()->getGradeColor(grade));

}

void CommandClassEdit::redo()
{
    m_tableWidget->setItem( row, col, new QTableWidgetItem(*m_toClass) );

    QString grade = m_toClass->data(Qt::UserRole).toStringList().at(MainTableOptions::ClassGrade);

    m_tableWidget->item(row, col)->setBackgroundColor(this->m_tableWidget->TableOptions()->getGradeColor(grade));

}
