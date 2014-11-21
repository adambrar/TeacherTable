#include "edittabletitledialog.h"

#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

EditTableTitleDialog::EditTableTitleDialog(QString oldTableTitle, QWidget *parent) :
    QDialog(parent)
{
    QFormLayout *formLayout = new QFormLayout;

    this->tableTitleInput = new QLineEdit;
    this->tableTitleInput->setText(oldTableTitle);

    formLayout->addRow( new QLabel(QString("Name: ")), this->tableTitleInput );

    QPushButton *addTitleButton = new QPushButton( QString("Done") );
    QPushButton *cancelButton = new QPushButton( QString("Cancel") );
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(addTitleButton);

    formLayout->addRow( buttonLayout );

    connect( addTitleButton, SIGNAL(clicked()), this, SLOT(doneClicked()) );

    connect( cancelButton, SIGNAL(clicked()), this, SLOT(close()) );

    setLayout(formLayout);
}

EditTableTitleDialog::~EditTableTitleDialog()
{
    delete this->tableTitleInput;
}

void EditTableTitleDialog::doneClicked()
{
    QString newTitle = this->tableTitleInput->text().simplified();
    emit changeTitle(newTitle);
    close();
}

void EditTableTitleDialog::showDialog()
{
    this->exec();
}
