#include "color_blocks.h"
#include "ui_color_blocks.h"
#include "mainwindow.h"// 引入主窗口的头文件，包含主窗口类的定义。
extern MainWindow* globalMainWindow;

Color_Blocks::Color_Blocks(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Color_Blocks)
{
    ui->setupUi(this);
    qlabels();
    // updateFramehsvs();
    // qDebug() << "123456" ;
    // int value1=ui->horizontalScrollBar->value();
    //  qDebug() << "Current horizontal scroll bar value:" << value1;
    // ui_SliderGroup();
}
// int value = horizontalScrollBar->value();
//

Color_Blocks::~Color_Blocks()
{
    delete ui;
}


void Color_Blocks::updateImages(const QImage &image) {

    ui->label->setPixmap(QPixmap::fromImage(image)); // 更新 UI

}

void Color_Blocks::updateImageshsv(const QImage &image) {

    ui->label_2->setPixmap(QPixmap::fromImage(image)); // 更新 UI

}



void Color_Blocks::on_pushButton_clicked()//跳转主界面
{
    this->close();
    qDebug()<< "切换按钮被点击";

    globalMainWindow->show();
}

bool Color_Blocks::detectAndDrawContours(cv::Mat &frame, const cv::Mat &mask, const cv::Scalar &color) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    bool detected = false;
    for (const auto &contour : contours) {
        if (cv::contourArea(contour) > 500) {  // 过滤掉较小区域
            cv::Rect boundingBox = cv::boundingRect(contour);
            cv::rectangle(frame, boundingBox, color, 2);
            detected = true;
        }
    }
    return detected;
}

void Color_Blocks::updateFramehsvs(cv::Mat frame) {
    if (frame.empty()) {
        return;
    }

    // 转换到 HSV 颜色空间
    cv::Mat hsvFrame;
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

    // 初始化空的掩膜
    cv::Mat combinedMask = cv::Mat::zeros(hsvFrame.size(), CV_8UC1);

    // 检测每种颜色，如果复选框启用则应用检测
    if (ui->checkBox->isChecked()) {
        cv::Mat mask1;
        cv::inRange(hsvFrame, cv::Scalar(color1LowH, color1LowS, color1LowV), cv::Scalar(color1HighH, color1HighS, color1HighV), mask1);
        if (detectAndDrawContours(frame, mask1, cv::Scalar(0, 0, 255))) {
         //   qDebug() << "Detected Color 1 (Red)";
        }
        combinedMask |= mask1;
    }
    if (ui->checkBox_2->isChecked()) {
        cv::Mat mask2;
        cv::inRange(hsvFrame, cv::Scalar(color2LowH, color2LowS, color2LowV), cv::Scalar(color2HighH, color2HighS, color2HighV), mask2);
        if (detectAndDrawContours(frame, mask2, cv::Scalar(0, 255, 0))) {
         //   qDebug() << "Detected Color 2 (Green)";
        }
        combinedMask |= mask2;
    }
    if (ui->checkBox_3->isChecked()) {
        cv::Mat mask3;
        cv::inRange(hsvFrame, cv::Scalar(color3LowH, color3LowS, color3LowV), cv::Scalar(color3HighH, color3HighS, color3HighV), mask3);
        if (detectAndDrawContours(frame, mask3, cv::Scalar(255, 0, 0))) {
          //  qDebug() << "Detected Color 3 (Blue)";
        }
        combinedMask |= mask3;
    }

    // 显示叠加后的图像
    cv::Mat filteredImage;
    cv::cvtColor(combinedMask, filteredImage, cv::COLOR_GRAY2RGB);
    QImage filteredQImage = QImage((const unsigned char*)filteredImage.data, filteredImage.cols, filteredImage.rows, filteredImage.step[0], QImage::Format_RGB888);
    // filteredLabel->setPixmap(QPixmap::fromImage(filteredQImage).scaled(filteredLabel->size(), Qt::KeepAspectRatio));
    ui->label_2->setPixmap(QPixmap::fromImage(filteredQImage).scaled(ui->label_2->size(), Qt::KeepAspectRatio));
    // 显示原始图像
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img = QImage((const unsigned char*)frame.data, frame.cols, frame.rows, frame.step[0], QImage::Format_RGB888);
  //   originalLabel->setPixmap(QPixmap::fromImage(img).scaled(originalLabel->size(), Qt::KeepAspectRatio));
    ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size(), Qt::KeepAspectRatio));

}


void Color_Blocks::qlabels()
{
    // int color1LowH = 0, color1HighH = 10, color1LowS = 100, color1HighS = 255, color1LowV = 100, color1HighV = 255;
    // int color2LowH = 35, color2HighH = 85, color2LowS = 100, color2HighS = 255, color2LowV = 100, color2HighV = 255;
    // int color3LowH = 100, color3HighH = 130, color3LowS = 100, color3HighS = 255, color3LowV = 100, color3HighV = 255;
ui->label_11->setNum(color1LowH);
ui->label_12->setNum(color1HighH);
ui->label_13->setNum(color1LowS);
ui->label_14->setNum(color1HighS);
ui->label_15->setNum(color1LowV);
ui->label_16->setNum(color1HighV);

ui->label_18->setNum(color2LowH);
ui->label_19->setNum(color2HighH);
ui->label_20->setNum(color2LowS);
ui->label_21->setNum(color2HighS);
ui->label_22->setNum(color2LowV);
ui->label_23->setNum(color2HighV);

ui->label_25->setNum(color3LowH);
ui->label_26->setNum(color3HighH);
ui->label_27->setNum(color3LowS);
ui->label_28->setNum(color3HighS);
ui->label_29->setNum(color3HighS);
ui->label_30->setNum(color3HighS);


// ui->horizontalScrollBar->setValue(157);
}


  //int color1LowH = 157, color1HighH = 180, color1LowS = 70, color1HighS = 198, color1LowV = 162, color1HighV = 255;
