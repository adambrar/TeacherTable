#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    window.resize( window.getWindowSize().width()*7, \
                   (window.getWindowSize().height() + (window.menuBar()->height()*2)) );
    window.move(window.size().width()/4, window.size().height()/8);

    if(argc >= 2)
        window.loadFile(argv[1]);
    return a.exec();
}
