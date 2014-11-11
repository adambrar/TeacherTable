#include "mainwindow.h"
#include "newteacherdialog.h"
#include "newclassdialog.h"
#include "editteacherdialog.h"
#include "commandclassadd.h"
#include "commandclassdelete.h"
#include "commandclassmove.h"
#include "commandclassedit.h"
#include "commandclasspaste.h"
#include "commandclasshighlight.h"
#include "commandteacheradd.h"
#include "commandteacherdelete.h"
#include "commandteacheredit.h"
#include "commandteachermove.h"
#include "horizontalheaderview.h"
#include "highlightitemdelegate.h"
#include "maintablewidget.h"
#include "rowgradesdialog.h"


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
#include <QStringList>
#include <QLabel>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QDate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pTableWidget(NULL)
{
    this->m_pTableWidget = new MainTableWidget(this);
    this->setCentralWidget(this->m_pTableWidget);

    this->m_pTableWidget->initTableWidget(m_pTableWidget);
    this->m_pTableWidget->insertInstructions( m_pTableWidget );
    this->m_pTableWidget->setUndoStack(m_undoStack);

    this->setMinimumHeight( this->m_pTableWidget->getTableSize(this->m_pTableWidget).height() );

    this->m_HTableHeader = this->m_pTableWidget->HTableHeader();

    connect( this->m_pTableWidget, SIGNAL( cellDoubleClicked (int, int) ),
             this, SLOT( cellDoubleClicked( int, int ) ) );

    connect( this->m_pTableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)),
             this, SLOT(hHeaderSelected(int)) );

    connect( this->m_pTableWidget, SIGNAL(customContextMenuRequested(QPoint)), \
             this, SLOT(cellContextMenu(QPoint)) );

    connect( this->m_pTableWidget->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), \
             this, SLOT(headerContextMenu(QPoint)) );

    connect( this->m_pTableWidget, SIGNAL(classMoved(QTableWidgetItem*,int,int,int,int)),\
             this, SLOT(moveClass(QTableWidgetItem*,int,int,int,int)) );

    connect( this->m_pTableWidget->getHHeaderView(), SIGNAL(teacherMoved(int,int)), this, SLOT(moveTeacher(int,int)) );

    connect( this->m_pTableWidget->verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(vHeaderSelected(int)) );

    this->m_undoStack = new QUndoStack(this);
    this->m_undoView = 0;
    createMenu();

    this->m_pTableWidget->setCopiedItem( new QTableWidgetItem(QString(" \n \n ")) );
    this->m_pTableWidget->setFocus();

    this->m_saveFileName = new QString("");
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
    QMenu *helpMenu = menuBar()->addMenu("&Help");

    this->m_undoStack = new QUndoStack(this);

    fileMenu->addAction("&New...", this, SLOT(createNewTable()), QKeySequence::New);
    fileMenu->addSeparator();
    fileMenu->addAction("&Save...", this, SLOT(saveToFile()), QKeySequence::Save);
    fileMenu->addAction("Save &As...", this, SLOT(saveAsToFile()), QKeySequence::SaveAs);
    fileMenu->addAction("&Open...", this, SLOT(loadFromFile()), QKeySequence::Open);
    fileMenu->addSeparator();
    fileMenu->addAction("&Print Table...", this, SLOT(printTable()), QKeySequence::Print);
    fileMenu->addAction("&Export as PDF...", this, SLOT(tableToPDF()));
    fileMenu->addSeparator();
    fileMenu->addAction("&Quit...", this, SLOT(closeWindow()), QKeySequence::Close);

    QAction *undoAction = m_undoStack->createUndoAction( this );
    QAction *redoAction = m_undoStack->createRedoAction( this );
    undoAction->setShortcut( QKeySequence::Undo );
    redoAction->setShortcut( QKeySequence::Redo );

    editMenu->addAction( undoAction );
    editMenu->addAction( redoAction );
    editMenu->addSeparator();
    editMenu->addAction( "&Edit Class", this, SLOT(editClassDialog()) );
    editMenu->addAction( "&Copy Class", this, SLOT(copyClass()), QKeySequence::Copy );
    editMenu->addAction( "&Paste Class", this, SLOT(pasteClass()), QKeySequence::Paste );
    editMenu->addSeparator();
    editMenu->addAction( "&Delete Class", this, SLOT(deleteClass()), QKeySequence::Delete );

    viewMenu->addAction( "&Show Notes", this, SLOT(showClass()) );

    helpMenu->addAction( "&Instructions", this, SLOT(showHelp()), QKeySequence::HelpContents );
    editMenu->addSeparator();
    helpMenu->addAction( "&About", this, SLOT(showQtHelp()) );

}

