#ifndef COMMANDCLASSDELETE_H
#define COMMANDCLASSDELETE_H

#include <QUndoCommand>

class MainTableWidget;
class QTableWidgetItem;

class CommandClassDelete : public QUndoCommand
{
public:
    explicit CommandClassDelete(int nRow, int nColumn, QTableWidgetItem *nClass, \
                                MainTableWidget *nTableWidget, QObject *parent = 0);
    ~CommandClassDelete();

    virtual void undo();

    virtual void redo();

private:
    MainTableWidget *m_tableWidget;
    QTableWidgetItem *m_class;
    int row;
    int col;


    
};

#endif // COMMANDCLASSDELETE_H
