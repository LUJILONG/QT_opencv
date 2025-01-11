#include "mainwindow.h"// 引入主窗口的头文件，包含主窗口类的定义。
#include "mainwindow2.h"// 引入主窗口的头文件，包含主窗口类的定义。
#include "color_blocks.h"
#include "thread.h"
#include <QZXing.h> // 确保包含 QZXing 的头文件
#include <QSerialPort>
extern MainWindow* globalMainWindow;//全局窗口
extern MainWindow2* globalMainWindow2;//全局窗口
extern Color_Blocks* color_blocks3;//全局窗口色块
QString usartqrresult,usartcolorresult;//定义全局变量 二维码识别结果，色块识别结果
bool isSorteSent;

/*
 * @brief MainWindow::MainWindow
 *        主窗口类的构造函数，接受一个父窗口指针作为参数。
 *        使用初始化列表初始化主窗口的父类 QMainWindow、UI 类指针 ui 和二维码识别对象 qZXing。
 * @param parent 父窗口指针，默认为 nullptr。
 */
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), qZXing(new QZXing(this)) {
    // 调用 UI 类的 setupUi 方法，设置主窗口的界面布局和组件。
    ui->setupUi(this);

    /*-----------------------串口通信-----------------------------*/
    // 创建 QSerialPort 对象，用于串口通信，并将其设置为当前对象的子对象。
    // 手动连接串口的 readyRead 信号到 manual_serialPortReadyRead 槽函数，以便在串口有数据可读时调用该槽函数。
    // 调用 populateSerialPortComboBox 函数，填充串口com下拉列表框。
    serialPort = new QSerialPort(this);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(manual_serialPortReadyRead()));
    populateSerialPortComboBox();


    /*-----------------------相机线程-----------------------------*/
    // 创建并启动CameraThread线程，创建 CameraThread 实例
    // 连接 cameraThread 的 newFrame 信号到 MainWindow 的 updateImage 槽函数，以便在新的摄像头帧可用时更新图像。
    // 启动 cameraThread 线程。
    // 输出调试信息，表示摄像头线程已启动。
    cameraThread = new CameraThread(this);
    connect(cameraThread, &CameraThread::newFrame, this, &MainWindow::updateImage);
    cameraThread->start();
    qDebug() << "Camera thread started";

    /*-----------------------二维码线程-----------------------------*/
    // 创建并启动QRCodeThread线程
    // 连接 qrCodeThread 的 qrCodeDetected 信号到 MainWindow 的 displayQRCodeResult 槽函数，以便在检测到二维码时显示结果。
    // 输出调试信息，指示连接是否成功。
    // 启动 qrCodeThread 线程。
    // 输出调试信息，表示二维码线程已启动。
    qrCodeThread = new QRCodeThread(cameraThread, this);
    bool connects=connect(qrCodeThread, &QRCodeThread::qrCodeDetected, this, &MainWindow::displayQRCodeResult);
    qDebug() << "Connection successful:" << connects;
    qrCodeThread->start();
    qDebug() << "QRCode thread started";

    /*-----------------------色块线程-----------------------------*/
    // 创建并启动 color_blockThread 线程
    // 连接 color_blocks3 的 colors_block_send 信号到 MainWindow 的 displaycolorsResult 槽函数，以便在接收到颜色块识别结果时显示。
    // 启动 colorblockThread 线程。
    // 输出调试信息，表示颜色块线程已启动。
    colorblockThread = new color_blockThread(this);
    connect(color_blocks3, &Color_Blocks::colors_block_send, this, &MainWindow::displaycolorsResult);//色块识别结果
    colorblockThread->start();
    qDebug() << "Color block thread started";

    /*-----------------------定时器-----------------------------*/
    // 创建定时器对象，并将其设置为当前对象的子对象。
    // 连接定时器的 timeout 信号到 MainWindow 的 onTimerTimeout 槽函数，以便在定时器超时时调用该槽函数。
    // 启动定时器，设置超时时间为 1000 毫秒（1 秒）。
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    timer->start(1000);

    // 初始序号为 1
    rowIndex = 1;
}

/*
 * @brief MainWindow::onTimerTimeout
 *        定时器超时处理函数，用于处理接收到的二维码和颜色块识别结果，并通过串口发送相应的数据。
 * @param
 */
void MainWindow::onTimerTimeout()
{
    QByteArray array;       // 用于存储要发送的数据
    QByteArray result;      // 用于存储要发送的数据
    int nums;               // 用于存储处理后的数字结果

    // 如果复选框被选中，表示启用二维码识别结果处理
    if(ui->checkBox->checkState() == Qt::Checked)
    {
        // 调用 handleReceivedData 函数处理接收到的二维码识别结果，并将结果转换为数字
       nums=handleReceivedData(QString(usartqrresult));
       // 根据处理后的数字结果，设置相应的发送数据 0x3a 0x3d num 0x3c // 通过串口发送数据
       if (nums != 0)
       {
           array = QByteArray::number(nums); // 将数字转换为字节数组
           array.append(0x3a);
           array.append(0x3d);
           array.append(array);
           array.append(0x3c);
           serialPort->write(array);
       }
            // 如果复选框被选中，表示启用颜色块识别结果处理
     }else if(ui->checkBox_4->checkState() == Qt::Checked)
           {
                // 根据接收到的颜色块识别结果，设置相应的发送数据
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
                // 如果结果不为空，则通过串口发送数据
                if(result != "")
                {
                    array.append(0x3a);
                    array.append(0x3d);
                    array.append(result);
                    array.append(0x3c);
                    serialPort->write(array);
                }

        }else{// 如果两个复选框都未选中，则输出调试信息表示无数据发送
                qDebug() << "无数据发送";
             }
    // 清空接收到的二维码和颜色块识别结果，准备下一次接收
    usartqrresult="";
    usartcolorresult="";
}

