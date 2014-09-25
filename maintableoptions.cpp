#include "maintableoptions.h"

#include <QMenu>

MainTableOptions::MainTableOptions(QWidget *parent) :
    QWidget(parent)
{
    gradeColors = new QList<QColor>;

    gradeColors->append(QColor(204,86,40));
    gradeColors->append(QColor(153,98,76));
    gradeColors->append(QColor(255,25,32));
    gradeColors->append(QColor(92,255,89));
    gradeColors->append(QColor(101,204,40));
    gradeColors->append(QColor(0,0,0));

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
    int factor(130);

    if(grade.contains("12")) {
        newBackgroundColor = this->gradeColors->at(4);
        if(grade.length() > 3)
        {
            newBackgroundColor = newBackgroundColor.lighter(factor);
        }
    } else if(grade.contains("11")) {
        newBackgroundColor = this->gradeColors->at(3);
        if(grade.length() > 3)
        {
            newBackgroundColor = newBackgroundColor.lighter(factor);
        }
    } else if(grade.contains("10")) {
        newBackgroundColor = this->gradeColors->at(2);
        if(grade.length() > 3)
        {
            newBackgroundColor = newBackgroundColor.lighter(factor);
        }
    } else if(grade.contains("9")) {
        newBackgroundColor = this->gradeColors->at(1);
        if(grade.length() > 2)
        {
            newBackgroundColor = newBackgroundColor.lighter(factor);
        }
    } else if(grade.contains("8")) {
        newBackgroundColor = this->gradeColors->at(0);
        if(grade.length() > 2)
        {
            newBackgroundColor = newBackgroundColor.lighter(factor);
        }
    } else if(grade == "xx - xx") {
        newBackgroundColor = QColor(255,255,255);
    } else {
        newBackgroundColor = QColor(255,255,255);
    }

    return newBackgroundColor;
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
