#include "mainwindow.h"
#include "newteacherdialog.h"
#include "newclassdialog.h"
#include "editteacherdialog.h"
#include "commandclassadd.h"
#include "commandclassdelete.h"
#include "commandclassmove.h"
#include "commandclassedit.h"
#include "commandclasspaste.h"
#include "commandteacheradd.h"
#include "commandteacherdelete.h"
#include "commandteacheredit.h"
#include "maintablewidget.h"


#include <QApplication>
#include <QDesktopWidget>
#include <QtCore/QCoreApplication>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QHeaderView>
#include <QModelIndex>
#include <QMenu>
#include <QMenuBar>
#include <QUndoStack>
#include <QUndoView>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pTableWidget(NULL)
{
    this->m_pTableWidget = new MainTableWidget(this);
    setCentralWidget(this->m_pTableWidget);

    this->m_pTableWidget->initTableWidget(m_pTableWidget);
    this->m_pTableWidget->insertInstructions( m_pTableWidget );
    this->m_pTableWidget->setUndoStack(m_undoStack);

    this->m_HTableHeader = this->m_pTableWidget->HTableHeader();

    connect( this->m_pTableWidget, SIGNAL( cellDoubleClicked (int, int) ),
             this, SLOT( cellDoubleClicked( int, int ) ) );

    connect( this->m_pTableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)),
             this, SLOT(headerSelected(int)) );

   // connect( this->m_pTableWidget->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), this

    connect( this->m_pTableWidget, SIGNAL(customContextMenuRequested(QPoint)), \
             this, SLOT(cellContextMenu(QPoint)) );

    connect( this->m_pTableWidget->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), \
             this, SLOT(headerContextMenu(QPoint)) );

    connect( this->m_pTableWidget, SIGNAL(classMoved(QTableWidgetItem*,int,int,int,int)),\
             this, SLOT(moveClass(QTableWidgetItem*,int,int,int,int)) );

    this->m_undoStack = new QUndoStack(this);
    this->m_undoView = 0;
    createMenu();

    this->m_pTableWidget->setCopiedItem( new QTableWidgetItem(QString(" \n \n ")) );
    this->m_pTableWidget->setFocus();

    this->visibleColumns = &(this->m_pTableWidget->visibleColumns);
}

MainWindow::~MainWindow()
{
    delete this->m_pTableWidget;
    delete this->m_undoStack;
}

void MainWindow::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    QMenu *viewMenu = menuBar()->addMenu("&View");
    menuBar()->addMenu("&Help");

    this->m_undoStack = new QUndoStack(this);

    fileMenu->addAction("&New...", this, SLOT(createNewTable()), QKeySequence::New);
    fileMenu->addAction("&Save...", this, SLOT(saveToFile()), QKeySequence::Save);
    fileMenu->addAction("&Open...", this, SLOT(loadFromFile()), QKeySequence::Open);
    fileMenu->addAction("&Close...", this, SLOT(closeWindow()), QKeySequence::Close);

    QAction *undoAction = m_undoStack->createUndoAction( this );
    QAction *redoAction = m_undoStack->createRedoAction( this );
    undoAction->setShortcut( QKeySequence::Undo );
    redoAction->setShortcut( QKeySequence::Redo );

    editMenu->addAction( undoAction );
    editMenu->addAction( redoAction );
    editMenu->addAction( "Delete Class", this, SLOT(deleteClass()), QKeySequence::Delete );
    editMenu->addAction( "Copy Class", this, SLOT(copyClass()), QKeySequence::Copy );
    editMenu->addAction( "Paste Class", this, SLOT(pasteClass()), QKeySequence::Paste );

    viewMenu->addAction( "Undo Stack", this, SLOT(showUndoStack()) );

}

void MainWindow::showUndoStack()
{
    if( this->m_undoView == 0 )
    {
        this->m_undoView = new QUndoView( m_undoStack );
        this->m_undoView->setWindowTitle( "Undo Stack");
        this->m_undoView->setAttribute( Qt::WA_QuitOnClose, false );
    }
    this->m_undoView->show();
}

