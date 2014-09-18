#include "editteacherdialog.h"

#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>


EditTeacherDialog::EditTeacherDialog(QString teacherName, int clickedColumn, QWidget *parent) :
    QDialog(parent)
{
    QFormLayout *formLayout = new QFormLayout;

    this->teacherNameInput = new QLineEdit;
    this->teacherNameInput->setText(teacherName);

    this->column = clickedColumn;

    formLayout->addRow( new QLabel(QString("Name: ")), this->teacherNameInput );

    QPushButton *addClassButton = new QPushButton( QString("Done") );
    QPushButton *cancelButton = new QPushButton( QString("Cancel") );
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(addClassButton);

    formLayout->addRow( buttonLayout );

    connect( addClassButton, SIGNAL(clicked()), this, SLOT(getName()) );

    connect( cancelButton, SIGNAL(clicked()), this, SLOT(cancelEdit()) );

    setLayout(formLayout);

}

EditTeacherDialog::~EditTeacherDialog()
{
    delete this->teacherNameInput;
}

void EditTeacherDialog::getName()
{
    emit editTeacherInput(this->teacherNameInput->text().toUpper(), column);
    close();
}

void EditTeacherDialog::cancelEdit()
{
    emit closeMain();
    close();
}

void EditTeacherDialog::showDialog()
{
    this->exec();
}
