#ifndef EDITTABLETITLEDIALOG_H
#define EDITTABLETITLEDIALOG_H

#include <QDialog>

class QLineEdit;

class EditTableTitleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EditTableTitleDialog(QString oldTableTitle, QWidget *parent = 0);
    ~EditTableTitleDialog();

    void showDialog();

signals:
    void changeTitle(QString newTitle);
public slots:
    void doneClicked();

private:
    QLineEdit *tableTitleInput;

};

#endif // EDITTABLETITLEDIALOG_H
