#ifndef CLASSHELPER_H
#define CLASSHELPER_H

#include <QObject>

class MainTableWidget;
class QTableWidgetItem;
class QUndoStack;

class ClassHelper : public QObject
{
    Q_OBJECT
public:
    explicit ClassHelper(MainTableWidget *m_pTableWidget, QUndoStack *m_pUndoStack, QObject *parent = 0);

signals:

public slots:
    void setClass(QTableWidgetItem*, int, int);
    void editClass(QTableWidgetItem*, int, int);
    void deleteClass();
    void moveClass(QTableWidgetItem*, int toRow, int toCol, \
                   int fromRow, int fromCol);
    void copyClass();
    void pasteClass();
    void highlightClass(int, int, QString);
    void showClass();

private:
    MainTableWidget *m_tableWidget;
    QUndoStack *m_undoStack;
};

#endif // CLASSHELPER_H
