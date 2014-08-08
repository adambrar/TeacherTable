#ifndef COMMANDCLASSPASTE_H
#define COMMANDCLASSPASTE_H

#include <QUndoCommand>

class MainTableWidget;
class QTableWidgetItem;

class CommandClassPaste : public QUndoCommand
{
public:
    explicit CommandClassPaste(int, int, MainTableWidget*, QObject *parent = 0);

    ~CommandClassPaste();

    virtual void undo();

    virtual void redo();
signals:

public slots:

private:
    int toRow;
    int toCol;

    QTableWidgetItem *m_fromClass;
    QTableWidgetItem *m_toClass;

    MainTableWidget *m_tableWidget;

};

#endif // COMMANDCLASSPASTE_H
