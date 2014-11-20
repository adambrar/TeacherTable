#include "teacherhelper.h"
#include "commandteacheradd.h"
#include "commandteacherdelete.h"
#include "commandteacheredit.h"
#include "commandteachermove.h"
#include "maintablewidget.h"

#include <QTableWidgetItem>
#include <QUndoStack>
#include <QTextEdit>

TeacherHelper::TeacherHelper(MainTableWidget *m_pTableWidget, QUndoStack *m_pUndoStack, QObject *parent) :
    QObject(parent)
{
    this->m_tableWidget = m_pTableWidget;
    this->m_undoStack = m_pUndoStack;
}

void TeacherHelper::createNewTeachers(QTextEdit *inText)
{
    QString input = inText->toPlainText().simplified().toUpper();
    if(input.length() < 1) { return; }

    QStringList teacherNames = input.split( "," );

    for(int i=0;i<teacherNames.length();i++)
    {
        if(teacherNames.at(i).length()>=18) {
            QString replaceName = teacherNames.at(i);
            int chopPoint = teacherNames.at(i).length()-18;
            replaceName.chop(chopPoint);
            teacherNames.replace(i, replaceName);
        } else if(teacherNames.at(i).length()<1) {
            teacherNames.removeAt(i);
        }
    }

    this->m_undoStack->push( new CommandTeacherAdd(&teacherNames, \
                                                   &this->m_tableWidget->HTableHeader(), \
                                                   this->m_tableWidget) );
}

void TeacherHelper::editTeacher(QString headerAfter, int column)
{
    this->m_undoStack->push( new CommandTeacherEdit(column, headerAfter, this->m_tableWidget));
}

void TeacherHelper::moveTeacher(int fromIndex, int toIndex)
{
    this->m_undoStack->push( new CommandTeacherMove(this->m_tableWidget, fromIndex, toIndex));
}
