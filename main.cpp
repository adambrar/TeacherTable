#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    window.resize( window.getWindowSize().width()*7, (window.getWindowSize().height() + (window.menuBar()->height()*2)) );
    //window.move(window.size().width()/4, window.size().height()/4);

    return a.exec();
}
