#include "commandclasshighlight.h"
#include "maintablewidget.h"

CommandClassHighlight::CommandClassHighlight(int row, int col, QString highlighter, \
                                             MainTableWidget *m_ptableWidget, \
                                             QObject*)
{
    this->row = row;
    this->col = col;
    this->m_tableWidget = m_ptableWidget;

    this->oldHighlight = new QString( m_ptableWidget->item(row,col)->data(Qt::UserRole).\
            toStringList().at(MainTableOptions::ClassHighlight) );

    this->newHighlight = new QString( highlighter );
}

CommandClassHighlight::~CommandClassHighlight()
{
    delete this->newHighlight;
    delete this->oldHighlight;
}

void CommandClassHighlight::undo()
{

}

void CommandClassHighlight::redo()
{

}