void MainWindow::printTable()
{
    if(this->m_pTableWidget->columnCount() < 2)
    {
        QMessageBox::information(this, "Ooops!", "Nothing to print.");
        return;
    }

    QPrinter printer;
    printer.setPageOrientation(QPageLayout::Landscape);

    QList<MainTableWidget*> tempTables = \
            this->m_pTableWidget->createPrintableTable(printer.pageRect().width());

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));

    if (dialog->exec() != QDialog::Accepted)
        return;

    QString date = QDate::currentDate().toString("dddd dd, MMMM, yyyy");
    QLabel dateLabel(date);
    QPixmap grabDateLabel = dateLabel.grab();

    QPainter painter;
    painter.begin(&printer);
    for(int i=0;i<tempTables.size();i++)
    {
        QPixmap grabMap = tempTables.at(i)->grab();

        QLabel pageLabel(QString("Page %1 of %2").arg(i+1).arg(tempTables.size()));
        QPixmap grabPageLabel = pageLabel.grab();

        painter.drawPixmap(10, 0, tempTables.at(i)->width(), \
                           printer.pageRect().height()-grabPageLabel.height(), grabMap);

        painter.drawPixmap(0, printer.pageRect().height()-grabPageLabel.height(), \
                           grabPageLabel.width(), grabPageLabel.height(), grabPageLabel);

        painter.drawPixmap(printer.pageRect().width()-grabDateLabel.width(), \
                           printer.pageRect().height()-grabDateLabel.height(), \
                           grabDateLabel.width(), grabPageLabel.height(), grabDateLabel);

        if(i != tempTables.size()-1)
            printer.newPage();
    }
    painter.end();

    QMessageBox::information(this, "Done!", "Finished printing table.");
}

void MainWindow::tableToPDF()
{
    if(this->m_pTableWidget->columnCount() < 2)
    {
        QMessageBox::information(this, "Ooops!", "Nothing to print.");
        return;
    }

    QString saveFile = QFileDialog::getSaveFileName(this, QString("Save TimeTable"), \
                                                    "", QString("PDF(*.pdf)") );

    if(saveFile.length() < 1)
        return;

    QPrinter printer;
    printer.setOutputFileName(saveFile);
    printer.setPageOrientation(QPageLayout::Landscape);

    QList<MainTableWidget*> tempTables = this->m_pTableWidget-> \
                                    createPrintableTable(printer.pageRect().width());

    QString date = QDate::currentDate().toString("dddd dd, MMMM, yyyy");
    QLabel dateLabel(date);
    QPixmap grabDateLabel = dateLabel.grab();

    QPainter painter;
    painter.begin(&printer);
    for(int i=0;i<tempTables.size();i++)
    {
        QPixmap grabMap = tempTables.at(i)->grab();

        QLabel pageLabel(QString("Page %1 of %2").arg(i+1).arg(tempTables.size()));
        QPixmap grabPageLabel = pageLabel.grab();

        painter.drawPixmap(10, 0, tempTables.at(i)->width(), \
                           printer.pageRect().height()-grabPageLabel.height(), grabMap);

        painter.drawPixmap(0, printer.pageRect().height()-grabPageLabel.height(), \
                           grabPageLabel.width(), grabPageLabel.height(), grabPageLabel);

        painter.drawPixmap(printer.pageRect().width()-grabDateLabel.width(), \
                           printer.pageRect().height()-grabDateLabel.height(), \
                           grabDateLabel.width(), grabPageLabel.height(), grabDateLabel);

        if(i != tempTables.size()-1)
            printer.newPage();
    }
    painter.end();

    QMessageBox::information(this, "Done!", "Finished exporting table.");
}

