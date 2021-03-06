#ifndef OPTIONSEDIT_H
#define OPTIONSEDIT_H

#include <QWidget>

class QMenu;

class MainTableOptions : public QWidget
{
    Q_OBJECT
public:
    explicit MainTableOptions(QWidget *parent = 0);
    ~MainTableOptions();

    QColor getGradeColor(QString grade = "");

    QMenu *getHighlightMenu() const;
    void makeHighlightMenu() const;

    enum ClassDataType{ClassName, ClassGrade, ClassSection, ClassNotes, \
                       ClassRow, ClassCol, ClassHighlight};

signals:

public slots:

private:
    QList<QColor> *gradeColors;

    QMenu *highlightMenu;

};

#endif // OPTIONSEDIT_H
