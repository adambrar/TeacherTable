#ifndef NEWCLASSDIALOG_H
#define NEWCLASSDIALOG_H

#include <QDialog>

class QLineEdit;
class QHBoxLayout;
class QCheckBox;
class QTextEdit;
class QTableWidgetItem;

class NewClassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewClassDialog(int, int, QString name = "", QString grade = "", \
                            QString section = "", QString notes = "", QWidget *parent = 0);
    ~NewClassDialog();

    int getRow() { return nRow; }
    int getColumn() { return nColumn; }
    void showDialog();
    QString getGradeString(QString *);
    QTableWidgetItem* createClass(QString, QString, QString, QString, QString);


signals:
    void newClassInput( QTableWidgetItem*, int, int );
    
public slots:
    void getInput();
    void cancelAdd();

private:
    QHBoxLayout* createGradeInput(QString);

    QLineEdit *classNameInput;
    QList<QCheckBox*> gradeInput;
    QLineEdit *classGradeInput;
    QLineEdit *classSectionInput;
    QTextEdit *classAdditionalNotes;

    int nRow;
    int nColumn;
};

#endif // NEWCLASSDIALOG_H
