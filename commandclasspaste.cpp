#include "commandclasspaste.h"
#include "maintablewidget.h"

#include <QTableWidgetItem>

CommandClassPaste::CommandClassPaste(int currentRow, int currentColumn, \
                                     MainTableWidget *tableWidget, QObject*)
{
    this->m_tableWidget = tableWidget;
    this->m_fromClass = new QTableWidgetItem( *tableWidget->item(currentRow, currentColumn) );

    QStringList oldData = tableWidget->getCopiedItem()->data(Qt::UserRole).toStringList();
    int section = oldData.at(2).toInt();
    section += tableWidget->getCopiedItemNumber();

    oldData.replace(2, QString("%1").arg(section));
    QVariant *newData = new QVariant((QVariant)oldData);

    QString gradeString = tableWidget->getCopiedItem()->text().split("\n").at(1);

    this->m_toClass = new QTableWidgetItem( *tableWidget->getCopiedItem() );
    m_toClass->setData(Qt::UserRole, *newData);
    m_toClass->setText( QString("%1\n%2\n00%3").arg(oldData.at(0)). \
                                                arg(gradeString). \
                                                arg(oldData.at(2)) );

    this->toRow = currentRow;
    this->toCol = currentColumn;

    setText(QString("Class pasted at %1, %2").arg(currentRow).arg(currentColumn));
}

CommandClassPaste::~CommandClassPaste()
{
    delete this->m_toClass;
    delete this->m_fromClass;
}

void CommandClassPaste::undo()
{
    this->m_tableWidget->decreaseCopiedItemNumber();
    this->m_tableWidget->setItem( this->toRow, \
                                  this->toCol, \
                                  new QTableWidgetItem(*this->m_fromClass) );
}

void CommandClassPaste::redo()
{
    this->m_tableWidget->increaseCopiedItemNumber();
    this->m_tableWidget->setItem( this->toRow, \
                                  this->toCol, \
                                  new QTableWidgetItem(*this->m_toClass) );
}
