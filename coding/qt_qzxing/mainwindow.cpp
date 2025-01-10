#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), qZXing(new QZXing(this)) {
    ui->setupUi(this);

    // 初始化摄像头，0为默认摄像头
    cap.open(0);
    if (!cap.isOpened()) {
        qDebug() << "无法打开摄像头!";
        return;
    }

    // 定时器，每隔33毫秒捕获一帧（约30帧/秒）
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::captureFrame);
    timer->start(33);
}

MainWindow::~MainWindow() {
    cap.release(); // 释放摄像头
    delete ui;
}

void MainWindow::captureFrame() {
    cv::Mat frame;
    cap >> frame; // 从摄像头获取一帧
    if (frame.empty()) {
        return; // 如果帧为空，返回
    }

    // 将OpenCV的Mat转换为QImage
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // 转换颜色格式
    QImage image = QImage((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    // 显示图像
    ui->label->setPixmap(QPixmap::fromImage(image));

    // 使用QZXing识别二维码
    QString result = qZXing->decodeImage(image);
    if (!result.isEmpty()) {
        qDebug() << "识别结果:" << result; // 输出识别结果
    }
}