void MainWindow::cellContextMenu(QPoint point)
{
    if( point.x() > ( this->m_pTableWidget->columnWidth(0)*(this->m_pTableWidget->columnCount()-1) ) )
    {
        return;
    }

    QMenu menu;
    QAction* viewAction;
    QAction* editAction;
    QAction* deleteAction;
    QAction* copyAction;
    QAction* pasteAction;

    QTableWidgetItem *clickedItem = this->m_pTableWidget->itemAt(point);

    if( (clickedItem->data(Qt::UserRole).isNull() || clickedItem == 0) )
    {
        if(this->m_pTableWidget->getCopiedItem()->text() != QString(" \n \n "))
        {
            pasteAction = menu.addAction("&Paste");
        } else {
            return;
        }
    } else {
        viewAction = menu.addAction("&Notes");
        editAction = menu.addAction("&Edit");
        deleteAction = menu.addAction("&Delete");
        copyAction = menu.addAction("&Copy");
        pasteAction = menu.addAction("&Paste");
    }

    QAction *clickedAction = menu.exec( QCursor::pos() );

    if ( clickedAction == editAction ) {
        QList<QVariant> data = clickedItem->data(Qt::UserRole).toList();

        NewClassDialog *editWindow = new NewClassDialog(clickedItem->row(), \
                                                       clickedItem->column(), \
                                                       data.at(0).toString(), \
                                                       data.at(1).toString(), \
                                                       data.at(2).toString(), \
                                                       data.at(3).toString(), \
                                                       this);

        connect( editWindow , SIGNAL(newClassInput(QString,QString,QString,QString, int, int)), \
                 this, SLOT(editClass(QString,QString,QString,QString, int, int)) );

        editWindow->setWindowTitle("Edit Class");
        editWindow->showDialog();

    } else if ( clickedAction == deleteAction ) {
        this->m_undoStack->push( new CommandClassDelete(clickedItem->row(), \
                                                  clickedItem->column(), \
                                                  clickedItem, this->m_pTableWidget) );
    } else if ( clickedAction == viewAction ) {
        QString displayedMessage = clickedItem->data(Qt::UserRole).toList().at(3).toString();

        QMessageBox::information( this, "Class Notes", displayedMessage );
    } else if ( clickedAction == copyAction ) {
        copyClass();
    } else if ( clickedAction == pasteAction ) {
        pasteClass();
    }
}

void MainWindow::headerContextMenu(QPoint point)
{
    if( point.x() > ( this->m_pTableWidget->columnWidth(0)*(*this->visibleColumns) ) )\
    {
        return;
    }

    QTableWidgetItem *item = this->m_pTableWidget->itemAt(point);
    int clickedColumn = item->column();
    QString headerName = this->m_HTableHeader.at(clickedColumn);

    headerName = headerName.remove("\n");

    QMenu menu;
    QAction* editAction = menu.addAction("&Edit");
    QAction* deleteAction = menu.addAction("&Delete");

    QAction *clickedAction = menu.exec( QCursor::pos() );

    if ( clickedAction == editAction )
    {
        EditTeacherDialog *editDialog = new EditTeacherDialog(headerName, clickedColumn);

        connect( editDialog, SIGNAL(editTeacherInput(QString,int)), this, SLOT(editTeacher(QString,int)) );

        editDialog->move(point);
        editDialog->showDialog();


    } else if ( clickedAction == deleteAction ) {
        m_undoStack->push( new CommandTeacherDelete(this->m_pTableWidget, &this->m_HTableHeader, \
                                                    this->visibleColumns, clickedColumn) );
    }

}

void MainWindow::cellDoubleClicked(int nRow, int nCol)
{
    NewClassDialog *newClass;
    QTableWidgetItem *clickedItem = this->m_pTableWidget->item(nRow, nCol);

    if(clickedItem->text() == " \n \n ")
    {
        newClass = new NewClassDialog(nRow, nCol);
    } else {
        QList<QVariant> data = clickedItem->data(Qt::UserRole).toList();
        newClass = new NewClassDialog(clickedItem->row(), \
                                       clickedItem->column(), \
                                       data.at(0).toString(), \
                                       data.at(1).toString(), \
                                       data.at(2).toString(), \
                                       data.at(3).toString());

    }

    connect( newClass, SIGNAL(newClassInput(QString,QString,QString,QString, int, int)), \
             this, SLOT(setClass(QString,QString,QString,QString, int, int)) );

    newClass->setWindowTitle("Add A Class");
    newClass->move(geometry().center().x()-newClass->geometry().width()/2, geometry().center().y()-newClass->geometry().height()/2);
    newClass->showDialog();
}

