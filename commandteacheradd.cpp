#include "commandteacheradd.h"
#include "highlightitemdelegate.h"
#include "maintablewidget.h"

#include <QHeaderView>
#include <QtCore/QStringList>

CommandTeacherAdd::CommandTeacherAdd(QStringList *newTeachersNames, QStringList *hTableHeader, MainTableWidget *tableWidget, QObject*)
{
    this->m_tableWidget = tableWidget;

    this->teacherNames = new QStringList;
    foreach(QString name, *newTeachersNames)
    {
        if(name.length() > 15) {
            name.chop(name.length() - 15);
            this->teacherNames->append(name);
        } else if(name.length() < 1) {
            teacherNames->removeOne(name);
        } else {
            this->teacherNames->append(name);
        }
    }

    this->m_oldHTableHeader = new QStringList(*hTableHeader);

    setText(QString("%1 teachers added.").arg(teacherNames->size()));
}

CommandTeacherAdd::~CommandTeacherAdd()
{
    delete this->teacherNames;
    delete this->m_oldHTableHeader;
}

void CommandTeacherAdd::undo()
{
    for( int i=0; i < this->teacherNames->size(); i++ )
    {
        m_tableWidget->removeColumn( m_tableWidget->columnCount() - 1 );
        this->m_tableWidget->HTableHeaderRemoveLast();
    }
}

void CommandTeacherAdd::redo()
{
    foreach( QString name, *teacherNames )
    {
        m_tableWidget->setColumnCount(m_tableWidget->columnCount() + 1);
        m_tableWidget->setItemDelegateForColumn( m_tableWidget->columnCount()-1, \
                                                new HighlightItemDelegate(m_tableWidget) );
        QString formattedName;

        foreach( const QChar c, name )
        {
            formattedName.append(c);
            formattedName.append("\n");
        }

        m_tableWidget->HTableHeaderAppend(formattedName);

        m_tableWidget->setItem( 0, m_tableWidget->columnCount() - 1, \
                                 new QTableWidgetItem("wwwwwww") );
        m_tableWidget->resizeColumnToContents(m_tableWidget->columnCount()-1);


        //insert data
        for (int row = 0; row < m_tableWidget->rowCount(); row++)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem(QString(" \n \n "));
            newItem->setTextAlignment(Qt::AlignCenter);
            newItem->setData(Qt::UserRole, (QVariant)QString("1"));
            newItem->setBackground(Qt::white);

            //insert black column
            if(row == 4)
            {
                newItem->setText(QString(" "));

                newItem->setFlags(newItem->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsDropEnabled);
                newItem->setData(Qt::UserRole, (QVariant)QString("0"));
                newItem->setBackground(Qt::gray);

            }

            m_tableWidget->setItem(row, m_tableWidget->columnCount() - 1, newItem);
            m_tableWidget->resizeRowToContents(row);
        }
    }
    m_tableWidget->setHorizontalHeaderLabels(m_tableWidget->HTableHeader());

}
