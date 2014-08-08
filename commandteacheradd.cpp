#include "commandteacheradd.h"

#include <QTableWidget>
#include <QHeaderView>
#include <QtCore/QStringList>

CommandTeacherAdd::CommandTeacherAdd(QStringList *newTeachersNames, QStringList *hTableHeader, \
                                     QTableWidget *tableWidget, QObject*)
{
    this->m_tableWidget = tableWidget;
    this->m_newHTableHeader = hTableHeader;

    this->teacherNames = new QStringList;
    foreach(QString name, *newTeachersNames)
    {
        this->teacherNames->append(name);
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
        m_newHTableHeader->removeLast();
    }
}

void CommandTeacherAdd::redo()
{
    foreach( QString name, *teacherNames )
    {
        m_tableWidget->setColumnCount(m_tableWidget->columnCount() + 1);
        QString formattedName;

        foreach( const QChar c, name )
        {
            formattedName.append(c);
            formattedName.append("\n");
        }

        m_newHTableHeader->append(formattedName);
        m_tableWidget->setHorizontalHeaderLabels(*m_newHTableHeader);

        m_tableWidget->setItem( 0, m_tableWidget->columnCount() - 1, \
                                 new QTableWidgetItem("wwwwwww") );
        m_tableWidget->resizeColumnToContents(m_tableWidget->columnCount()-1);


        //insert data
        for (int row = 0; row < m_tableWidget->rowCount(); row++)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem \
                    (QString(" \n \n "));
            newItem->setTextAlignment(Qt::AlignCenter);

            m_tableWidget->setItem(row, m_tableWidget->columnCount() - 1, newItem);

            m_tableWidget->resizeRowToContents(row);
            m_tableWidget->item( row, m_tableWidget->columnCount() - 1)-> \
                    setBackgroundColor(QColor(255,255,255) );
        }

        m_tableWidget->horizontalHeader()->swapSections( \
                   m_tableWidget->columnCount() - 2, \
                   m_tableWidget->columnCount() - 1 );
    }
}
