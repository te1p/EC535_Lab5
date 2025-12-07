#include "mainwindow.h"
#include <QApplication>

//entry point for the Qt application.
//creates the QApplication object and shows MainWindow.
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Launch home menu (MainWindow)
    MainWindow w;
    w.show();

    //Start Qt event loop
    return a.exec();
}
