#include "commandteacherdelete.h"

#include <QStringList>

CommandTeacherDelete::CommandTeacherDelete(QTableWidget *tableWidget, QStringList *tableHeader, \
                                           int *visibleColumnsStore, int column, QObject*)
{
    this->m_tableWidget = tableWidget;
    this->m_hTableHeader = tableHeader;
    this->clickedColumn = column;
    this->visibleColumns = visibleColumnsStore;

    QString header = m_tableWidget->takeHorizontalHeaderItem(clickedColumn)-> text();
    header.remove("\n");

    setText( QString("Deleted %1 at %2").arg(header).arg(clickedColumn) );
}

CommandTeacherDelete::~CommandTeacherDelete()
{
    this->m_tableWidget->removeColumn(clickedColumn);
    this->m_hTableHeader->removeAt(clickedColumn);
}

void CommandTeacherDelete::undo()
{
    this->m_tableWidget->showColumn(clickedColumn);
    this->m_tableWidget->setHorizontalHeaderLabels(*this->m_hTableHeader);
    (*visibleColumns)++;
}

void CommandTeacherDelete::redo()
{
    this->m_tableWidget->hideColumn(clickedColumn);
    (*visibleColumns)--;
}
