#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //set application run center screen
    QDesktopWidget *scrSize = QApplication::desktop();
    int W = scrSize->width()/2-w.width()/2;
    int H = scrSize->height()/2-w.height()/2;
    w.move(W,H);
    w.show();

    return a.exec();
}
