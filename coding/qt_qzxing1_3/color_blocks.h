#ifndef COLOR_BLOCKS_H
#define COLOR_BLOCKS_H

#include <QMainWindow>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QTimer>

namespace Ui {
class Color_Blocks;
}

class Color_Blocks : public QMainWindow
{
    Q_OBJECT

public:
    explicit Color_Blocks(QWidget *parent = nullptr);
    ~Color_Blocks();
    // 更新 HSV 图像的处理函数，处理实时摄像头帧并更新显示
    void updateFramehsvs(cv::Mat frame);

    // 检测并绘制轮廓，返回检测到的状态
    bool detectAndDrawContours(cv::Mat &frame, const cv::Mat &mask, const cv::Scalar &color);
    void qlabels();
// public slots:
//     void updateImages(const QImage &image);
//     void updateImageshsv(const QImage &image);
signals:
    void colors_block_send(const QString &colors);
private slots:
    void on_pushButton_clicked();
    // void processImageFromLabel(); // 新增槽函数处理 QLabel 的图像

    // void on_horizontalScrollBar_actionTriggered(int action);

    void on_horizontalScrollBar_valueChanged(int value);

    void on_horizontalScrollBar_2_valueChanged(int value);

    void on_horizontalScrollBar_3_valueChanged(int value);

    void on_horizontalScrollBar_4_valueChanged(int value);

    void on_horizontalScrollBar_5_valueChanged(int value);

    void on_horizontalScrollBar_6_valueChanged(int value);

    void on_horizontalScrollBar_7_valueChanged(int value);

    void on_horizontalScrollBar_8_valueChanged(int value);

    void on_horizontalScrollBar_9_valueChanged(int value);

    void on_horizontalScrollBar_10_valueChanged(int value);

    void on_horizontalScrollBar_11_valueChanged(int value);

    void on_horizontalScrollBar_12_valueChanged(int value);

    void on_horizontalScrollBar_13_valueChanged(int value);

    void on_horizontalScrollBar_14_valueChanged(int value);

    void on_horizontalScrollBar_15_valueChanged(int value);

    void on_horizontalScrollBar_16_valueChanged(int value);

    void on_horizontalScrollBar_17_valueChanged(int value);

    void on_horizontalScrollBar_18_valueChanged(int value);

    void on_horizontalScrollBar_19_valueChanged(int value);

    void on_horizontalScrollBar_20_valueChanged(int value);

private:
    Ui::Color_Blocks *ui;
    cv::VideoCapture cap;

    // HSV 阈值和启用复选框
    int color1LowH = 163, color1HighH = 180, color1LowS = 138, color1HighS = 255, color1LowV = 85, color1HighV = 255;
    int color2LowH = 46, color2HighH = 103, color2LowS = 53, color2HighS = 146, color2LowV = 77, color2HighV = 178;
    int color3LowH = 86, color3HighH = 154, color3LowS = 36, color3HighS = 178, color3LowV = 28, color3HighV = 138;
    int cokor_blockmax=100000,cokor_blockmin=500;
    // int color1LowH = 0, color1HighH = 10, color1LowS = 100, color1HighS = 255, color1LowV = 100, color1HighV = 255;
    // int color2LowH = 35, color2HighH = 85, color2LowS = 100, color2HighS = 255, color2LowV = 100, color2HighV = 255;
    // int color3LowH = 100, color3HighH = 130, color3LowS = 100, color3HighS = 255, color3LowV = 100, color3HighV = 255;
    QCheckBox *color1Enabled = nullptr;
    QCheckBox *color2Enabled = nullptr;
    QCheckBox *color3Enabled = nullptr;

};







#endif // COLOR_BLOCKS_H
