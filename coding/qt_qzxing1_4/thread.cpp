#include "thread.h"
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// CameraThread 类实现
extern Color_Blocks* color_blocks3;


void CameraThread::run() {
    while (!stopFlag) {
        cv::Mat frame;
        cap >> frame; // 获取当前帧
        if (!frame.empty()) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
            {
                QMutexLocker locker(&mutex);
                currentFrame = frame.clone(); // 保存当前帧用于二维码识别
            }
            QImage image((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            emit newFrame(image); // 发送图像用于显示
        }
        QThread::msleep(33); // 每33毫秒捕获一帧
    }
    cap.release();
}

void CameraThread::stop() {
    stopFlag = true;
    wait(); // 等待线程结束
}

cv::Mat CameraThread::getFrame() {
    QMutexLocker locker(&mutex);
    return currentFrame.clone(); // 返回当前帧
}

// QRCodeThread 类实现
void QRCodeThread::run() {
    while (!stopFlag) {
        cv::Mat frame = cameraThread->getFrame(); // 从 CameraThread 获取当前帧

        if (!frame.empty()) {
            QImage image((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            QString result = qZXing->decodeImage(image); // 识别二维码

            if (!result.isEmpty()) {
                emit qrCodeDetected(result); // 发送识别结果
            }
        }
        QThread::msleep(100); // 每800毫秒识别一次
    }
}

void QRCodeThread::stop() {
    stopFlag = true;
    wait(); // 等待线程结束
}



void color_blockThread::run() {
    if (!cap.open(0)) {
        qDebug() << "Failed to open camera in color_blockThread"; // 摄像头打开失败
    } else {
        qDebug() << "Camera opened successfully in color_blockThread"; // 摄像头打开成功
    }
    while (!stopFlag) {
        cv::Mat frame,framergb,framehsv;
        cap >> frame; // 获取当前帧
        color_blocks3->updateFramehsvs(frame);
        QThread::msleep(33); // 每33毫秒捕获一帧
    }
    cap.release();
}

void color_blockThread::stop()
{
    stopFlag = true;
    wait();

}





