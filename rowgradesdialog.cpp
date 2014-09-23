#include "rowgradesdialog.h"
#include "maintablewidget.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

RowGradesDialog::RowGradesDialog(MainTableWidget *m_pTableWidget, int clickedRow, QWidget *parent) :
    QDialog(parent)
{
    this->m_tableWidget = m_pTableWidget;
    this->row = clickedRow;

    QFormLayout *f_layout = new QFormLayout;

    QList<float> count = countGrades();

    f_layout->addRow( new QLabel( QString("Grade 8: ") ), new QLabel(QString("%1").arg(count.at(0)) ) );
    f_layout->addRow( new QLabel( QString("Grade 9: ") ), new QLabel(QString("%1").arg(count.at(1)) ) );
    f_layout->addRow( new QLabel( QString("Grade 10: ") ), new QLabel(QString("%1").arg(count.at(2)) ) );
    f_layout->addRow( new QLabel( QString("Grade 11: ") ), new QLabel(QString("%1").arg(count.at(3)) ) );
    f_layout->addRow( new QLabel( QString("Grade 12: ") ), new QLabel(QString("%1").arg(count.at(4)) ) );

    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    QPushButton *doneButton = new QPushButton( QString("Done") );
    buttonsLayout->addWidget(doneButton);

    f_layout->addRow( buttonsLayout );

    connect( doneButton, SIGNAL(clicked()), this, SLOT(close()) );

    this->setLayout(f_layout);
}

RowGradesDialog::~RowGradesDialog()
{
}

QList<float> RowGradesDialog::countGrades()
{
    QList<float> results = QList<float>();

    results<<0<<0<<0<<0<<0;

    for(int i=this->m_tableWidget->BlackoutColumns(); i<this->m_tableWidget->columnCount(); i++)
    {
        QTableWidgetItem *currentItem = this->m_tableWidget->item(this->row, i);

        if(currentItem->data(Qt::UserRole).toStringList().isEmpty())
            continue;

        QString grades = currentItem->data(Qt::UserRole).toStringList().at(MainTableOptions::ClassGrade);
        float valueAdded = 1;
        if(grades.size() > 2)
            valueAdded = 0.5;

        if(grades.contains("8"))
            results.replace(0, results.at(0)+valueAdded);
        if(grades.contains("9"))
            results.replace(1, results.at(1)+valueAdded);
        if(grades.contains("10"))
            results.replace(2, results.at(2)+valueAdded);
        if(grades.contains("11"))
            results.replace(3, results.at(3)+valueAdded);
        if(grades.contains("12"))
            results.replace(4, results.at(4)+valueAdded);
    }

    return results;
}

void RowGradesDialog::showDialog()
{
    this->exec();
}
