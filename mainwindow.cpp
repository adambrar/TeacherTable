#include "mainwindow.h"
#include "newteacherdialog.h"
#include "newclassdialog.h"
#include "editteacherdialog.h"
#include "commandclassdelete.h"
//#include "commandteacheradd.h"
//#include "commandteacherdelete.h"
//#include "commandteacheredit.h"
//#include "commandteachermove.h"
#include "edittabletitledialog.h"
#include "horizontalheaderview.h"
#include "highlightitemdelegate.h"
#include "maintablewidget.h"
#include "rowgradesdialog.h"
#include "classhelper.h"
#include "teacherhelper.h"


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
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->m_undoStack = new QUndoStack(this);
    this->m_undoView = 0;
    this->validateString = QString("This is a valid TeacherTable!");

    this->m_pTableWidget = new MainTableWidget(this);
    this->m_tableTitle = new QLabel("My Table");
    this->m_tableTitle->setAlignment(Qt::AlignCenter);

    if (this->m_tableTitle->fontInfo().pointSize() != -1) {
        QFont font(this->m_tableTitle->font());
        font.setPointSize(this->m_tableTitle->font().pointSize()+6);
        this->m_tableTitle->setFont(font);
    } else if (this->m_tableTitle->fontInfo().pixelSize() != -1) {
        QFont font(this->m_tableTitle->font());
        font.setPixelSize(this->m_tableTitle->font().pixelSize()+10);
        this->m_tableTitle->setFont(font);
    }

    this->mainWidget = new QWidget();

    QVBoxLayout *verticalWindowLayout = new QVBoxLayout;
    verticalWindowLayout->addWidget(this->m_tableTitle);
    verticalWindowLayout->addWidget(this->m_pTableWidget);
    mainWidget->setLayout(verticalWindowLayout);

    this->setCentralWidget(this->mainWidget);

    this->m_pTableWidget->initTableWidget(m_pTableWidget);
    this->m_pTableWidget->insertInstructions( m_pTableWidget );
    this->m_pTableWidget->setUndoStack(m_undoStack);

    createMenu();

    this->setMinimumHeight( this->m_pTableWidget->getTableSize(this->m_pTableWidget).height() );

    connect( this->m_pTableWidget, SIGNAL( cellDoubleClicked (int, int) ),
             this, SLOT( cellDoubleClicked( int, int ) ) );

    connect( this->m_pTableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)),
             this, SLOT(hHeaderSelected(int)) );

    connect( this->m_pTableWidget, SIGNAL(customContextMenuRequested(QPoint)), \
             this, SLOT(cellContextMenu(QPoint)) );

    connect( this->m_pTableWidget->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)), \
             this, SLOT(headerContextMenu(QPoint)) );

    connect( this->m_pTableWidget, SIGNAL(classMoved(QTableWidgetItem*,int,int,int,int)),\
             this->m_classHelper, SLOT(moveClass(QTableWidgetItem*,int,int,int,int)) );

    connect( this->m_pTableWidget->getHHeaderView(), SIGNAL(teacherMoved(int,int)), \
             this->m_teacherHelper, SLOT(moveTeacher(int,int)) );

    connect( this->m_pTableWidget->verticalHeader(), SIGNAL(sectionClicked(int)), \
             this, SLOT(vHeaderSelected(int)) );

    this->m_pTableWidget->setCopiedItem( new QTableWidgetItem(QString(" \n \n ")) );
    this->m_pTableWidget->setFocus();

    this->m_saveFileName = new QString("");
    this->blackRowNum = 4;
}

MainWindow::~MainWindow()
{
    this->m_undoStack->clear();

    delete this->m_pTableWidget;
    delete this->m_undoStack;
    delete this->m_classHelper;
    delete this->m_teacherHelper;
    delete this->m_tableTitle;
    delete this->m_saveFileName;
    delete this->mainWidget;
}

