#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "ui_mainwindow.h" // 由 Qt Designer 生成的 UI 头文件
#include <QZXing.h>
// 前向声明
class CameraThread;
class QRCodeThread;
class MainWindow2;
class color_blockThread;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void updateImage(const QImage &image); // 更新摄像头图像
    void displayQRCodeResult(const QString &result); // 显示二维码识别结果
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui; // UI 指针
    CameraThread *cameraThread; // 摄像头线程实例
    QRCodeThread *qrCodeThread; // 二维码识别线程实例
    color_blockThread *colorblockThread;//色块识别线程
    QZXing *qZXing; // 二维码识别对象
};

#endif // MAINWINDOW_H