void MainWindow::cellContextMenu(QPoint point)
{
    QTableWidgetItem *clickedItem = this->m_pTableWidget->itemAt(point);

    QMenu menu;
    QAction* viewAction;
    QAction* editAction;
    QAction* deleteAction;
    QAction* copyAction;
    QAction* pasteAction;
    QList<QAction *> highlighterActions;

    //if on inaccessible item return
    if( this->m_pTableWidget->visualColumn(clickedItem->column()) < this->m_pTableWidget->BlackoutColumns() || \
            clickedItem->text() == QString(" \n \n ") )
    {   
        //unless able to paste then offer paste action
        if( this->m_pTableWidget->getCopiedItem()->text() != QString(" \n \n ") )
        {
            pasteAction = menu.addAction("&Paste");
        } else {
            return;
        }

    //otherwise offer all actions
    } else {
        QMenu *highlightMenu = new QMenu("Highlighters");

        highlighterActions = \
                this->m_pTableWidget->TableOptions()->getHighlightMenu()->actions();

        foreach(QAction *action, highlighterActions)
            highlightMenu->addAction(action);

        menu.addMenu(highlightMenu);
        menu.addSeparator();
        viewAction = menu.addAction("&Notes");
        menu.addSeparator();
        editAction = menu.addAction("&Edit");
        copyAction = menu.addAction("&Copy");
        pasteAction = menu.addAction("&Paste");
        menu.addSeparator();
        deleteAction = menu.addAction("&Delete");
    }

    QAction *clickedAction = menu.exec( QCursor::pos() );

    if ( clickedAction == editAction ) {
        editClassDialog();

    } else if ( clickedAction == deleteAction ) {
        this->m_undoStack->push( new CommandClassDelete(clickedItem->row(), \
                                                  clickedItem->column(), \
                                                  clickedItem, this->m_pTableWidget) );
    } else if ( clickedAction == viewAction ) {
        showClass();
    } else if ( clickedAction == copyAction ) {
        copyClass();
    } else if ( clickedAction == pasteAction ) {
        pasteClass();
    } else if ( highlighterActions.contains(clickedAction) ) {
        QString actionText = clickedAction->text();
        highlightClass( clickedItem->row(), clickedItem->column(), \
                        actionText );
    }
}

void MainWindow::headerContextMenu(QPoint point)
{
    QTableWidgetItem *clickedItem = this->m_pTableWidget->itemAt(point);

    if( clickedItem == 0 || this->m_pTableWidget->visualColumn(clickedItem->column()) < \
                                                            this->m_pTableWidget->BlackoutColumns() )
        return;

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
        this->m_undoStack->clear();
        this->m_pTableWidget->removeColumn(clickedColumn);
        this->m_HTableHeader.removeAt(clickedColumn);
    }
}

