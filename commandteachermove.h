#ifndef COMMANDTEACHERMOVE_H
#define COMMANDTEACHERMOVE_H

#include <QUndoCommand>

class MainTableWidget;

class CommandTeacherMove : public QUndoCommand
{
public:
    explicit CommandTeacherMove(MainTableWidget*, int, int, QObject *parent = 0);

    ~CommandTeacherMove();

    virtual void undo();

    virtual void redo();
signals:

public slots:

private:
    int fromCol;
    int toCol;
    bool firstTime;

    MainTableWidget *m_tableWidget;
};

#endif // COMMANDTEACHERMOVE_H
