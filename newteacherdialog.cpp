#include "newteacherdialog.h"

#include <QFormLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

NewTeacherDialog::NewTeacherDialog(QWidget *parent) :
    QDialog(parent)
{
    QFormLayout *f_layout = new QFormLayout;
    inText = new QTextEdit;
    f_layout->addRow( new QLabel( QString("Enter the names " \
                                          "of teachers to add.\nEnter " \
                                          "each teacher separated by a comma." \
                                          ) ), inText );
    QPushButton *addTeacherButton = new QPushButton( QString("Done Adding") );
    QPushButton *addFileButton = new QPushButton( QString("Add .csv File") );
    QPushButton *cancelButton = new QPushButton( QString("Cancel") );

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(addFileButton);
    buttonsLayout->addWidget(addTeacherButton);

    f_layout->addRow( buttonsLayout );

    setLayout(f_layout);
    addTeacherButton->setFocus();

    connect( addTeacherButton, SIGNAL(clicked()), this, SLOT(getInput()) );
    connect( addFileButton, SIGNAL(clicked()), this, SLOT(addFile()) );
    connect( cancelButton, SIGNAL(clicked()), this, SLOT(cancelAdd()) );
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

void NewTeacherDialog::addFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Teachers File"), "", tr("Files(*.csv)") );
    QFile file(fileName);

    if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QString allText = file.readLine();
        this->inText->append(allText);
    }
}

void NewTeacherDialog::showDialog()
{
    this->exec();
}
