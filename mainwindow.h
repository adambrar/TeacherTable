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
class ClassHelper;


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

    void editClassDialog();

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

    ClassHelper *classHelper;
};

#endif // MAINWINDOW_H
