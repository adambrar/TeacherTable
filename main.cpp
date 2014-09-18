#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(w.getWindowSize());
    w.move(w.size().width()/4, w.size().height()/4);
    w.show();

    return a.exec();
}
