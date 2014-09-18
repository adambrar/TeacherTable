#ifndef EDITTEACHERDIALOG_H
#define EDITTEACHERDIALOG_H

#include <QDialog>

class QLineEdit;

class EditTeacherDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EditTeacherDialog(QString currentName, int column, QWidget *parent = 0);
    ~EditTeacherDialog();

    void showDialog();

signals:
    void editTeacherInput(QString, int);
    void closeMain();

public slots:
    void getName();
    void cancelEdit();

private:
    int column;
    QLineEdit *teacherNameInput;

};

#endif // EDITTEACHERDIALOG_H
