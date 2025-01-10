#include "mainwindow.h"// 引入主窗口的头文件，包含主窗口类的定义。
#include "mainwindow2.h"// 引入主窗口的头文件，包含主窗口类的定义。
#include "color_blocks.h"
#include "thread.h"
#include <QZXing.h> // 确保包含 QZXing 的头文件

extern MainWindow* globalMainWindow;
extern MainWindow2* globalMainWindow2;
extern Color_Blocks* color_blocks3;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), qZXing(new QZXing(this)) {
    // 主窗口类的构造函数，接受一个父窗口指针作为参数。使用初始化列表初始化主窗口的父类 QMainWindow、UI 类指针 ui 和二维码识别对象 qZXing。
    ui->setupUi(this); // 调用 UI 类的 setupUi 方法，设置主窗口的界面布局和组件。

    // 创建并启动CameraThread线程
    cameraThread = new CameraThread(this); // 创建 CameraThread 实例
    connect(cameraThread, &CameraThread::newFrame, this, &MainWindow::updateImage);
    cameraThread->start();
    qDebug() << "Camera thread started";

    // 创建并启动QRCodeThread线程
    qrCodeThread = new QRCodeThread(cameraThread, this);
    bool connects=connect(qrCodeThread, &QRCodeThread::qrCodeDetected, this, &MainWindow::displayQRCodeResult);
    qDebug() << "Connection successful:" << connects;
    qrCodeThread->start();
    qDebug() << "QRCode thread started";

    // 如果你需要启动色块识别线程
    colorblockThread = new color_blockThread(this);
    if (!color_blocks3) {
        qDebug() << "color_blocks3 is not initialized";
    } else {
        bool connected = connect(colorblockThread, &color_blockThread::color_blockFrame, color_blocks3, &Color_Blocks::updateImages);
        Q_ASSERT(connected);
        qDebug() << "Connection successful:" << connected;
    }
      connect(colorblockThread, &color_blockThread::color_blockFramehsv, color_blocks3, &Color_Blocks::updateImageshsv);
    colorblockThread->start();
    qDebug() << "Color block thread started";
}

void MainWindow::updateImage(const QImage &image){
    ui->label->setPixmap(QPixmap::fromImage(image)); // 更新实时摄像头图像
}

void MainWindow::displayQRCodeResult(const QString &result) {
    ui->qrCodeLabel->setText("识别结果: " + result); // 显示二维码结果
}



MainWindow::~MainWindow() {
    // 主窗口类的析构函数。
qDebug() << "释放";
    cameraThread->stop();
    qrCodeThread->stop();
    colorblockThread->stop(); // 确保在析构时停止所有线程
    delete ui;
    // 删除 UI 类指针，释放其占用的内存。
    delete qZXing; // 记得释放 qZXing 的资源
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "切换按钮被点击";

    if (globalMainWindow2) {
        this->close(); // 关闭当前主窗口
        globalMainWindow2->show(); // 显示另一个窗口实例
    } else {
        qDebug() << "globalMainWindow2 未初始化";
    }
}


void MainWindow::on_pushButton_3_clicked()//跳转色块界面
{
    qDebug() << "切换按钮被点击";

    if (color_blocks3) {
        this->close(); // 关闭当前主窗口
        color_blocks3->show(); // 显示另一个窗口实例
    } else {
        qDebug() << "color_blocks3 未初始化";
    }
}

