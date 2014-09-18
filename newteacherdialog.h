#ifndef NEWTEACHERDIALOG_H
#define NEWTEACHERDIALOG_H

#include <QDialog>

class QTextEdit;

class NewTeacherDialog : public QDialog
{
    Q_OBJECT

public:
    NewTeacherDialog( QWidget *parent = 0 );
    ~NewTeacherDialog();

    void showDialog();

signals:
    void newTeacherInput( QTextEdit* );

public slots:
    void getInput();
    void addFile();
    void cancelAdd();

private:
    QTextEdit *inText;
};

#endif // NEWTEACHERDIALOG_H
