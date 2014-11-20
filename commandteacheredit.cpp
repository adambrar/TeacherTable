#include "commandteacheredit.h"
#include "maintablewidget.h"

#include <QStringList>
#include <QHeaderView>

CommandTeacherEdit::CommandTeacherEdit(int column, QString name, \
                                       MainTableWidget *m_pTableWidget, \
                                       QObject*)
{
    this->editedColumn = column;
    this->headerBefore = m_pTableWidget->HTableHeader().at(column);
    this->m_tableWidget = m_pTableWidget;

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
    QStringList tmpList = this->m_tableWidget->HTableHeader();
    tmpList.replace(this->editedColumn, this->headerBefore);
    this->m_tableWidget->setHTableHeader(tmpList);
    this->m_tableWidget->setHorizontalHeaderLabels(tmpList);
}

void CommandTeacherEdit::redo()
{
    QStringList tmpList = this->m_tableWidget->HTableHeader();
    tmpList.replace(this->editedColumn, this->headerAfter);
    this->m_tableWidget->setHTableHeader(tmpList);
    this->m_tableWidget->setHorizontalHeaderLabels(tmpList);
}
