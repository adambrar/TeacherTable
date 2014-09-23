#ifndef OPTIONSEDIT_H
#define OPTIONSEDIT_H

#include <QWidget>

class MainTableOptions : public QWidget
{
    Q_OBJECT
public:
    explicit MainTableOptions(QWidget *parent = 0);

    QColor getGradeColor(QString grade = "");

    enum classDataType{ClassName, ClassGrade, ClassSection, ClassNotes, \
                       ClassRow, ClassCol, ClassHighlight};
signals:

public slots:

private:
    QList<QColor> *gradeColors;

};

#endif // OPTIONSEDIT_H
