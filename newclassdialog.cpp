#include "newclassdialog.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QIntValidator>
#include <QButtonGroup>
#include <QTextEdit>
#include <QTableWidgetItem>

NewClassDialog::NewClassDialog(int row, int column, QString name, \
                               QString grade, QString section, QString notes, QWidget *parent) :
    QDialog(parent)
{
    nRow = row;
    nColumn = column;

    QFormLayout *formLayout = new QFormLayout;

    this->classNameInput = new QLineEdit;
    this->classNameInput->setMaxLength(7);
    if(name != "=======")
        this->classNameInput->setText(name);

    this->classSectionInput = new QLineEdit;
    this->classSectionInput->setValidator( new QIntValidator(1, 9, this) );
    this->classSectionInput->setText(section);

    this->classAdditionalNotes = new QTextEdit;
    this->classAdditionalNotes->setMinimumHeight(200);
    this->classAdditionalNotes->setMinimumWidth(100);
    this->classAdditionalNotes->setText(notes);

    QHBoxLayout *gradeInputButtons = createGradeInput(grade);

    formLayout->addRow( new QLabel(QString("Subject: ")), this->classNameInput );
    formLayout->addRow( new QLabel(QString("Grade: ")), gradeInputButtons );
    formLayout->addRow( new QLabel(QString("Section: ")), this->classSectionInput );
    formLayout->addRow( new QLabel(QString("Notes: ")), this->classAdditionalNotes);

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

NewClassDialog::~NewClassDialog()
{
    delete this->classNameInput;
    delete this->classSectionInput;
    delete this->classAdditionalNotes;
}

void NewClassDialog::getInput()
{
    if( classNameInput->text() == "")
        classNameInput->setText("=======");
    else
        classNameInput->setText(classNameInput->text().simplified());

    QString *classGradeData = new QString;
    QString classGradeString = this->getGradeString(classGradeData);

    if(classSectionInput->text() == "")
        classSectionInput->setText("0");

    QTableWidgetItem* newItem = createClass(classNameInput->text().toUpper(), classGradeString, *classGradeData, \
                                            classSectionInput->text(), classAdditionalNotes->toPlainText());

    emit newClassInput( newItem, this->nRow, this->nColumn );
    close();
}

QTableWidgetItem* NewClassDialog::createClass(QString name, QString gradeString, \
                                              QString gradeData, QString section, QString notes)
{
    QList<QVariant> *newData = new QList<QVariant>;
    newData->append(QVariant(name));
    newData->append(QVariant(gradeData));
    newData->append(QVariant(section));
    newData->append(QVariant(notes));
    newData->append(QVariant(nRow));
    newData->append(QVariant(nColumn));
    newData->append((QVariant)QString("None"));

    QVariant *dataToAdd = new QVariant(*newData);
    QTableWidgetItem *newItem;

    if(notes == "")
    {
        newItem = new QTableWidgetItem( \
                QString("%1\n%2\n00%3").arg(name).arg(gradeString).arg(section) );
    } else {
        newItem = new QTableWidgetItem( \
                QString("%1\n%2\n** 00%3 **").arg(name).arg(gradeString).arg(section) );
    }
    newItem->setTextAlignment(Qt::AlignCenter);
    newItem->setData(Qt::UserRole, *dataToAdd);

    return newItem;
}

void NewClassDialog::cancelAdd()
{
    close();
}

QString NewClassDialog::getGradeString(QString *classGradeData)
{
    QString returnString;

    foreach(QCheckBox *input, gradeInput)
    {
        if(input->isChecked())
        {
            classGradeData->append( QString("%1 ").arg(input->text()) );
        }
    }

    QStringList gradeString = classGradeData->split(" ", QString::SkipEmptyParts);

    if(gradeString.size() == 1)
        returnString.append( gradeString.first() );
    else if(gradeString.size() > 1)
        returnString.append( QString("%1 - %2").arg( gradeString.first() ).arg( gradeString.last() ) );
    else if(gradeString.size() < 1)
        returnString.append( QString("xx - xx") );

    return returnString;
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
