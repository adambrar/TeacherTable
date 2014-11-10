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
    void hHeaderSelected(int);
    void vHeaderSelected(int);
    void cellDoubleClicked(int, int);

    void printTable();
    void tableToPDF();

    void setClass(QTableWidgetItem*, int, int);
    void editClassDialog();
    void editClass(QTableWidgetItem*, int, int);
    void deleteClass();
    void moveClass(QTableWidgetItem*, int toRow, int toCol, \
                   int fromRow, int fromCol);
    void copyClass();
    void pasteClass();
    void highlightClass(int, int, QString);
    void showClass();

    void createNewTeachers(QTextEdit*);
    void editTeacher(QString, int);
    void moveTeacher(int, int);

    void createNewTable();
    void cellContextMenu(QPoint);
    void headerContextMenu(QPoint);
    void showUndoStack();

    void saveToFile();
    void saveAsToFile();
    void loadFromFile();

    void showHelp();
    void showQtHelp();

    void closeWindow();

private:
    Ui::MainWindow *ui;

    QUndoStack *m_undoStack;
    QUndoView *m_undoView;
    MainTableWidget *m_pTableWidget;

    QStringList m_HTableHeader;
    QStringList m_VTableHeader;

    QString *m_saveFileName;
};

#endif // MAINWINDOW_H
