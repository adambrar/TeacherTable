#ifndef TEACHERHELPER_H
#define TEACHERHELPER_H

#include <QObject>

class MainTableWidget;
class QUndoStack;
class QTextEdit;

class TeacherHelper : public QObject
{
    Q_OBJECT
public:
    TeacherHelper(MainTableWidget *m_pTableWidget, QUndoStack *m_pUndoStack, QObject *parent = 0);
signals:

public slots:
    void createNewTeachers(QTextEdit*);
    void editTeacher(QString, int);
    void moveTeacher(int, int);

private:
    MainTableWidget *m_tableWidget;
    QUndoStack *m_undoStack;
};

#endif // TEACHERHELPER_H