void Color_Blocks::on_horizontalScrollBar_valueChanged(int value)
{
    //qDebug() << "rhsv_value"<<value;
    color1LowH=value;
    ui->label_11->setNum(color1LowH);
    //qDebug() << "color1LowH"<<color1LowH;
}

void Color_Blocks::on_horizontalScrollBar_2_valueChanged(int value)
{
    //qDebug() << "rhsv_value"<<value;
    color1HighH=value;
    ui->label_12->setNum(color1HighH);
    //qDebug() << "color1HighH"<<color1HighH;
}


void Color_Blocks::on_horizontalScrollBar_3_valueChanged(int value)
{
    color1LowS=value;
    ui->label_13->setNum(color1LowS);
    //qDebug() << "color1LowS"<<color1LowS;
}


void Color_Blocks::on_horizontalScrollBar_4_valueChanged(int value)
{
    color1HighS=value;
    ui->label_14->setNum(color1HighS);
    //qDebug() << "color1HighS"<<color1HighS;
}


void Color_Blocks::on_horizontalScrollBar_5_valueChanged(int value)
{
    color1LowV=value;
    ui->label_15->setNum(color1LowV);
    //qDebug() << "color1LowV"<<color1LowV;
}


void Color_Blocks::on_horizontalScrollBar_6_valueChanged(int value)
{
    color1HighV=value;
    ui->label_16->setNum(color1HighV);
    //qDebug() << "color1HighV"<<color1HighV;
}


// int color2LowH = 35, color2HighH = 85, color2LowS = 100, color2HighS = 255, color2LowV = 100, color2HighV = 255;

void Color_Blocks::on_horizontalScrollBar_7_valueChanged(int value)
{
    color2LowH=value;
    ui->label_18->setNum(color2LowH);

    //qDebug() << "color2LowH"<<color2LowH;
}


void Color_Blocks::on_horizontalScrollBar_8_valueChanged(int value)
{
    color2HighH=value;
    ui->label_19->setNum(color2HighH);
    // qDebug() << "color2HighH"<<color2HighH;
}


void Color_Blocks::on_horizontalScrollBar_9_valueChanged(int value)
{
    color2LowS=value;
    ui->label_20->setNum(color2LowS);

    // qDebug() << "color2LowS"<<color2LowS;
}


void Color_Blocks::on_horizontalScrollBar_10_valueChanged(int value)
{
    color2HighS=value;
    ui->label_21->setNum(color2HighS);
    // qDebug() << "color2HighS"<<color2HighS;
}


void Color_Blocks::on_horizontalScrollBar_11_valueChanged(int value)
{
    color2LowV=value;
    ui->label_22->setNum(color2LowV);
    // qDebug() << "color2LowV"<<color2LowV;
}


void Color_Blocks::on_horizontalScrollBar_12_valueChanged(int value)
{
    color2HighV=value;
    ui->label_23->setNum(color2HighV);
    // qDebug() << "color2HighV"<<color2HighV;
}

// int color3LowH = 100, color3HighH = 130, color3LowS = 100, color3HighS = 255, color3LowV = 100, color3HighV = 255;
void Color_Blocks::on_horizontalScrollBar_13_valueChanged(int value)
{
    color3LowH=value;
    ui->label_25->setNum(color3LowH);
    // qDebug() << "color3LowH"<<color3LowH;
}


void Color_Blocks::on_horizontalScrollBar_14_valueChanged(int value)
{
    color3HighH=value;
    ui->label_26->setNum(color3HighH);
    // qDebug() << "color3HighH"<<color3HighH;
}


void Color_Blocks::on_horizontalScrollBar_15_valueChanged(int value)
{
    color3LowS=value;
    ui->label_27->setNum(color3LowS);
    // qDebug() << "color3LowS"<<color3LowS;
}


void Color_Blocks::on_horizontalScrollBar_16_valueChanged(int value)
{
    color3HighS=value;
    ui->label_28->setNum(color3HighS);
    // qDebug() << "color3HighS"<<color3HighS;
}


void Color_Blocks::on_horizontalScrollBar_17_valueChanged(int value)
{
    color3LowV=value;
    ui->label_29->setNum(color3LowV);
    // qDebug() << "color3LowV"<<color3LowV;
}


void Color_Blocks::on_horizontalScrollBar_18_valueChanged(int value)
{
    color3HighV=value;
    ui->label_30->setNum(color3HighV);
    // qDebug() << "color3HighV"<<color3HighV;
}

