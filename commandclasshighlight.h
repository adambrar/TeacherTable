#ifndef COMMANDCLASSHIGHLIGHT_H
#define COMMANDCLASSHIGHLIGHT_H

#include <QUndoCommand>

class MainTableWidget;
class QTableWidgetItem;

class CommandClassHighlight : public QUndoCommand
{
public:
    explicit CommandClassHighlight(int, int, QString, MainTableWidget*, QObject *parent = 0);

    ~CommandClassHighlight();

    virtual void undo();

    virtual void redo();
signals:

public slots:

private:
    int row;
    int col;

    QString *oldHighlight;
    QString *newHighlight;

    MainTableWidget *m_tableWidget;
};

#endif // COMMANDCLASSHIGHLIGHT_H
