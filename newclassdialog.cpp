#include "newclassdialog.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QIntValidator>
#include <QButtonGroup>
#include <QTextEdit>

NewClassDialog::NewClassDialog(int row, int column, QString name, \
                               QString grade, QString section, QString notes, QWidget *parent) :
    QDialog(parent)
{
    nRow = row;
    nColumn = column;

    QFormLayout *formLayout = new QFormLayout;

    classNameInput = new QLineEdit;
    classNameInput->setMaxLength(7);
    classNameInput->setText(name);

    classSectionInput = new QLineEdit;
    classSectionInput->setValidator( new QIntValidator(1, 9, this) );
    classSectionInput->setText(section);

    classAdditionalNotes = new QTextEdit;
    classAdditionalNotes->setMinimumHeight(200);
    classAdditionalNotes->setMinimumWidth(100);
    classAdditionalNotes->setText(notes);

    QHBoxLayout *gradeInputButtons = createGradeInput(grade);

    formLayout->addRow( new QLabel(QString("Subject: ")), classNameInput );
    formLayout->addRow( new QLabel(QString("Grade: ")), gradeInputButtons );
    formLayout->addRow( new QLabel(QString("Section: ")),classSectionInput );
    formLayout->addRow( new QLabel(QString("Notes: ")),classAdditionalNotes);

    QPushButton *addClassButton = new QPushButton( QString("Done") );
    QPushButton *cancelButton = new QPushButton( QString("Cancel") );
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(addClassButton);

    formLayout->addRow( buttonLayout );

    setLayout(formLayout);

    classNameInput->setFocus();

    connect( addClassButton, SIGNAL(clicked()), this, SLOT(getInput()) );
    connect( cancelButton, SIGNAL(clicked()), this, SLOT(cancelAdd()) );
}

void NewClassDialog::getInput()
{
    if( classNameInput->text() == "")
        classNameInput->setText("=======");

    QString classGradeInputString("");
    QList<QString> checkedGrades;

    foreach(QCheckBox *input, gradeInput)
    {
        if(input->isChecked())
        {
            checkedGrades.append(input->text());
        }
    }

    if(checkedGrades.size() == 1)
        classGradeInputString.append( checkedGrades.first() );
    else if(checkedGrades.size() > 1)
        classGradeInputString.append( QString("%1 - %2").arg( checkedGrades.first() ).arg( checkedGrades.last() ) );
    else if(checkedGrades.size() < 1)
        classGradeInputString.append( QString("xx - xx") );

    if(classSectionInput->text() == "")
        classSectionInput->setText("0");

    emit newClassInput( classNameInput->text().toUpper(), classGradeInputString, classSectionInput->text(), \
                        classAdditionalNotes->toPlainText(), this->nRow, this->nColumn );
    close();
}

void NewClassDialog::cancelAdd()
{
    close();
}

QHBoxLayout* NewClassDialog::createGradeInput(QString currentGrade)
{
    QHBoxLayout *boxLayout = new QHBoxLayout;

    QCheckBox *newGradeInput;
    for(int i=8; i<=12; i++)
    {
        newGradeInput = new QCheckBox(QString("%1").arg(i));

        if( currentGrade.contains( QString("%1").arg(i) ) )
            newGradeInput->setChecked(true);

        boxLayout->addWidget(newGradeInput);
        gradeInput.append(newGradeInput);
    }
    return boxLayout;
}

void NewClassDialog::showDialog()
{
    this->exec();
}
