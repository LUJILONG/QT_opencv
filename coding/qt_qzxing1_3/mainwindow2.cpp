#include "mainwindow2.h"
#include "ui_mainwindow2.h"
#include "mainwindow.h"
#include "color_blocks.h"
#include "thread.h"
#include <QDebug>
//#include "ui_mainwindow.h"
extern MainWindow* globalMainWindow;
class CameraThread;
class QRCodeThread;
class MainWindow;
class color_blockThread;
MainWindow2::MainWindow2(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow2)
{
    //this->show();
    ui->setupUi(this);
}

MainWindow2::~MainWindow2()
{
    delete ui;
}

void MainWindow2::on_SystemButton_clicked()
{
    this->close();


        // cameraThread->start();
        // qrCodeThread->start();
        // colorblockThread->start();
        // timer->start(1000);
    qDebug()<< "切换按钮被点击";
    globalMainWindow->show();
}

