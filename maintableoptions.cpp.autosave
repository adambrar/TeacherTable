#include "maintableoptions.h"

#include <QMenu>

MainTableOptions::MainTableOptions(QWidget *parent) :
    QWidget(parent)
{
    gradeColors = new QList<QColor>;

    gradeColors->append(QColor(255,0,0,230));
    gradeColors->append(QColor(0,255,0,230));
    gradeColors->append(QColor(51,51,255,230));
    gradeColors->append(QColor(255,0,255,230));
    gradeColors->append(QColor(255,255,0,230));
    gradeColors->append(QColor(0,0,0,230));

    this->highlightMenu = new QMenu;
    makeHighlightMenu();
}

MainTableOptions::~MainTableOptions()
{
    delete this->gradeColors;
    delete this->highlightMenu;
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
        if(grade.length() > 2)
        {
            newBackgroundColor = newBackgroundColor.lighter();
        }
    } else if(grade.contains("8")) {
        newBackgroundColor = this->gradeColors->at(0);
        if(grade.length() > 2)
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


QBrush MainTableOptions::getBrush(QColor color, int brushType) const
{
    QBrush returnBrush(color);

    switch(brushType)
    {
        case 1:
            returnBrush.setStyle(Qt::Dense4Pattern);
            break;
        case 2:
            returnBrush.setStyle(Qt::Dense5Pattern);
            break;
        case 3:
            returnBrush.setStyle(Qt::Dense6Pattern);
            break;
        case 4:
            returnBrush.setStyle(Qt::Dense7Pattern);
            break;
        default:
            returnBrush.setStyle(Qt::SolidPattern);

    }

    return returnBrush;
}

QMenu *MainTableOptions::getHighlightMenu() const
{
    return highlightMenu;
}

void MainTableOptions::makeHighlightMenu() const
{
    this->highlightMenu->addAction("None");
    this->highlightMenu->addAction("Highlight 1");
    this->highlightMenu->addAction("Highlight 2");
    this->highlightMenu->addAction("Highlight 3");
    this->highlightMenu->addAction("Highlight 4");
}
