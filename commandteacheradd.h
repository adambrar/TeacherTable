#ifndef COMMANDTEACHERADD_H
#define COMMANDTEACHERADD_H

#include <QUndoCommand>

class QTableWidget;

class CommandTeacherAdd : public QUndoCommand
{
public:
    explicit CommandTeacherAdd(QStringList*, QStringList*, QTableWidget*, QObject *parent = 0);

    ~CommandTeacherAdd();

    virtual void undo();

    virtual void redo();
signals:

public slots:

private:
    QTableWidget *m_tableWidget;
    QStringList *m_oldHTableHeader;
    QStringList *m_newHTableHeader;
    QStringList *teacherNames;
};

#endif // COMMANDTEACHERADD_H