void MainWindow::cellDoubleClicked(int nRow, int nCol)
{
    NewClassDialog *newClass;
    QTableWidgetItem *clickedItem = this->m_pTableWidget->item(nRow, nCol);

    if(clickedItem->text() == " \n \n ")
    {
        newClass = new NewClassDialog(nRow, nCol);
        newClass->setWindowTitle("Add A Class");
        connect( newClass, SIGNAL(newClassInput(QTableWidgetItem*, int, int)), \
                 this, SLOT(setClass(QTableWidgetItem*, int, int)) );

    } else {
        QList<QVariant> data = clickedItem->data(Qt::UserRole).toList();
        newClass = new NewClassDialog(clickedItem->row(), \
                                       clickedItem->column(), \
                                       data.at(MainTableOptions::ClassName).toString(), \
                                       data.at(MainTableOptions::ClassGrade).toString(), \
                                       data.at(MainTableOptions::ClassSection).toString(), \
                                       data.at(MainTableOptions::ClassNotes).toString());
        newClass->setWindowTitle("Edit A Class");
        connect( newClass, SIGNAL(newClassInput(QTableWidgetItem*, int, int)), \
                 this, SLOT(editClass(QTableWidgetItem*, int, int)) );

    }


    newClass->move(this->geometry().center().x()-newClass->geometry().width()/2, \
                   this->geometry().center().y()-newClass->geometry().height()/2);
    newClass->showDialog();
}

void MainWindow::hHeaderSelected(int column)
{
    int visualIndex = this->m_pTableWidget->horizontalHeader()->visualIndex(column);
    if(visualIndex == 0)
    {
        NewTeacherDialog *newTeach = new NewTeacherDialog;
        connect( newTeach, SIGNAL(newTeacherInput(QTextEdit*)), this, \
                 SLOT(createNewTeachers(QTextEdit*)) );


        newTeach->setWindowTitle("Add teachers");
        newTeach->move(geometry().center().x()-newTeach->geometry().width()/2, \
                       geometry().center().y()-newTeach->geometry().height()/2);

        newTeach->showDialog();
    }
}

void MainWindow::vHeaderSelected(int row)
{
    RowGradesDialog *rowGrades = new RowGradesDialog(this->m_pTableWidget, row);
    rowGrades->setWindowTitle("Grade Totals");
    rowGrades->move(this->geometry().center().x()-rowGrades->geometry().width()/2, \
                   this->geometry().center().y()-rowGrades->geometry().height()/2 + \
                    this->m_pTableWidget->rowHeight(0)*row);
    rowGrades->showDialog();
}

void MainWindow::createNewTeachers(QTextEdit *inText)
{
    QString input = inText->toPlainText().toUpper();
    if(input.length() < 1) { return; }

    QStringList teacherNames = input.split( "," );

    for(int i=0;i<teacherNames.length();i++)
    {
        if(teacherNames.at(i).length()>=18) {
            QString replaceName = teacherNames.at(i);
            int chopPoint = teacherNames.at(i).length()-18;
            replaceName.chop(chopPoint);
            teacherNames.replace(i, replaceName);
        } else if(teacherNames.at(i).length()<1) {
            teacherNames.removeAt(i);
        }
    }

    this->m_undoStack->push( new CommandTeacherAdd(&teacherNames, &this->m_HTableHeader, this->m_pTableWidget) );
}

void MainWindow::editTeacher(QString headerAfter, int column)
{
    this->m_undoStack->push( new CommandTeacherEdit(column, headerAfter, this->m_pTableWidget, &this->m_HTableHeader));
}

void MainWindow::moveTeacher(int fromIndex, int toIndex)
{
    this->m_undoStack->push( new CommandTeacherMove(this->m_pTableWidget, fromIndex, toIndex));
}

void MainWindow::setClass(QTableWidgetItem *item, int nRow, int nColumn)
{
    QTableWidgetItem *newItem = new QTableWidgetItem(*item);
    this->m_undoStack->push( new CommandClassAdd(nRow, nColumn, newItem, m_pTableWidget) );
}

