#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(w.getWindowSize());
    w.move(10, 10);
    w.show();

    return a.exec();
}
