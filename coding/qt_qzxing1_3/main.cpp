#include "mainwindow.h"
#include "mainwindow2.h"
#include "color_blocks.h"
#include <QApplication>
#include <QtSerialPort/QtSerialPort>

// extern MainWindow* globalMainWindow;
// extern MainWindow2* globalMainWindow2;
// extern Color_Blocks* color_blocks3;// 定义另一个全局指针变量，初始化为 NULL，可能用于在程序的不同部分访问另一个窗口实例。
MainWindow* globalMainWindow=NULL;// 定义一个全局指针变量初始化为 NULL，可能用于在程序的不同部分访问主窗口实例。
MainWindow2* globalMainWindow2=NULL;// 定义另一个全局指针变量，初始化为 NULL，可能用于在程序的不同部分访问另一个窗口实例。
Color_Blocks* color_blocks3=NULL;// 定义另一个全局指针变量，初始化为 NULL，可能用于在程序的不同部分访问另一个窗口实例。
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 创建 Color_Blocks 实例
    Color_Blocks color_blocks;
    color_blocks3 = &color_blocks; // 初始化全局指针
    // // 创建主窗口实例
    MainWindow w;
    globalMainWindow = &w;
    // 创建第二个窗口实例
    MainWindow2 mainwindow2;
    globalMainWindow2 = &mainwindow2;
    mainwindow2.show();
    return a.exec();
}


