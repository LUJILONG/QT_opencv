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
        QThread::msleep(800); // 每800毫秒识别一次
    }
}

void QRCodeThread::stop() {
    stopFlag = true;
    wait(); // 等待线程结束
}












void color_blockThread::run() {
    qDebug() << "Color block thread started"; // 调试输出，确认线程启动
    if (!cap.open(0)) {
        qDebug() << "Failed to open camera in color_blockThread"; // 摄像头打开失败
    } else {
        qDebug() << "Camera opened successfully in color_blockThread"; // 摄像头打开成功
    }

    while (!stopFlag) {
        cv::Mat frame,framergb,framehsv;
        // cv::Mat frames;
        cap >> frame; // 获取当前帧



        // cv::cvtColor(frame, framergb, cv::COLOR_BGR2RGB);
        //  // QImage image((const unsigned char*)frames.data, frames.cols, frames.rows, frames.step, QImage::Format_RGB888);
        //  QImage image((const unsigned char*)framergb.data, framergb.cols, framergb.rows, framergb.step, QImage::Format_RGB888);
        //  emit color_blockFrame(image); // 发送信号



        color_blocks3->updateFramehsvs(frame);
        // qDebug() << "123456" ;


        QThread::msleep(33); // 每33毫秒捕获一帧
    }
    cap.release();
}




// 将图像转换为 HSV
// cv::cvtColor(frame, framehsv, cv::COLOR_RGB2HSV);


// color_blocks3->ui_SliderGroup();
// qDebug() << "界面"; // 摄像头打开失败
// 创建滑块组的用户界面
// colorBlocks->ui_SliderGroup();

// horizontalScrollBar
//     void MyWidget::onValueChanged()
// {
// int value = horizontalScrollBar->value();
// qDebug() << "Current horizontal scroll bar value:" << value;
// }
// // 定义 HSV 阈值（示例：红色范围）
// int lowH = 0, highH = 10; // 红色的 Hue 值范围
// int lowS = 100, highS = 255;
// int lowV = 100, highV = 255;




// // 创建掩码
// cv::Mat mask;
// cv::inRange(framehsv, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), mask);

// // 轮廓检测
// std::vector<std::vector<cv::Point>> contours;
// cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

// // 在原始帧上绘制轮廓
// for (const auto &contour : contours) {
//     if (cv::contourArea(contour) > 500) { // 过滤较小的区域
//         cv::Rect boundingBox = cv::boundingRect(contour);
//         cv::rectangle(frame, boundingBox, cv::Scalar(0, 255, 0), 2); // 绘制绿色矩形框
//     }
// }

// // 发送处理后的图像
// QImage processedImage((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
// emit color_blockFramehsv(processedImage); // 发送带有轮廓的图像信号





void color_blockThread::stop()
{
    stopFlag = true;
    wait();

}

// void color_blockThread::updateImages(const QImage &image)
// {
//      ui->label->setPixmap(QPixmap::fromImage(image));
// }