void MainWindow::headerSelected(int index)
{
    int visualIndex = this->m_pTableWidget->horizontalHeader()->visualIndex(index);
    if(visualIndex == this->m_pTableWidget->columnCount() - 1)
    {
        NewTeacherDialog *newTeach = new NewTeacherDialog;
        connect( newTeach, SIGNAL(newTeacherInput(QTextEdit*)), this, \
                 SLOT(createNewTeachers(QTextEdit*)) );


        newTeach->setWindowTitle("Add teachers");
        newTeach->move(geometry().center().x()-newTeach->geometry().width()/2, geometry().center().y()-newTeach->geometry().height()/2);

        newTeach->showDialog();
    }
}

void MainWindow::createNewTeachers(QTextEdit *inText)
{
    QString input = inText->toPlainText().toUpper();
    if(input.length() < 1) { return; }

    QStringList teacherNames = input.split( "," );

    (*this->visibleColumns) += teacherNames.size();

    this->m_undoStack->push( new CommandTeacherAdd(&teacherNames, &this->m_HTableHeader, this->m_pTableWidget) );
}

void MainWindow::editTeacher(QString headerAfter, int column)
{
    this->m_undoStack->push( new CommandTeacherEdit(column, headerAfter, this->m_pTableWidget, &this->m_HTableHeader));
}

void MainWindow::createNewTable()
{
    this->m_pTableWidget->setColumnCount(1);
    this->m_pTableWidget->HTableHeader().clear();
    this->m_pTableWidget->HTableHeader()<<"A\nd\nd\n \nT\ne\na\nc\nh\ne\nr";

    this->m_pTableWidget->insertInstructions(this->m_pTableWidget);

    this->m_HTableHeader.clear();
    this->m_HTableHeader = this->m_pTableWidget->HTableHeader();
    this->m_pTableWidget->setHorizontalHeaderLabels(this->m_HTableHeader);

    this->m_undoStack->clear();
}

void MainWindow::setClass(QString name, QString grade, \
                          QString section, QString notes, int nRow, int nColumn)
{
    QList<QVariant> *newData = new QList<QVariant>;
    newData->append(QVariant(name));
    newData->append(QVariant(grade));
    newData->append(QVariant(section));
    newData->append(QVariant(notes));
    newData->append(QVariant(nRow));
    newData->append(QVariant(nColumn));

    QVariant *dataToAdd = new QVariant(*newData);

    QTableWidgetItem *newItem = new QTableWidgetItem( QString("%1\n%2\n00%3").arg(name).arg(grade).arg(section) );
    newItem->setTextAlignment(Qt::AlignCenter);
    newItem->setData(Qt::UserRole, *dataToAdd);

    this->m_undoStack->push( new CommandClassAdd(nRow, nColumn, newItem, m_pTableWidget) );
}

void MainWindow::deleteClass()
{
    this->m_undoStack->push( new CommandClassDelete( \
                           this->m_pTableWidget->currentRow(), \
                           this->m_pTableWidget->currentColumn(), \
                           this->m_pTableWidget->currentItem(), \
                           this->m_pTableWidget) );
}

void MainWindow::moveClass(QTableWidgetItem *movedItem, \
                           int toRow, int toCol, \
                           int fromRow, int fromCol)
{
    if(movedItem->text() != QString(" \n \n "))
    {
        m_undoStack->push( new CommandClassMove( \
                        toRow, toCol, \
                        fromRow, fromCol, \
                        this->m_pTableWidget, movedItem ) );
    }

}

