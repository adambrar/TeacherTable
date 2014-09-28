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
    this->m_tableWidget->setItem( fromRow, fromCol, new QTableWidgetItem(*m_toClass) );
    this->m_tableWidget->setItem( toRow, toCol, new QTableWidgetItem(*m_fromClass) );

    this->m_tableWidget->hide();
    this->m_tableWidget->show();
    this->m_tableWidget->setCurrentCell(fromRow,fromCol);
}

void CommandClassMove::redo()
{
    this->m_tableWidget->setItem( toRow, toCol, new QTableWidgetItem(*m_toClass) );
    this->m_tableWidget->setItem( fromRow, fromCol, new QTableWidgetItem(" \n \n ") );
    this->m_tableWidget->item(fromRow, fromCol)->setData(Qt::UserRole, (QVariant)QString("1"));

    this->m_tableWidget->hide();
    this->m_tableWidget->show();
    this->m_tableWidget->setCurrentCell(toRow,toCol);
}