void MainWindow::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    QMenu *toolsMenu = menuBar()->addMenu("&Tools");
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
    fileMenu->addSeparator();
    fileMenu->addAction("&Quit...", this, SLOT(closeWindow()), QKeySequence::Close);

    QAction *undoAction = m_undoStack->createUndoAction( this );
    QAction *redoAction = m_undoStack->createRedoAction( this );
    undoAction->setShortcut( QKeySequence::Undo );
    redoAction->setShortcut( QKeySequence::Redo );

    this->m_classHelper = new ClassHelper(this->m_pTableWidget, this->m_undoStack);
    this->m_teacherHelper = new TeacherHelper(this->m_pTableWidget, this->m_undoStack);

    editMenu->addAction( undoAction );
    editMenu->addAction( redoAction );
    editMenu->addSeparator();
    editMenu->addAction( "&Edit Class", this, SLOT(editClassDialog()) );
    editMenu->addAction( "&Copy Class", this->m_classHelper, SLOT(copyClass()), QKeySequence::Copy );
    editMenu->addAction( "&Paste Class", this->m_classHelper, SLOT(pasteClass()), QKeySequence::Paste );
    editMenu->addSeparator();
    editMenu->addAction( "&Delete Class", this->m_classHelper, SLOT(deleteClass()), QKeySequence::Delete );

    toolsMenu->addAction("&Add Teachers...", this, SLOT(createNewTeachersDialog()));
    toolsMenu->addSeparator();
    toolsMenu->addAction("E&xport to PDF", this, SLOT(tableToPDF()));
    toolsMenu->addSeparator();
    toolsMenu->addAction("Edit Table Title", this, SLOT(createChangeTitleDialog()));

    viewMenu->addAction( "&Show Notes", this->m_classHelper, SLOT(showClass()) );
    //viewMenu->addAction( "Show Stac&k", this, SLOT(showUndoStack()) );

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
    dateLabel.setStyleSheet("QLabel { background-color : white; color : black }");
    QPixmap grabDateLabel = dateLabel.grab();

    QLabel tableTitle(this->m_tableTitle->text());
    tableTitle.setStyleSheet("QLabel { background-color : white; color : black }");

    QFont font(tableTitle.font());
    if (tableTitle.fontInfo().pointSize() != -1) {
        font.setPointSize(tableTitle.font().pointSize()+6);
    } else if (tableTitle.fontInfo().pixelSize() != -1) {
        font.setPixelSize(tableTitle.font().pixelSize()+10);
    }
    tableTitle.setFont(font);
    tableTitle.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QPixmap grabTableTitle = tableTitle.grab();

    QPainter painter;
    painter.begin(&printer);
    for(int i=0;i<tempTables.size();i++)
    {
        tempTables.at(i)->setStyleSheet("QHeaderView::section { background-color : white } \
                                                   QHeaderView { background-color : white }");
        QPixmap grabTableMap = tempTables.at(i)->grab();

        QLabel pageLabel(QString("Page %1 of %2").arg(i+1).arg(tempTables.size()));
        QPixmap grabPageLabel = pageLabel.grab();

        painter.drawPixmap(printer.pageRect().width()/2-grabTableTitle.width()/2, \
                           0, grabTableTitle.width(), \
                           grabTableTitle.height(), grabTableTitle);

        painter.drawPixmap(10, grabTableTitle.height(), tempTables.at(i)->width(), \
                           printer.pageRect().height()-grabPageLabel.height()-grabTableTitle.height(), \
                           grabTableMap);

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

    //date display
    QString date = QDate::currentDate().toString("dddd dd, MMMM, yyyy");
    QLabel dateLabel(date);
    dateLabel.setStyleSheet("QLabel { background-color : white; color : black }");
    QPixmap grabDateLabel = dateLabel.grab();
    //
    //table title display
    QLabel tableTitle(this->m_tableTitle->text());
    tableTitle.setStyleSheet("QLabel { background-color : white; color : black }");

    QFont font(tableTitle.font());
    if (tableTitle.fontInfo().pointSize() != -1) {
        font.setPointSize(tableTitle.font().pointSize()+6);
    } else if (tableTitle.fontInfo().pixelSize() != -1) {
        font.setPixelSize(tableTitle.font().pixelSize()+10);
    }
    tableTitle.setFont(font);
    tableTitle.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QPixmap grabTableTitle = tableTitle.grab();
    //
    //page number display
    QLabel pageLabel;
    pageLabel.setStyleSheet("QLabel { background-color : white; color : black }");
    //

    QPainter painter;
    painter.begin(&printer);
    for(int i=0;i<tempTables.size();i++)
    {
        QPixmap grabTableMap = tempTables.at(i)->grab();

        pageLabel.setText(QString("Page %1 of %2").arg(i+1).arg(tempTables.size()));
        QPixmap grabPageLabel = pageLabel.grab();

        painter.drawPixmap(printer.pageRect().width()/2-grabTableTitle.width()/2, \
                           0, grabTableTitle.width(), \
                           grabTableTitle.height(), grabTableTitle);

        painter.drawPixmap(10, grabTableTitle.height(), tempTables.at(i)->width(), \
                           printer.pageRect().height()-grabPageLabel.height()-grabTableTitle.height()-2, \
                           grabTableMap);

        painter.drawPixmap(0, printer.pageRect().height()-grabPageLabel.height(), \
                           grabPageLabel.width(), grabPageLabel.height(), \
                           grabPageLabel);

        painter.drawPixmap(printer.pageRect().width()-grabDateLabel.width(), \
                           printer.pageRect().height()-grabDateLabel.height(), \
                           grabDateLabel.width(), grabPageLabel.height(), \
                           grabDateLabel);

        if(i != tempTables.size()-1)
            printer.newPage();
    }
    painter.end();

    QMessageBox::information(this, "Done!", "Finished exporting table.");
}

