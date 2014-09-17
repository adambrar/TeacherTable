#ifndef HEADERVIEW_H
#define HEADERVIEW_H

#include <QHeaderView>

class MainTableWidget;

class HorizontalHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit HorizontalHeaderView(MainTableWidget*, QWidget *parent = 0);

    void mousePressEvent( QMouseEvent* );
    void mouseReleaseEvent( QMouseEvent* );
    void mouseMoveEvent( QMouseEvent* );

signals:
    void teacherMoved(int, int);

public slots:

private:
    MainTableWidget* m_tableWidget;
    QPoint pressedPoint;
    QPoint releasedPoint;
    int blackedOutCols;
};

#endif // HEADERVIEW_H
