#ifndef COMMANDTEACHERDELETE_H
#define COMMANDTEACHERDELETE_H

#include <QUndoCommand>
#include <QList>
#include <QTableWidgetItem>

class QTableWidget;

class CommandTeacherDelete : public QUndoCommand
{
public:
    explicit CommandTeacherDelete(QTableWidget*, QStringList*, int*, int, QObject *parent = 0);

    ~CommandTeacherDelete();

    virtual void undo();

    virtual void redo();

signals:

public slots:

private:
    QTableWidget *m_tableWidget;
    QStringList *m_hTableHeader;

    int clickedColumn;
    int *visibleColumns;
};

#endif // COMMANDTEACHERDELETE_H
