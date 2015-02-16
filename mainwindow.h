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
class TeacherHelper;
class QLabel;
class QVBoxLayout;


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
    void loadFile(QString file) { loadFromFile(file); }

private slots:
    void hHeaderSelected(int);
    void vHeaderSelected(int);
    void cellDoubleClicked(int, int);

    void printTable();
    void tableToPDF();

    void createChangeTitleDialog();
    void tableTitleChanged(QString);

    void editClassDialog();
    void createNewTeachersDialog();

    void createNewTable();
    void cellContextMenu(QPoint);
    void headerContextMenu(QPoint);
    void showUndoStack();

    void saveToFile();
    void saveAsToFile();
    void loadFromFile(QString fileName="");

    void showHelp();
    void showQtHelp();

    void closeWindow();

private:
    Ui::MainWindow *ui;

    QUndoStack *m_undoStack;
    QUndoView *m_undoView;
    MainTableWidget *m_pTableWidget;

    QString *m_saveFileName;

    QLabel *m_tableTitle;
    QWidget *mainWidget;

    ClassHelper *m_classHelper;
    TeacherHelper *m_teacherHelper;

    QString validateString;

    int blackRowNum;
};

#endif // MAINWINDOW_H
