#include "newteacherdialog.h"
#include "xlsxdocument.h"

#include <QFormLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringBuilder>

NewTeacherDialog::NewTeacherDialog(QWidget *parent) :
    QDialog(parent)
{
    QFormLayout *f_layout = new QFormLayout;
    this->inText = new QTextEdit;
    f_layout->addRow( new QLabel( QString("Enter the names " \
                                          "of teachers to add.\nEnter " \
                                          "each teacher separated by a comma." \
                                          ) ), inText );
    QPushButton *addTeacherButton = new QPushButton( QString("Done Adding") );
    QPushButton *addCSVFileButton = new QPushButton( QString("Add CSV File") );
    QPushButton *addExcelFileButton = new QPushButton( QString("Add Excel File") );
    QPushButton *cancelButton = new QPushButton( QString("Cancel") );

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(addCSVFileButton);
    buttonsLayout->addWidget(addExcelFileButton);
    buttonsLayout->addWidget(addTeacherButton);

    f_layout->addRow( buttonsLayout );

    setLayout(f_layout);
    inText->setFocus();

    connect( addTeacherButton, SIGNAL(clicked()), this, SLOT(getInput()) );
    connect( addCSVFileButton, SIGNAL(clicked()), this, SLOT(addCSVFile()) );
    connect( addExcelFileButton, SIGNAL(clicked()), this, SLOT(addExcelFile()) );
    connect( cancelButton, SIGNAL(clicked()), this, SLOT(cancelAdd()) );
}

NewTeacherDialog::~NewTeacherDialog()
{
    delete this->inText;
}

void NewTeacherDialog::getInput()
{
    emit newTeacherInput( this->inText );
    close();
}

void NewTeacherDialog::cancelAdd()
{
    close();
}

void NewTeacherDialog::addCSVFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Teachers File"), "", tr("Files(*.csv)") );
    QFile file(fileName);

    if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QString allText = file.readLine();
        this->inText->append(allText);
    }
}

void NewTeacherDialog::addExcelFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Teachers File"), "", tr("Files(*.xlsx)") );

    if(!fileName.isNull())
    {
        QXlsx::Document xlsxRead(fileName);
        QXlsx::CellRange range = xlsxRead.dimension();

        QString allText = xlsxRead.cellAt(1,1)->value().toString();
        for( int row=2; row<range.lastRow(); ++row ) {
            if( QXlsx::Cell *cell=xlsxRead.cellAt(row, 1) ) {
                allText = allText % ", " % cell->value().toString();
            }
        }

        this->inText->append(allText);
    }
}

void NewTeacherDialog::showDialog()
{
    this->exec();
}
