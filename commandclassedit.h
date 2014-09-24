#ifndef COMMANDCLASSEDIT_H
#define COMMANDCLASSEDIT_H

#include <QUndoCommand>

class QTableWidgetItem;
class MainTableWidget;

class CommandClassEdit : public QUndoCommand
{
public:
    explicit CommandClassEdit(int, int, QTableWidgetItem*, MainTableWidget*, QObject *parent = 0);

    ~CommandClassEdit();

    virtual void undo();

    virtual void redo();

signals:

public slots:

private:
    int row;
    int col;

    QBrush *oldBrush;

    QTableWidgetItem *m_fromClass;
    QTableWidgetItem *m_toClass;

    MainTableWidget *m_tableWidget;
};

#endif // COMMANDCLASSEDIT_H
