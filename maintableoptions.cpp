#include "maintableoptions.h"

MainTableOptions::MainTableOptions(QWidget *parent) :
    QWidget(parent)
{
    gradeColors = new QList<QColor>;

    gradeColors->append(QColor(255,0,0,230));
    gradeColors->append(QColor(0,255,0,230));
    gradeColors->append(QColor(51,51,255,255));
    gradeColors->append(QColor(255,0,255,230));
    gradeColors->append(QColor(255,255,0,230));
    gradeColors->append(QColor(0,0,0,230));
}

QColor MainTableOptions::getGradeColor(QString grade)
{
    QColor newBackgroundColor = QColor(255,255,255);

    if(grade.contains("12")) {
        newBackgroundColor = this->gradeColors->at(4);
        if(grade.length() > 3)
        {
            newBackgroundColor = newBackgroundColor.lighter();
        }
    } else if(grade.contains("11")) {
        newBackgroundColor = this->gradeColors->at(3);
        if(grade.length() > 3)
        {
            newBackgroundColor = newBackgroundColor.lighter();
        }
    } else if(grade.contains("10")) {
        newBackgroundColor = this->gradeColors->at(2);
        if(grade.length() > 3)
        {
            newBackgroundColor = newBackgroundColor.lighter();
        }
    } else if(grade.contains("9")) {
        newBackgroundColor = this->gradeColors->at(1);
        if(grade.length() > 3)
        {
            newBackgroundColor = newBackgroundColor.lighter();
        }
    } else if(grade.contains("8")) {
        newBackgroundColor = this->gradeColors->at(0);
        if(grade.length() > 3)
        {
            newBackgroundColor = newBackgroundColor.lighter();
        }
    } else if(grade == "xx - xx") {
        newBackgroundColor = QColor(255,255,255);
    } else {
        newBackgroundColor = QColor(255,255,255);
    }

    return newBackgroundColor;
}