void MainWindow::deleteClass()
{
    if(this->m_pTableWidget->currentColumn() < 1 || \
            this->m_pTableWidget->currentItem()->data(Qt::UserRole).toStringList().size() <= 1)
    {
        return;
    } else {
        this->m_undoStack->push( new CommandClassDelete( \
                           this->m_pTableWidget->currentRow(), \
                           this->m_pTableWidget->currentColumn(), \
                           this->m_pTableWidget->currentItem(), \
                           this->m_pTableWidget) );
    }
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

void MainWindow::editClassDialog()
{
    if(this->m_pTableWidget->currentColumn() < 1)
        return;

    QTableWidgetItem *clickedItem = this->m_pTableWidget->currentItem();

    if(clickedItem->data(Qt::UserRole).toStringList().size() <= 1)
        return;

    QList<QVariant> data = clickedItem->data(Qt::UserRole).toList();

    NewClassDialog *editWindow = \
            new NewClassDialog(clickedItem->row(), \
                               clickedItem->column(), \
                               data.at(MainTableOptions::ClassName).toString(), \
                               data.at(MainTableOptions::ClassGrade).toString(), \
                               data.at(MainTableOptions::ClassSection).toString(), \
                               data.at(MainTableOptions::ClassNotes).toString(), \
                               this);

    connect( editWindow , SIGNAL(newClassInput(QTableWidgetItem*, int, int)), \
             this, SLOT(editClass(QTableWidgetItem*, int, int)) );

    editWindow->setWindowTitle("Edit Class");
    editWindow->showDialog();
}

void MainWindow::editClass(QTableWidgetItem *item, int nRow, int nColumn)
{
    QTableWidgetItem *newItem = new QTableWidgetItem(*item);
    this->m_undoStack->push( new CommandClassEdit(nRow, nColumn, newItem, this->m_pTableWidget) );
}

void MainWindow::copyClass()
{
    if(this->m_pTableWidget->currentColumn() < 1 || \
            this->m_pTableWidget->currentItem()->data(Qt::UserRole).toStringList().size() <= 1)
        return;

    this->m_pTableWidget->resetCopiedItemNumber();
    this->m_pTableWidget->setCopiedItem( this->m_pTableWidget->currentItem() );
}

void MainWindow::pasteClass()
{
    if(this->m_pTableWidget->currentColumn() < 1 || \
            this->m_pTableWidget->getCopiedItem()->text() == " \n \n ")
        return;
    else
        this->m_undoStack->push( new CommandClassPaste(this->m_pTableWidget->currentRow(), \
                                                       this->m_pTableWidget->currentColumn(), \
                                                       this->m_pTableWidget) );
}

void MainWindow::highlightClass(int row, int column, QString highlighter)
{
    this->m_undoStack->push( new CommandClassHighlight(row, column, highlighter, \
                                                       this->m_pTableWidget) );
}

void MainWindow::showClass()
{
    if(this->m_pTableWidget->currentColumn() < 1)
        return;

    QTableWidgetItem *clickedItem = this->m_pTableWidget->currentItem();

    if( clickedItem->data(Qt::UserRole).toStringList().size() <= 1)
        return;

    QString displayedMessage = \
            clickedItem->data(Qt::UserRole).toList().at(MainTableOptions::ClassNotes).toString();

    QMessageBox::information( this, "Notes", displayedMessage );
}

void MainWindow::showHelp()
{
    QMessageBox::about(this, "TeacherTable", "TeacherTable version 1.0.\n\nGo to www.vpd29772.vps.ovh.ca for help and updates.");
}

void MainWindow::showQtHelp()
{
    QMessageBox::aboutQt(this, "AboutQt");
}

void MainWindow::createNewTable()
{
    this->m_pTableWidget->setColumnCount(1);
    this->m_pTableWidget->HTableHeader().clear();
    this->m_pTableWidget->HTableHeader()<<"A\nD\nD\n \nT\nE\nA\nC\nH\nE\nR";

    this->m_pTableWidget->insertInstructions(this->m_pTableWidget);

    this->m_HTableHeader.clear();
    this->m_HTableHeader = this->m_pTableWidget->HTableHeader();
    this->m_pTableWidget->setHorizontalHeaderLabels(this->m_HTableHeader);

    this->m_undoStack->clear();
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

QSize MainWindow::getWindowSize()
{
    return this->m_pTableWidget->getTableSize(this->m_pTableWidget);
}

void MainWindow::saveToFile()
{
    if(this->m_saveFileName->size() <= 1)
        this->m_saveFileName = new QString( QFileDialog::getSaveFileName( \
                                                this, QString("Save TimeTable"), \
                                                "", QString("TimeTable(*.ttl);;All Files (*)") ) );

    if( !this->m_saveFileName->endsWith(".ttl") )
        this->m_saveFileName->append(QString(".ttl"));

    if (this->m_saveFileName->isEmpty())
        return;
    else {
        QFile file(*this->m_saveFileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), \
                file.errorString());
            return;
        }
        this->m_undoStack->clear();

        QDataStream out(&file);

        //table size
        qint32 numRows = this->m_pTableWidget->rowCount();
        qint32 numCols = this->m_pTableWidget->columnCount();
        out << numRows << numCols;

        QStringList visualhHeaders;

        for(int i=0; i<numCols; i++)
        {
            int logicalCol = this->m_pTableWidget->horizontalHeader()->logicalIndex(i);
            visualhHeaders.append(this->m_pTableWidget->horizontalHeaderItem(logicalCol)->text());
        }

        out << visualhHeaders << this->m_VTableHeader;

        //table items by row then column
        for (int i=0; i<numRows; i++)
        {
            int visualRow = this->m_pTableWidget->verticalHeader()->logicalIndex(i);
            for (int j=1; j<numCols; j++)
            {
                int visualCol = this->m_pTableWidget->horizontalHeader()->logicalIndex(j);
                this->m_pTableWidget->item(visualRow,visualCol)->write(out);
            }
        }

        file.close();
    }
}

