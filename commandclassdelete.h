#ifndef COMMANDCLASSDELETE_H
#define COMMANDCLASSDELETE_H

#include <QUndoCommand>

class QTableWidget;
class QTableWidgetItem;

class CommandClassDelete : public QUndoCommand
{
public:
    explicit CommandClassDelete(int nRow, int nColumn, QTableWidgetItem *nClass, \
                                QTableWidget *nTableWidget, QObject *parent = 0);
    ~CommandClassDelete();

    virtual void undo();

    virtual void redo();

private:
    QTableWidget *m_tableWidget;
    QTableWidgetItem *m_class;
    int row;
    int col;


    
};

#endif // COMMANDCLASSDELETE_H
