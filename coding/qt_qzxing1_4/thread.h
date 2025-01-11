#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QMutex>
#include <QImage>
#include <QZXing.h>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <mainwindow.h> // 引入主窗口头文件
#include <color_blocks.h> // 引入主窗口头文件

class Color_Blocks;
// 摄像头实时显示线程
class CameraThread : public QThread {
    Q_OBJECT

public:
    CameraThread(QObject *parent = nullptr) : QThread(parent), stopFlag(false) {
        cap.open(0); // 打开摄像头
    }

    void run() override;
    void stop();

    cv::Mat getFrame(); // 用于二维码识别线程获取当前帧

signals:
    void newFrame(const QImage &image);

private:
    cv::VideoCapture cap;
    cv::Mat currentFrame;
    QMutex mutex;
    bool stopFlag;
};

// 二维码识别线程
class QRCodeThread : public QThread {
    Q_OBJECT

public:
    QRCodeThread(CameraThread *cameraThread, QObject *parent = nullptr)
        : QThread(parent), cameraThread(cameraThread), qZXing(new QZXing(this)), stopFlag(false) {}

    void run() override;
    void stop();

signals:
    void qrCodeDetected(const QString &result);
private:
    CameraThread *cameraThread;
    QZXing *qZXing;
    bool stopFlag;
};


// 色块识别线程
class color_blockThread : public QThread {
    Q_OBJECT

public:
    color_blockThread(QObject *parent = nullptr) :QThread(parent), stopFlag(false) {}
    // 构造函数，接受 Color_Blocks 实例的指针
    // color_blockThread(Color_Blocks *colorBlocksInstance, QObject *parent = nullptr)
        // : QThread(parent), colorBlocks(colorBlocksInstance), stopFlag(false) {}
    void run() override;
    void stop();

// signals:
//     void color_blockFrame(const QImage &image);
//     void color_blockFramehsv(const QImage &image);

private:
    cv::VideoCapture cap;
    // Color_Blocks *colorBlocks; // 指向 Color_Blocks 实例的指针
    bool stopFlag;
    QMutex mutex;

};




#endif // THREAD_H