void MainWindow::createChangeTitleDialog()
{
    EditTableTitleDialog *titleDialog = new EditTableTitleDialog(this->m_tableTitle->text());
    titleDialog->setWindowTitle("Edit Table Title");
    titleDialog->move(this->geometry().center().x(), this->geometry().center().y());
    connect(titleDialog, SIGNAL(changeTitle(QString)), this, SLOT(tableTitleChanged(QString)));
    titleDialog->showDialog();
}

void MainWindow::tableTitleChanged(QString newTitle)
{
    this->m_tableTitle->setText(newTitle);
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
        this->m_classHelper->showClass();
    } else if ( clickedAction == copyAction ) {
        this->m_classHelper->copyClass();
    } else if ( clickedAction == pasteAction ) {
        this->m_classHelper->pasteClass();
    } else if ( highlighterActions.contains(clickedAction) ) {
        QString actionText = clickedAction->text();
        this->m_classHelper->highlightClass( clickedItem->row(), clickedItem->column(), \
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
    QString headerName = this->m_pTableWidget->HTableHeader().at(clickedColumn);

    headerName = headerName.remove("\n");

    QMenu menu;
    QAction* editAction = menu.addAction("&Edit");
    QAction* deleteAction = menu.addAction("&Delete");

    QAction *clickedAction = menu.exec( QCursor::pos() );

    if ( clickedAction == editAction )
    {
        EditTeacherDialog *editDialog = new EditTeacherDialog(headerName, clickedColumn);

        connect( editDialog, SIGNAL(editTeacherInput(QString,int)), \
                 this->m_teacherHelper, SLOT(editTeacher(QString,int)) );

        editDialog->move(point);
        editDialog->setWindowTitle("Edit Teacher");
        editDialog->showDialog();


    } else if ( clickedAction == deleteAction ) {
        this->m_undoStack->clear();
        this->m_pTableWidget->removeColumn(clickedColumn);
        this->m_pTableWidget->HTableHeader().removeAt(clickedColumn);
    }
}

void MainWindow::cellDoubleClicked(int nRow, int nCol)
{
    NewClassDialog *newClass;
    QTableWidgetItem *clickedItem = this->m_pTableWidget->item(nRow, nCol);

    if(clickedItem->data(Qt::UserRole).toList().length() < 2)
    {
        newClass = new NewClassDialog(nRow, nCol);
        newClass->setWindowTitle("Add A Class");
        connect( newClass, SIGNAL(newClassInput(QTableWidgetItem*, int, int)), \
                 this->m_classHelper, SLOT(setClass(QTableWidgetItem*, int, int)) );

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
                 this->m_classHelper, SLOT(editClass(QTableWidgetItem*, int, int)) );

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
        createNewTeachersDialog();
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

void MainWindow::createNewTeachersDialog()
{
    NewTeacherDialog *newTeach = new NewTeacherDialog;
    connect( newTeach, SIGNAL(newTeacherInput(QTextEdit*)), this->m_teacherHelper, \
             SLOT(createNewTeachers(QTextEdit*)) );


    newTeach->setWindowTitle("Add teachers");
    newTeach->move(geometry().center().x()-newTeach->geometry().width()/2, \
                   geometry().center().y()-newTeach->geometry().height()/2);

    newTeach->showDialog();
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
             this->m_classHelper, SLOT(editClass(QTableWidgetItem*, int, int)) );

    editWindow->setWindowTitle("Edit Class");
    editWindow->showDialog();
}

void MainWindow::showHelp()
{
    QMessageBox::about(this, "TeacherTable", "TeacherTable version 1.5.0.5014.\n\nGo to www.teachertables.xyz for help and updates.");
}

void MainWindow::showQtHelp()
{
    QMessageBox::aboutQt(this, "AboutQt");
}

void MainWindow::createNewTable()
{
    this->m_tableTitle->setText("My Table");

    this->m_pTableWidget->setColumnCount(1);

    this->m_pTableWidget->HTableHeaderClear();

    this->m_pTableWidget->HTableHeaderAppend(" \n \n \nA\nD\nD\n \nT\nE\nA\nC\nH\nE\nR\n \n ");

    this->m_pTableWidget->insertInstructions(this->m_pTableWidget);

    this->m_pTableWidget->setHorizontalHeaderLabels(this->m_pTableWidget->HTableHeader());

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
    QSize tableSize = this->m_pTableWidget->getTableSize(this->m_pTableWidget);

    tableSize.setHeight(this->m_tableTitle->height()+tableSize.height());

    return tableSize;
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

        //string to validate file
        out << this->validateString;

        //table title
        out << this->m_tableTitle->text();

        //table size
        qint32 numRows = this->m_pTableWidget->rowCount();
        qint32 numCols = this->m_pTableWidget->columnCount();
        out << numRows << numCols;

        //horizontal headers...teacher names
        QStringList visualhHeaders;

        for(int i=0; i<numCols; i++)
        {
            int logicalCol = this->m_pTableWidget->horizontalHeader()->logicalIndex(i);
            visualhHeaders.append(this->m_pTableWidget->horizontalHeaderItem(logicalCol)->text());
        }

        //vertical headers...blocks
        out << visualhHeaders << this->m_pTableWidget->VTableHeader();

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

void MainWindow::loadFromFile(QString fileName)
{
    //open file
    if(fileName == "") {
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open TimeTable"), *this->m_saveFileName,
            tr("TimeTable (*.ttl);;All Files (*)"));
    }

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

        //check for valid teachertable file
        QString fileValidate;
        in >> fileValidate;

        if(fileValidate != this->validateString) {
            QMessageBox::information(this, "Warning", \
                                     "Invalid file format. Double check the name and location of the file you are loading.");
            return;
        }

        createNewTable();

        //initialize table with title, rows, cols, instructions and delegates
        QString tableTitle;
        in >> tableTitle;
        this->m_tableTitle->setText(tableTitle);

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

        //input headers
        QStringList inHTableHeader, inVTableHeader;

        in >> inHTableHeader >> inVTableHeader;
        this->m_pTableWidget->setHTableHeader(inHTableHeader);
        this->m_pTableWidget->setVTableHeader(inVTableHeader);

        //set vertical header at end to account for added rows
        int height = this->m_pTableWidget->horizontalHeader()->height();
        this->m_pTableWidget->setHorizontalHeaderLabels(this->m_pTableWidget->HTableHeader());
        this->m_pTableWidget->setVerticalHeaderLabels(this->m_pTableWidget->VTableHeader());
        this->m_pTableWidget->horizontalHeader()->setFixedHeight(height);

        //remove black column of old table
        if( numRows != this->m_pTableWidget->numBlocks()+this->m_pTableWidget->numExtraBlocks() ) {
            this->m_pTableWidget->removeRow(this->blackRowNum);
            this->m_pTableWidget->setRowCount(this->m_pTableWidget->rowCount()+1);

        }

        //insert all items
        for (int i = 0; i < this->m_pTableWidget->rowCount(); i++)
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
               } else {
                   QString grade = item->data(Qt::UserRole).toStringList().at(
                               MainTableOptions::ClassGrade);

                   this->m_pTableWidget->item(i, j)-> \
                           setBackgroundColor(this->m_pTableWidget->TableOptions()->getGradeColor(
                                                  grade));
               }
           }
        }
        this->m_pTableWidget->resizeRowsToContents();

        //reinsert black column
        if( numRows != this->m_pTableWidget->numBlocks()+this->m_pTableWidget->numExtraBlocks() ) {
            this->m_pTableWidget->insertRow(this->blackRowNum);

            for (int j = 1; j < numCols; j++)
            {
                QTableWidgetItem *newItem = new QTableWidgetItem;
                newItem->setText(QString(" "));

                newItem->setFlags(newItem->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsDropEnabled);
                newItem->setData(Qt::UserRole, (QVariant)QString("0"));
                newItem->setBackground(Qt::gray);

                this->m_pTableWidget->setItem(this->blackRowNum, j, newItem);
            }
            this->m_pTableWidget->resizeRowToContents(this->blackRowNum);

            QStringList VTableHeader;
            VTableHeader<<"  A "<<"  B "<<"  C "<<"  D "
                                   <<""<<"  E "<<"  F "<<"  G "<<"  H "<<" "<<" "<<" ";

            this->m_pTableWidget->setVerticalHeaderLabels(VTableHeader);
            this->m_pTableWidget->setVTableHeader(VTableHeader);

        }

        file.close();
    }
}

void MainWindow::closeWindow()
{
    this->close();
}
