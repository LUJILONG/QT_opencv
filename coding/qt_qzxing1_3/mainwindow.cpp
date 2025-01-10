#include "mainwindow.h"// 引入主窗口的头文件，包含主窗口类的定义。
#include "mainwindow2.h"// 引入主窗口的头文件，包含主窗口类的定义。
#include "color_blocks.h"
#include "thread.h"
#include <QZXing.h> // 确保包含 QZXing 的头文件
#include <QSerialPort>
extern MainWindow* globalMainWindow;
extern MainWindow2* globalMainWindow2;
extern Color_Blocks* color_blocks3;
QString usartqrresult,usartcolorresult;;
bool isSorteSent;
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), qZXing(new QZXing(this)) {
    // 主窗口类的构造函数，接受一个父窗口指针作为参数。使用初始化列表初始化主窗口的父类 QMainWindow、UI 类指针 ui 和二维码识别对象 qZXing。
    ui->setupUi(this); // 调用 UI 类的 setupUi 方法，设置主窗口的界面布局和组件。
    serialPort = new QSerialPort(this);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(manual_serialPortReadyRead()));/*手动连接槽函数*/
    qDebug() << "xuhao chufadi";
    populateSerialPortComboBox();
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
    // if (!color_blocks3) {
    //     qDebug() << "color_blocks3 is not initialized";
    // } else {
    //     bool connected = connect(colorblockThread, &color_blockThread::color_blockFrame, color_blocks3, &Color_Blocks::updateImages);
    //     Q_ASSERT(connected);
    //     qDebug() << "Connection successful:" << connected;
    // }
    // connect(colorblockThread, &color_blockThread::color_blockFramehsv, color_blocks3, &Color_Blocks::updateImageshsv);
    connect(color_blocks3, &Color_Blocks::colors_block_send, this, &MainWindow::displaycolorsResult);//色块识别结果
    colorblockThread->start();
    qDebug() << "Color block thread started";

    timer = new QTimer(this);//建立定时器
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    timer->start(1000); // 每 1000 毫秒（1 秒）触发一次定时器
    // 初始序号为 1
    rowIndex = 1;
    // // 创建表头字符串列表
    // QStringList headers;
    // headers << "序号" << "出发地" << "目的地" << "物品类型";

    // // 设置表格的水平表头为上述字符串列表
    // ui->tableWidget->setHorizontalHeaderLabels(headers);
    // if (ui->tableWidget->horizontalHeaderItem(0))
    // {
    //     ui->tableWidget->horizontalHeaderItem(0)->setText("序号");
    // }
    // else
    // {
    //     qDebug() << "Header item at index 0 is null. Unable to set text.";
    // }
}

void MainWindow::onTimerTimeout()
{
    QByteArray array;
    QByteArray result;
    int nums;
    if(ui->checkBox->checkState() == Qt::Checked){

       nums=handleReceivedData(QString(usartqrresult));//处理识别到的结果
        // if (usartqrresult == "青山市-云庭市-纺织品类")
        // {
        //     result = "1";
        // }
        // else if (usartqrresult == "霜花市-碧水市-日用品类")
        // {
        //     result = "2";
        // }
        // else if(usartqrresult == "青阳市-白云市-厨具类")
        // {
        //     result = "3";
        // }
        // else if(usartqrresult == "延桐市-白云市-服装类")
        // {
        //     result = "3";
        // }
        // else if(usartqrresult == "青阳市-白云市-易碎品-1")
        // {
        //     result = "3";
        // }
        // else
        // {
        //     result = "";

        // }
       if (nums!=0)
       {
           array =QByteArray::number(nums);
           QString::number(nums);
           array.append(0x3a);
           array.append(0x3d);
           array.append(array);
           array.append(0x3c);
           // qDebug() << "Timer："<<nums;
           serialPort->write(array);
       }

    }else if(ui->checkBox_4->checkState() == Qt::Checked){
        // qDebug() << "color";
        // array = usartcolorresult.toLocal8Bit().data();
        if (usartcolorresult == "color1")
        {
            result = "1";
        }
        else if (usartcolorresult == "color2")
        {
            result = "3";
        }
        else if(usartcolorresult == "color3")
        {
            result = "2";
        }
        else
        {
            result = "";

        }
        if(result != "")
        {
            array.append(0x3a);
            array.append(0x3d);
            array.append(result);
            array.append(0x3c);
            serialPort->write(array);
        }


        // }
    }else{
        qDebug() << "无数据发送";
    }

    usartqrresult="";
    usartcolorresult="";
    // array = usartcolorresult.toLocal8Bit().data();
    // serialPort->write(array);
    // serialPort->write(usartcolorresult);
}


