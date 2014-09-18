#ifndef MAINTABLEWIDGET_H
#define MAINTABLEWIDGET_H

#include <QTableWidget>

class QUndoStack;
class QModelIndex;
class HorizontalHeaderView;

class MainTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit MainTableWidget(QWidget *parent = 0);
    ~MainTableWidget();

    void dropEvent(QDropEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void setUndoStack(QUndoStack*);
    void initTableWidget( QTableWidget* );
    void insertInstructions( QTableWidget* );

    int numBlocks() { return 8; }
    int numExtraBlocks() { return 2; }
    QSize getTableSize( QTableWidget* );

    QTableWidgetItem* emptyCell() const;

    QStringList HTableHeader() const;
    void setHTableHeader(const QStringList &HTableHeader);

    QStringList VTableHeader() const;
    void setVTableHeader(const QStringList &VTableHeader);

    QTableWidgetItem *getCopiedItem() const;
    void setCopiedItem(QTableWidgetItem *value);
    int getCopiedItemNumber() { return copiedItemNumber; }
    void increaseCopiedItemNumber() { copiedItemNumber++; }
    void decreaseCopiedItemNumber() { copiedItemNumber--; }
    void resetCopiedItemNumber(int number = 0) { number++; copiedItemNumber = number; }

    int BlackoutColumns() const { return blackoutColumns; }

    HorizontalHeaderView *getHHeaderView() const;

signals:
    void classMoved(QTableWidgetItem*, \
                    int toRow, int toCol, int fromRow, int fromCol);
public slots:

private:
    QTableWidgetItem *copiedItem;
    int copiedItemNumber;

    QUndoStack *undoStack;

    QTableWidgetItem *movingItem;
    QModelIndex fromIndex;

    QStringList m_HTableHeader;
    QStringList m_VTableHeader;

    HorizontalHeaderView *hHeaderView;
    int blackoutColumns;
};

#endif // MAINTABLEWIDGET_H
