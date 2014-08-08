#include "commandteacheredit.h"

#include <QStringList>
#include <QTableWidget>

CommandTeacherEdit::CommandTeacherEdit(int column, QString name, \
                                       QTableWidget *m_pTableWidget, \
                                       QStringList *m_pHTableHeader, QObject*)
{
    this->editedColumn = column;
    this->headerBefore = m_pHTableHeader->at(column);
    this->m_tableWidget = m_pTableWidget;
    this->m_HTableHeader = m_pHTableHeader;

    QString formattedName;

    foreach( const QChar c, name )
    {
        formattedName.append(c);
        formattedName.append("\n");
    }

    this->headerAfter = formattedName;

    QString nameBefore = headerBefore.remove("\n");
    setText( QString("%1 to %2 at %3").arg(nameBefore).arg(name).arg(column));
}

CommandTeacherEdit::~CommandTeacherEdit()
{
}

void CommandTeacherEdit::undo()
{
    this->m_HTableHeader->replace(editedColumn, headerBefore);
    this->m_tableWidget->setHorizontalHeaderLabels(*this->m_HTableHeader);
}

void CommandTeacherEdit::redo()
{
    this->m_HTableHeader->replace(editedColumn, headerAfter);
    this->m_tableWidget->setHorizontalHeaderLabels(*this->m_HTableHeader);
}