void MainWindow::editClass(QString name, QString grade, \
                           QString section, QString notes, int nRow, int nColumn)
{
    QList<QVariant> *newData = new QList<QVariant>;
    newData->append(QVariant(name));
    newData->append(QVariant(grade));
    newData->append(QVariant(section));
    newData->append(QVariant(notes));
    newData->append(QVariant(nRow));
    newData->append(QVariant(nColumn));

    QVariant *dataToAdd = new QVariant(*newData);

    QTableWidgetItem *newItem = new QTableWidgetItem( QString("%1\n%2\n00%3").arg(name).arg(grade).arg(section) );
    newItem->setTextAlignment(Qt::AlignCenter);
    newItem->setData(Qt::UserRole, *dataToAdd);

    this->m_undoStack->push( new CommandClassEdit(nRow, nColumn, newItem, this->m_pTableWidget) );
}

void MainWindow::copyClass()
{
    this->m_pTableWidget->resetCopiedItemNumber();
    this->m_pTableWidget->setCopiedItem( this->m_pTableWidget->currentItem() );
}

void MainWindow::pasteClass()
{
    if(this->m_pTableWidget->getCopiedItem()->text() == " \n \n ")
        return;
    else
        this->m_undoStack->push( new CommandClassPaste(this->m_pTableWidget->currentRow(), \
                                                       this->m_pTableWidget->currentColumn(), \
                                                       this->m_pTableWidget) );
}

QSize MainWindow::getWindowSize()
{
    return this->m_pTableWidget->getTableSize(this->m_pTableWidget);
}

void MainWindow::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        QString("Save TimeTable"), "",
        QString("TimeTable(*.ttl);;All Files (*)"));

    if( !fileName.endsWith(".ttl") )
        fileName.append(QString(".ttl"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        this->m_undoStack->clear();

        QDataStream out(&file);

        //table size
        qint32 numRows = this->m_pTableWidget->rowCount();
        qint32 numCols = this->m_pTableWidget->columnCount();
        qint32 visibleColStore = *this->visibleColumns;
        out << numRows << numCols << visibleColStore;

        out << this->m_HTableHeader << this->m_VTableHeader;

        //table items by row then column
        for (int i=0; i<numRows; ++i)
        {
            for (int j=0; j<numCols; j++)
            {
              this->m_pTableWidget->item(i,j)->write(out);
            }
        }

        file.close();
    }
}

void MainWindow::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open TimeTable"), "",
        tr("TimeTable (*.ttl);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_5);

        createNewTable();

        qint32 numRows, numCols, numVisibleCols;
        in >> numRows >> numCols >> numVisibleCols;

        *this->visibleColumns = numVisibleCols;

        this->m_pTableWidget->setRowCount(numRows);
        this->m_pTableWidget->setColumnCount(numCols);

        for (int row = 0; row < m_pTableWidget->rowCount(); row++)
        {
            QTableWidgetItem *newItem = this->m_pTableWidget->item(row, 0);
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsDropEnabled);
        }


        for(int i=0; i < this->m_pTableWidget->columnCount() - 1; i++)
        {
            this->m_pTableWidget->horizontalHeader()->swapSections(i, i+1);
        }

        QStringList inHTableHeader, inVTableHeader;

        in >> inHTableHeader >> inVTableHeader;
        this->m_HTableHeader = inHTableHeader;
        this->m_VTableHeader = inVTableHeader;

        this->m_pTableWidget->setHorizontalHeaderLabels(this->m_HTableHeader);
        this->m_pTableWidget->setVerticalHeaderLabels(this->m_VTableHeader);

        for (int i = 0; i < numRows; ++i)
        {
           for (int j = 0; j < numCols; j++)
           {
               QTableWidgetItem *item = new QTableWidgetItem;
               item->read(in);
               this->m_pTableWidget->setItem(i, j, item);
           }
        }

        int lastColumn = 0;

        for (int row = 0; row < m_pTableWidget->rowCount(); row++)
        {
            QTableWidgetItem *newItem = this->m_pTableWidget->item(row, lastColumn)->clone();
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsDropEnabled \
                              & ~Qt::ItemIsEnabled & ~Qt::ItemIsDragEnabled);
            this->m_pTableWidget->setItem(row, lastColumn, newItem);
        }

        file.close();
    }
}

void MainWindow::closeWindow()
{
    this->m_undoStack->clear();

    this->close();
}
