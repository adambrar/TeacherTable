#ifndef COMMANDTEACHEREDIT_H
#define COMMANDTEACHEREDIT_H

#include <QUndoCommand>

class MainTableWidget;

class CommandTeacherEdit : public QUndoCommand
{
public:
    explicit CommandTeacherEdit(int, QString, MainTableWidget *, QObject *parent = 0);

    ~CommandTeacherEdit();

    virtual void undo();

    virtual void redo();
signals:

public slots:

private:
    MainTableWidget *m_tableWidget;
    QString headerBefore;
    QString headerAfter;
    int editedColumn;
};

#endif // COMMANDTEACHEREDIT_H