void MainWindow::updateImage(const QImage &image){
    ui->label->setPixmap(QPixmap::fromImage(image)); // 更新实时摄像头图像
}

void MainWindow::displayQRCodeResult(const QString &result) {
    // handleReceivedData(QString(result));//处理识别到的结果
    ui->qrCodeLabel->setText("识别结果: " + result); // 显示二维码结果
    usartqrresult=result;
    // result = QString();
}
void MainWindow::displaycolorsResult(const QString &colors) {
    ui->label_7->setText("识别结果: " + colors); // 显示色块结果
    usartcolorresult=colors;

    // qDebug() << "color";
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
        // delete w;

        //
        // cameraThread->start();
        // qrCodeThread->start();
        // colorblockThread->start();
        // timer->start(1000);
        // cameraThread->stop();
        // qrCodeThread->stop();
        // colorblockThread->stop();
        // timer->stop();
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

void MainWindow::populateSerialPortComboBox() {
    // qDebug() << "com";
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    if (portList.isEmpty()) {
        qDebug() << "没有可用的串口";
    } else {
        foreach (const QSerialPortInfo &info, portList) {
            ui->comboBox->addItem(info.portName());
        }
    }
}

void MainWindow::on_openbt_clicked()
{
    /*串口初始化*/
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity checkBits;
    qDebug() << "ssss";
    // 获取串口波特率
    if(ui->comboBox_2->currentText()=="1200")
        baudRate=QSerialPort::Baud1200;
    else if(ui->comboBox_2->currentText()=="2400")
        baudRate=QSerialPort::Baud2400;
    else if(ui->comboBox_2->currentText()=="4800")
        baudRate=QSerialPort::Baud4800;
    else if(ui->comboBox_2->currentText()=="9600")
        baudRate=QSerialPort::Baud9600;
    else if(ui->comboBox_2->currentText()=="19200")
        baudRate=QSerialPort::Baud19200;
    else if(ui->comboBox_2->currentText()=="38400")
        baudRate=QSerialPort::Baud38400;
    else if(ui->comboBox_2->currentText()=="57600")
        baudRate=QSerialPort::Baud57600;
    else if(ui->comboBox_2->currentText()=="115200")
        baudRate=QSerialPort::Baud115200;

    // 获取串口数据位
    if(ui->comboBox_3->currentText()=="5")
        dataBits=QSerialPort::Data5;
    else if(ui->comboBox_3->currentText()=="6")
        dataBits=QSerialPort::Data6;
    else if(ui->comboBox_3->currentText()=="7")
        dataBits=QSerialPort::Data7;
    else if(ui->comboBox_3->currentText()=="8")
        dataBits=QSerialPort::Data8;

    // 获取串口停止位
    if(ui->comboBox_4->currentText()=="1")
        stopBits=QSerialPort::OneStop;
    else if(ui->comboBox_4->currentText()=="1.5")
        stopBits=QSerialPort::OneAndHalfStop;
    else if(ui->comboBox_4->currentText()=="2")
        stopBits=QSerialPort::TwoStop;

    // 获取串口奇偶校验位
    if(ui->comboBox_5->currentText() == "none"){
        checkBits = QSerialPort::NoParity;
    }else if(ui->comboBox_5->currentText() == "奇校验"){
        checkBits = QSerialPort::OddParity;
    }else if(ui->comboBox_5->currentText() == "偶校验"){
        checkBits = QSerialPort::EvenParity;
    }else{

    }

    // 初始化串口属性
    serialPort->setPortName(ui->comboBox->currentText());
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setStopBits(stopBits);
    serialPort->setParity(checkBits);

    // 根据初始化好的串口属性，打开串口
    if(ui->openbt->text() == "打开串口"){
        if(serialPort->open(QIODevice::ReadWrite) == true){
            ui->openbt->setText("关闭串口");
            // 禁用端口号下拉框
            ui->comboBox->setEnabled(false);
            // qDebug() << "ok";

        }

    }else{
        serialPort->close();
        ui->openbt->setText("打开串口");
        // 启用端口号下拉框
        ui->comboBox->setEnabled(true);
        // 更新状态栏显示端口状态
    }
}
void MainWindow::on_pushButton_2_clicked()
{
    QByteArray array;

    // 如果选中 Hex 复选框
    if(ui->chk_send_hex->checkState() == Qt::Checked){
        // 将发送文本框中的十六进制字符串转换为字节数组
        array = QByteArray::fromHex(ui->sendEdit->toPlainText().toUtf8()).data();
    }else{
        // 将发送文本框中的 ASCII 字符串转换为字节数组
        array = ui->sendEdit->toPlainText().toLocal8Bit().data();
    }

    if(ui->chk_send_line->checkState() == Qt::Checked){
        array.append("\r\n");
    }
    // 发送数据并获取发送的字节长度
    // int a = serialPort->write(array);
    serialPort->write(array);

}

/*手动实现接收数据函数*/
void MainWindow::manual_serialPortReadyRead()
{
    // qDebug() << "xyz";
    // 读取串口接收到的所有数据
    QByteArray recBuf = serialPort->readAll();
    QString str_rev;

    str_rev = QString(recBuf);

    // 将接收的数据插入到接收文本框中并移动光标到末尾
    ui->recvEdit->insertPlainText(str_rev);
    ui->recvEdit->moveCursor(QTextCursor::End);
    // ui->recvEdit->insertPlainText(str_rev);

}



// 处理接收到的数据的函数
int MainWindow::handleReceivedData(QString data)
{
    int num;
    // 将接收到的数据按照'-'分割成出发地、目的地和物品类型三个部分，并存储为字符串列表
    QStringList parts = data.split('-');
    if (parts.size() == 3 || parts.size() == 4)
    {
        // 获取当前表格的行数
        int currentRow = ui->tableWidget->rowCount();
        // 在表格中插入新的一行
        ui->tableWidget->insertRow(currentRow);

        // 设置序号，将当前序号转换为字符串并创建表格项
        QTableWidgetItem *indexItem = new QTableWidgetItem(QString::number(rowIndex));
        // 将序号表格项设置到表格的第一列（序号列）
        ui->tableWidget->setItem(currentRow, 0, indexItem);

        // 设置出发地，创建出发地表格项并设置到表格的第二列
        QTableWidgetItem *departureItem = new QTableWidgetItem(parts[0]);
        ui->tableWidget->setItem(currentRow, 1, departureItem);

        // 设置目的地，创建目的地表格项并设置到表格的第三列
        QTableWidgetItem *destinationItem = new QTableWidgetItem(parts[1]);
        ui->tableWidget->setItem(currentRow, 2, destinationItem);
        // qDebug() << "目的地"<<parts[1];
        if (parts[1] == "云庭市")
        {
            num = 1;
        }
        else if (parts[1] == "碧水市")
        {
            num = 2;
        }
        else if(parts[1] == "白云市")
        {
            num = 3;
        }else
        {
            num = 0;

        }
        if (parts.size() == 4)
        {
            // 如果有4个部分，将第三个和第四个部分合并
            QString combinedType = parts[2] + "-" + parts[3];
            QTableWidgetItem *typeItem = new QTableWidgetItem(combinedType);
            ui->tableWidget->setItem(currentRow, 3, typeItem);
        }
        else
        {
            // 设置物品类型
            QTableWidgetItem *typeItem = new QTableWidgetItem(parts[2]);
            ui->tableWidget->setItem(currentRow, 3, typeItem);
        }

        // 序号自增
        rowIndex++;
        // 检查表格是否需要滚动
        QRect rect = ui->tableWidget->viewport()->rect();
        int visibleRows = rect.height() / ui->tableWidget->rowHeight(0);
        if (currentRow >= visibleRows)
        {
            // 滚动到最后一行
            ui->tableWidget->scrollToBottom();
        }
    }
    else
    {
        // 如果接收到的数据格式不正确，打印错误信息
        qDebug() << "Invalid data format:"<<data;
    }
    return num;
}
void MainWindow::on_pushButton_5_clicked()
{
    ui->recvEdit->clear();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->tableWidget->setRowCount(0);
    rowIndex = 1;
}


void MainWindow::on_pushButton_6_clicked()
{
    QByteArray data;
    if (!isSorteSent) {
        data.append(0x3a);
        data.append(0x3b);
        data.append("sorte");
        data.append(0x3c);
        isSorteSent = true;
        ui->pushButton_6->setText("关闭传送带");
    } else {
        data.append(0x3a);
        data.append(0x3b);
        data.append("exit");
        data.append(0x3c);
        isSorteSent = false;
        ui->pushButton_6->setText("打开传送带");
    }

    serialPort->write(data);


}

