#include "commandclassmove.h"
#include "maintablewidget.h"

#include <QTableWidgetItem>

CommandClassMove::CommandClassMove(int nToRow, int nToCol, \
                                   int nFromRow, int nFromCol, \
                                   MainTableWidget *m_pTableWidget, QTableWidgetItem *m_pClass, \
                                   QObject*)
{
    m_tableWidget = m_pTableWidget;
    m_toClass = new QTableWidgetItem(*m_pClass);
    m_fromClass = new QTableWidgetItem(*m_pTableWidget->item(nToRow, nToCol));

    fromRow = nFromRow;
    fromCol = nFromCol;
    toRow = nToRow;
    toCol = nToCol;
    setText(QString("%1, %2 -> %3, %4").arg( \
                QString::number(fromRow)).arg(QString::number(fromCol)).arg( \
                    QString::number(toRow)).arg(QString::number(toCol)));
}

CommandClassMove::~CommandClassMove()
{
}

void CommandClassMove::undo()
{
    m_tableWidget->setItem( fromRow, fromCol, new QTableWidgetItem(*m_toClass) );
    m_fromClass->setData( Qt::UserRole, QVariant(0) );
    m_tableWidget->setItem( toRow, toCol, new QTableWidgetItem(*m_fromClass) );
}

void CommandClassMove::redo()
{
    m_tableWidget->setItem( toRow, toCol, new QTableWidgetItem(*m_toClass) );
    m_tableWidget->setItem( fromRow, fromCol, new QTableWidgetItem(" \n \n ") );
}
