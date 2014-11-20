#ifndef COMMANDTEACHERADD_H
#define COMMANDTEACHERADD_H

#include <QUndoCommand>

class MainTableWidget;

class CommandTeacherAdd : public QUndoCommand
{
public:
    explicit CommandTeacherAdd(QStringList*, QStringList*, MainTableWidget*, QObject *parent = 0);

    ~CommandTeacherAdd();

    virtual void undo();

    virtual void redo();
signals:

public slots:

private:
    MainTableWidget *m_tableWidget;
    QStringList *m_oldHTableHeader;
    QStringList *teacherNames;
};

#endif // COMMANDTEACHERADD_H
