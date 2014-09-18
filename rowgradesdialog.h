#ifndef ROWGRADESDIALOG_H
#define ROWGRADESDIALOG_H

#include <QDialog>

class MainTableWidget;

class RowGradesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RowGradesDialog(MainTableWidget*, int, QWidget *parent = 0);
    ~RowGradesDialog();

    void showDialog();
    QList<float> countGrades();

signals:

public slots:

private:
    MainTableWidget *m_tableWidget;
    int row;

};

#endif // ROWGRADESDIALOG_H