void MainWindow::saveAsToFile()
{
    this->m_saveFileName = new QString( QFileDialog::getSaveFileName( \
                                            this, QString("Save TimeTable"), \
                                            "", QString("TimeTable(*.ttl);;All Files (*)") ) );
    saveToFile();
}

void MainWindow::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open TimeTable"), *this->m_saveFileName,
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

        qint32 numRows, numCols;
        in >> numRows >> numCols;

        this->m_pTableWidget->setRowCount(numRows);
        this->m_pTableWidget->setColumnCount(numCols);

        for (int row = 0; row < m_pTableWidget->rowCount(); row++)
        {
            QTableWidgetItem *newItem = this->m_pTableWidget->item(row, 0);
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsDropEnabled);
        }

        for (int col = 1; col < m_pTableWidget->columnCount(); col++)
        {
            m_pTableWidget->setItemDelegateForColumn( col, new HighlightItemDelegate(this->m_pTableWidget) );
        }

        QStringList inHTableHeader, inVTableHeader;

        in >> inHTableHeader >> inVTableHeader;
        this->m_HTableHeader = inHTableHeader;
        this->m_VTableHeader = inVTableHeader;

        this->m_pTableWidget->setHorizontalHeaderLabels(this->m_HTableHeader);
        this->m_pTableWidget->setVerticalHeaderLabels(this->m_VTableHeader);

        for (int i = 0; i < numRows; i++)
        {
           for (int j = 1; j < numCols; j++)
           {
               QTableWidgetItem *item = new QTableWidgetItem;
               item->read(in);
               this->m_pTableWidget->setItem(i, j, item);

               if(item->data(Qt::UserRole).toStringList().size() <= 1)
               {
                   item->setText("wwwwwww");
                   this->m_pTableWidget->resizeColumnToContents(j);
                   item->setText(" \n \n ");
               }
           }
           this->m_pTableWidget->resizeRowToContents(i);
        }

        for (int row = 0; row < m_pTableWidget->rowCount(); row++)
        {
            QTableWidgetItem *newItem = this->m_pTableWidget->item(row, 0)->clone();
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsDropEnabled \
                              & ~Qt::ItemIsEnabled & ~Qt::ItemIsDragEnabled);
            this->m_pTableWidget->setItem(row, 0, newItem);
        }

        file.close();
    }
}

void MainWindow::closeWindow()
{
    this->m_undoStack->clear();

    this->close();
}
