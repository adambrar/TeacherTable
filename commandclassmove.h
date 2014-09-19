#ifndef COMMANDCLASSMOVE_H
#define COMMANDCLASSMOVE_H

#include <QUndoCommand>
#include <QTableWidgetItem>

class MainTableWidget;

class CommandClassMove : public QUndoCommand
{
public:
    explicit CommandClassMove(int, int, int, int, MainTableWidget*, \
                              QTableWidgetItem*, QObject *parent = 0);
    ~CommandClassMove();

    virtual void undo();

    virtual void redo();
    
signals:
    
public slots:

private:
    MainTableWidget *m_tableWidget;
    QTableWidgetItem m_fromClass;
    QTableWidgetItem m_toClass;
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
    
};

#endif // COMMANDCLASSMOVE_H
