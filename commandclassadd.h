#ifndef COMMANDCLASSADD_H
#define COMMANDCLASSADD_H

#include <QUndoCommand>

class MainTableWidget;
class QTableWidgetItem;

class CommandClassAdd : public QUndoCommand
{
public:
    explicit CommandClassAdd(int, int, QTableWidgetItem*, MainTableWidget*, QObject *parent = 0);
    ~CommandClassAdd();

    //reset cell to old widget, m_fromClass
    virtual void undo();

    //set cell to newly created widget, m_toClass
    virtual void redo();
    
private:
    MainTableWidget *m_tableWidget;
    QTableWidgetItem *m_fromClass;
    QTableWidgetItem *m_toClass;

    int row;
    int col;

};

#endif // COMMANDCLASSADD_H
