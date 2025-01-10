#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QZXing.h>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void captureFrame(); // 捕获帧的槽函数

private:
    Ui::MainWindow *ui;
    cv::VideoCapture cap; // 摄像头对象
    QTimer *timer; // 定时器
    QZXing *qZXing; // QZXing对象
};

#endif // MAINWINDOW_H
