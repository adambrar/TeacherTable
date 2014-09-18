#include "commandteachermove.h"
#include "maintablewidget.h"
#include "horizontalheaderview.h"

CommandTeacherMove::CommandTeacherMove(MainTableWidget *m_pTableWidget, int fromColumn, int toColumn, QObject*)
{
    this->fromCol = fromColumn;
    this->toCol = toColumn;

    this->m_tableWidget = m_pTableWidget;

    this->firstTime = true;

    setText( QString("Teacher moved: %1 to %2").arg(fromCol).arg(toCol) );
}

CommandTeacherMove::~CommandTeacherMove()
{
}

void CommandTeacherMove::undo()
{
    this->m_tableWidget->getHHeaderView()->moveSection(toCol, fromCol);
}

void CommandTeacherMove::redo()
{
    if(firstTime==true) {
        firstTime = false;
    } else {
        this->m_tableWidget->getHHeaderView()->moveSection(fromCol, toCol);
    }
}
