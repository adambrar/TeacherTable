#ifndef HEADERVIEW_H
#define HEADERVIEW_H

#include <QHeaderView>

class MainTableWidget;

class HorizontalHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit HorizontalHeaderView(MainTableWidget*, QWidget *parent = 0);
    ~HorizontalHeaderView();

    void mousePressEvent( QMouseEvent* );
    void mouseReleaseEvent( QMouseEvent* );
    void mouseMoveEvent( QMouseEvent* );

signals:
    void teacherMoved(int, int);

public slots:

private:
    MainTableWidget* m_tableWidget;
    QPoint pressedPoint;
    int pressedLogicalIndex;
    int pressedVisualIndex;
    int blackedOutCols;
};

#endif // HEADERVIEW_H
