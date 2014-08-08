#ifndef COMMANDTEACHEREDIT_H
#define COMMANDTEACHEREDIT_H

#include <QUndoCommand>

class QTableWidget;

class CommandTeacherEdit : public QUndoCommand
{
public:
    explicit CommandTeacherEdit(int, QString, QTableWidget*, QStringList*, QObject *parent = 0);

    ~CommandTeacherEdit();

    virtual void undo();

    virtual void redo();
signals:

public slots:

private:
    QStringList *m_HTableHeader;
    QTableWidget *m_tableWidget;
    QString headerBefore;
    QString headerAfter;
    int editedColumn;
};

#endif // COMMANDTEACHEREDIT_H
