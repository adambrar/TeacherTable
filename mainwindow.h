#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QTextEdit>
#include <QLineEdit>
#include <QStringList>

namespace Ui {
class MainWindow;
}
class MainTableWidget;
class QTableWidgetItem;
class QUndoStack;
class QUndoView;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createMenu();
    int numBlocks() { return 8; }
    int numExtraBlocks() { return 2; }
    QSize getWindowSize();

private slots:
    void headerSelected(int);
    void cellDoubleClicked(int, int);

    void setClass(QString, QString, QString, QString, int, int);
    void editClass(QString, QString, QString, QString, int, int);
    void deleteClass();
    void moveClass(QTableWidgetItem*, int toRow, int toCol, \
                   int fromRow, int fromCol);
    void copyClass();
    void pasteClass();

    void createNewTeachers(QTextEdit*);
    void editTeacher(QString, int);
    void moveTeacher(int, int);

    void createNewTable();
    void cellContextMenu(QPoint);
    void headerContextMenu(QPoint);
    void showUndoStack();

    void saveToFile();
    void loadFromFile();

    void closeWindow();

private:
    Ui::MainWindow *ui;

    QUndoStack *m_undoStack;
    QUndoView *m_undoView;
    MainTableWidget *m_pTableWidget;

    QStringList m_HTableHeader;
    QStringList m_VTableHeader;
};

#endif // MAINWINDOW_H
