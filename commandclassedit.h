#ifndef COMMANDCLASSEDIT_H
#define COMMANDCLASSEDIT_H

#include <QUndoCommand>

class QTableWidgetItem;
class QTableWidget;

class CommandClassEdit : public QUndoCommand
{
public:
    explicit CommandClassEdit(int, int, QTableWidgetItem*, QTableWidget*, QObject *parent = 0);

    ~CommandClassEdit();

    virtual void undo();

    virtual void redo();

signals:

public slots:

private:
    int row;
    int col;

    QTableWidgetItem *m_fromClass;
    QTableWidgetItem *m_toClass;

    QTableWidget *m_tableWidget;
};

#endif // COMMANDCLASSEDIT_H