// 更新图像显示的槽函数，用于在主窗口中显示摄像头捕获的实时图像。
void MainWindow::updateImage(const QImage &image){
    ui->label->setPixmap(QPixmap::fromImage(image)); // 将 QImage 对象转换为 QPixmap，并设置给标签控件进行显示
}

// 显示二维码识别结果的槽函数，用于在主窗口中显示二维码识别的结果。
void MainWindow::displayQRCodeResult(const QString &result) {
    // 在标签控件中显示识别结果
    ui->qrCodeLabel->setText("识别结果: " + result);
    // 将识别结果存储到全局变量中，以便在其他地方使用
    usartqrresult=result;
}

// 显示颜色块识别结果的槽函数，用于在主窗口中显示颜色块识别的结果。
void MainWindow::displaycolorsResult(const QString &colors) {
    // 在标签控件中显示识别结果
    ui->label_7->setText("识别结果: " + colors);
    // 将识别结果存储到全局变量中，以便在其他地方使用
    usartcolorresult=colors;
}

// 主窗口类的析构函数。
MainWindow::~MainWindow() {
    qDebug() << "释放";// 输出调试信息，表示正在释放资源
    cameraThread->stop();// 停止摄像头线程
    qrCodeThread->stop();// 停止二维码识别线程
    colorblockThread->stop(); // 停止颜色块识别线程
    delete ui;// 释放 UI 类指针占用的内存
    delete qZXing; // 释放 qZXing 对象占用的内存
}

//切换进入系统界面
void MainWindow::on_pushButton_clicked()
{
    qDebug() << "切换页面按钮被点击";
    if (globalMainWindow2) {
        this->close(); // 关闭当前主窗口
        globalMainWindow2->show(); // 显示另一个窗口实例
    } else {
        qDebug() << "globalMainWindow2 未初始化";
    }
}

//切换色块界面
void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << "切换色块页面按钮被点击";
    if (color_blocks3) {
        this->close(); // 关闭当前主窗口
        color_blocks3->show(); // 显示另一个窗口实例
    } else {
        qDebug() << "color_blocks3 未初始化";
    }
}

//获取串口com下拉按钮
void MainWindow::populateSerialPortComboBox() {
     // 获取系统中所有可用的串口信息
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
     // 如果串口列表为空，输出调试信息
    if (portList.isEmpty()) {
        qDebug() << "没有可用的串口";
    } else {
        // 遍历串口列表，将每个串口的名称添加到下拉列表框中
        foreach (const QSerialPortInfo &info, portList) {
        ui->comboBox->addItem(info.portName());
        }
    }
}

/*打开串口按钮
 * @brief MainWindow::on_openbt_clicked
 *        处理打开或关闭串口的槽函数
 *        该函数根据用户在界面上的选择，初始化并打开或关闭串口。
 *        它首先获取用户选择的串口参数，包括波特率、数据位、停止位和奇偶校验位，
 *        然后将这些参数应用到串口对象上，并尝试打开或关闭串口。
 *        如果成功打开串口，它会更新按钮文本和下拉列表的状态。
 */
void MainWindow::on_openbt_clicked()
{
    /*串口初始化参数定义*/
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity checkBits;

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
        //尝试打开串口
        if(serialPort->open(QIODevice::ReadWrite) == true){
            //改变串口文本
            ui->openbt->setText("关闭串口");
            // 禁用端口号下拉框
            ui->comboBox->setEnabled(false);
        }

    }else{
        //如果字体不是打开串口则关闭串口
        serialPort->close();
        //改变串口文本
        ui->openbt->setText("打开串口");
        // 启用端口号下拉框
        ui->comboBox->setEnabled(true);
    }
}

//发送数据按钮
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
    serialPort->write(array);

}

/*实现接收数据函数*/
void MainWindow::manual_serialPortReadyRead()
{
    // 读取串口接收到的所有数据
    QByteArray recBuf = serialPort->readAll();
    QString str_rev;
    str_rev = QString(recBuf);
    // 将接收的数据插入到接收文本框中并移动光标到末尾
    ui->recvEdit->insertPlainText(str_rev);
    ui->recvEdit->moveCursor(QTextCursor::End);
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

//清除接收文本框
void MainWindow::on_pushButton_5_clicked()
{
    ui->recvEdit->clear();
}

//清除表格
void MainWindow::on_pushButton_4_clicked()
{
    ui->tableWidget->setRowCount(0);
    rowIndex = 1;
}

//打开传送带按钮
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
    //串口发送
    serialPort->write(data);
}

