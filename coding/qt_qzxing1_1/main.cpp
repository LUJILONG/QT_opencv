#include "mainwindow.h"
#include "mainwindow2.h"
#include <QApplication>
#include <QtSerialPort/QtSerialPort>

extern MainWindow* globalMainWindow;
extern MainWindow2* globalMainWindow2;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MainWindow2 mainwindow2;//实例化窗口对象二
    globalMainWindow=&w;
    globalMainWindow2=&mainwindow2;
    mainwindow2.show();
    //w.show();
    return a.exec();
}


