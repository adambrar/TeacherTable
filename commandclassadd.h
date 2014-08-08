#ifndef COMMANDCLASSADD_H
#define COMMANDCLASSADD_H

#include <QUndoCommand>

class QTableWidget;
class QTableWidgetItem;

class CommandClassAdd : public QUndoCommand
{
public:
    explicit CommandClassAdd(int, int, QTableWidgetItem*, QTableWidget*, QObject *parent = 0);
    ~CommandClassAdd();

    //reset cell to old widget, m_fromClass
    virtual void undo();

    //set cell to newly created widget, m_toClass
    virtual void redo();
    
private:
    QTableWidget *m_tableWidget;
    QTableWidgetItem *m_fromClass;
    QTableWidgetItem *m_toClass;

    int row;
    int col;

};

#endif // COMMANDCLASSADD_H
