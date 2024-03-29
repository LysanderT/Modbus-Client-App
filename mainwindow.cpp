#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <iostream>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QProgressDialog>
#include <QTextEdit>
#include <QModelIndex>
#include <endianness.h>
#include <QtXml>
#include <QDomDocument>

// static look-up table (CRC-16)
static QVector<unsigned char> CRC_lo = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
};
static QVector<unsigned char> CRC_hi = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40
} ;
// function declaration
void append_CRC(QByteArray&);

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    serial = new QSerialPort();
    socket = new QTcpSocket();
    initSetup();
    initTabelview();
    initConnection();
//    //test
//    char x= 0x0a;
//    int y=x;
//    qDebug() << y;
//    QByteArray z=QByteArray::fromHex("0bcd");
//    qDebug() << z;
//    int g = z.at(0);
//    qDebug() << g;
//    QString h = QString("%1").arg(g,0,16);
//    qDebug() << h;
//    QString f = QString::number(g,16);
//    qDebug() << f;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serial;
    delete model;
}

//void MainWindow::_changeText(){
//    switch (ui->comboBox_func->currentText().toInt()) {
//    case 1:
//        ui->label_x->setText(QStringLiteral("起始地址："));
//        ui->label_y->setText(QStringLiteral("线圈数量："));
//        break;
//    case 2:
//        ui->label_x->setText(QStringLiteral("起始地址："));
//        ui->label_y->setText(QStringLiteral("输入数量："));
//        break;
//    case 3:
//        ui->label_x->setText(QStringLiteral("字节数："));
//        ui->label_y->setText(QStringLiteral("寄存器值："));
//        break;
//    case 4:
//        ui->label_x->setText(QStringLiteral("起始地址："));
//        ui->label_y->setText(QStringLiteral("输入寄存器数量："));
//        break;
//    case 5:
//        ui->label_x->setText(QStringLiteral("输出地址："));
//        ui->label_y->setText(QStringLiteral("输出值："));
//        break;
//    case 6:
//        ui->label_x->setText(QStringLiteral("寄存器地址："));
//        ui->label_y->setText(QStringLiteral("寄存器值："));
//        break;
//    default:
//        qDebug() << "error in slot: changeText().";
//    }
//}

void MainWindow::openPort(){
    if(ui->tabWidget->currentIndex()==0){
        qDebug() << ui->rtu_button->text();
        if (ui->rtu_button->text() == tr("打开串口")){
            serial->setPortName(ui->comboBox_port->currentText());
            baudRate=ui->comboBox_baudRate->currentText().toInt();
            serial->setBaudRate(baudRate);
            serial->setDataBits(QSerialPort::Data8);
            switch (ui->comboBox_parity->currentText().toInt()) {
                case 0:
                    serial->setParity(QSerialPort::NoParity);
                    // **if no parity, add one stopbit to fill the frame.
                    serial->setStopBits(QSerialPort::TwoStop);
                    break;
                case 2:
                    serial->setParity(QSerialPort::EvenParity);
                    serial->setStopBits(QSerialPort::OneStop);
                    break;
                case 3:
                    serial->setParity(QSerialPort::OddParity);
                    serial->setStopBits(QSerialPort::OneStop);
                    break;
            }
            // open the serial port
            if (serial->open(QIODevice::ReadWrite)){
                ui->comboBox_port->setEnabled(false);

                ui->comboBox_baudRate->setEnabled(false);
                ui->comboBox_parity->setEnabled(false);
                ui->tabWidget->tabBar()->setTabEnabled(1,false);
                ui->ModbusButton->setEnabled(true);
                ui->rtu_button->setText(tr("关闭串口"));
                // ***
                connect(serial, &QSerialPort::readyRead, this, &MainWindow::_receiveData);
                qDebug() << "serial handle:" << serial->handle();
            }else{
                qDebug() << "error: fail to open";
                QMessageBox::critical(this, "连接失败","\n串口打开失败！请检查驱动版本是否正确以及串口占用情况。");
            }
        }else{
            //close the serial port
            //serial->clear();
            serial->close();
            //serial->deleteLater();
            ui->comboBox_port->setEnabled(true);

            ui->comboBox_baudRate->setEnabled(true);
            ui->comboBox_parity->setEnabled(true);
            ui->tabWidget->tabBar()->setTabEnabled(1,true);
            ui->rtu_button->setText(tr("打开串口"));
            ui->ModbusButton->setEnabled(false);
            disconnect(serial, &QSerialPort::readyRead, this, &MainWindow::_receiveData);
        }
    }else if(ui->tabWidget->currentIndex()==1){
        if(ui->tcp_button->text()==tr("TCP连接")){
            QString address = ui->lineEdit_address->text().toLatin1();
            qDebug() << "address:" << address;
            quint16 port = ui->lineEdit_port->text().toUShort();
            qDebug() << "port:" << port;
            socket->connectToHost(address,port);
            if(socket->waitForConnected(3000)){
                qDebug() << "TCP successfully connected.";
                ui->lineEdit_address->setEnabled(false);
                ui->lineEdit_port->setEnabled(false);
                ui->lineEdit_delay->setEnabled(false);
                ui->tabWidget->tabBar()->setTabEnabled(0,false);
                ui->ModbusButton->setEnabled(true);
                ui->tcp_button->setText(tr("断开连接"));
                connect(socket, &QTcpSocket::readyRead, this, &MainWindow::_receiveData);
                connect(socket,&QAbstractSocket::disconnected,this,&MainWindow::_hostError);
            }else{
                qDebug() << "fail to connect TCP";
                socket->disconnectFromHost();
                QMessageBox::critical(this, "连接超时","\n连接失败！请检查ip地址/端口号是否正确以及服务器在线情况。");
            }
        }else{
            // break the TCP connection
            disconnect(socket,&QAbstractSocket::disconnected,this,&MainWindow::_hostError);
            socket->abort();
            if(socket->state()==QTcpSocket::UnconnectedState){
                qDebug() << "TCP connection broken";
                ui->lineEdit_address->setEnabled(true);
                ui->lineEdit_port->setEnabled(true);
                ui->lineEdit_delay->setEnabled(true);
                ui->tabWidget->tabBar()->setTabEnabled(0,true);
                ui->ModbusButton->setEnabled(false);
                ui->tcp_button->setText(tr("TCP连接"));
                disconnect(socket, &QTcpSocket::readyRead, this, &MainWindow::_receiveData);
            }else{
                qDebug() << "fail to break TCP connection";
                QMessageBox::critical(this,"错误","\n断开TCP连接失败！");
            }
        }
    }
}

void Delay_MSec(unsigned int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void MainWindow::sendData(){
    if(model->rowCount()==0){return;}
    row_mp.clear(); // reset row_mp
    receive_index = 0;  // ***reset receive_index
    QProgressDialog progress("进度","停止",0,model->rowCount());
    progress.setWindowTitle("进度");
//    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    progress.setAttribute(Qt::WA_DeleteOnClose,true);
    int delay = ui->tabWidget->currentIndex()==0?3.5*11*1000/baudRate+1:ui->lineEdit_delay->text().toInt();

    QString last_slave = model->data(0,1).toString();
    QString last_funccode = model->data(0,2).toString();
    int last_address = model->data(0,3).toString().toInt(nullptr,16);
//    int begin_address = last_address; // the leading one of one frame
    int begin_index = 0;
//    int last_length = model->data(0,4).toString().toInt();

   // 或许要考虑长度 int last unit = x;
    for(int i=1; i<=model->rowCount();i++){
        QString curr_slave = model->data(i,1).toString();
        QString curr_funccode = model->data(i,2).toString();
        int curr_address = model->data(i,3).toString().toInt(nullptr,16);
//        int curr_length = model->data(i,4).toString().toInt();

        progress.setValue(i-1);

//        if(i!=model->rowCount()&&curr_slave==last_slave&&curr_funccode==last_funccode&&curr_address==last_address+last_length)
        if(i!=model->rowCount()&&curr_funccode!="05"&&curr_funccode!="06"&&curr_slave==last_slave&&curr_funccode==last_funccode&&curr_address==last_address+1)
        {// successive query
            last_address = curr_address;
//            last_length = curr_length;
            continue;
        }
        int count = i-begin_index;

        qDebug() << "i:" << i;
        qDebug() << "curr_address:" << curr_address;
        qDebug() << "last_address:" << last_address;
//        qDebug() << "last_length:" << last_length;
//        qDebug() << "begin_address:" << begin_address;
        qDebug() << "count:" << count;

        last_slave=curr_slave;
        last_funccode=curr_funccode;
        last_address=curr_address;
//        last_length = curr_length;

//        begin_address = curr_address;
        bool res = constructFrame(begin_index,count);

        if(res){
            qDebug() << qPrintable("第"+QString::number(begin_index+1))<<"~"<<i<<"行报文发送成功";
        }else{
            QString output=qPrintable("第"+QString::number(begin_index+1)+"行报文发送失败");
            qDebug() << output;
            QMessageBox::critical(this, "错误",output);
        }
        begin_index = i;
        QCoreApplication::processEvents();
        QThread::msleep(delay);
        if(progress.wasCanceled()) break;
    }
    progress.setValue(model->rowCount());
//    for(int i=0; i<model->rowCount(); i++) {
//        // process task
//        bool res = constructFrame(i);
//        if(res){
//            qDebug() << "第"+QString::number(i)+"次报文发送成功";
//        }else{
//            QString output="第"+QString::number(i)+"次报文发送失败";
//            qDebug() << output;
//            QMessageBox::critical(this, "错误",output);
//        }
//        // increment
//        progress.setValue(i);
//        QCoreApplication::processEvents();
//        QThread::msleep(delay);
//        if(progress.wasCanceled()) break;
//    }
    qDebug() << "发报结束";
}

//void MainWindow::_click_ModbusButton_Test(){
//    qDebug() << "发送报文";
////    int delay = 3.5*11*1000/baudRate+1;
//    QString str;
//    if(mode==1){
//        str.append(QString("%1").arg(transaction,4,16,QLatin1Char('0')));
//        transaction+=1;
//        str.append("0000"); // label: modbus
//        str.append("0006"); // for single read/write
//    }
//    str.append(ui->lineEdit_slave->text());
//    str.append(ui->comboBox_func->currentText());
//    str.append(ui->lineEdit_x->text());
//    str.append(ui->lineEdit_y->text());
//    QByteArray frame = QByteArray::fromHex(str.toLatin1());
//    int res=0;
//    if(mode==0){
//        append_CRC(frame);
//        res = serial->write(frame);
//        serial->flush();
//    }else if(mode==1){
//        res = socket->write(frame);
//        socket->flush();
//    }
//    if(res){
//        qDebug() << "successfully sent.";
//    }
//    else{qDebug() << "fail to send.";}

////--------another implementation--------
////    QByteArray frame;
////    frame.append(ui->lineEdit_slave->text().toLatin1());
////    frame.append(ui->comboBox_func->currentText().toLatin1());
////    frame.append(ui->lineEdit_x->text().toLatin1());
////    frame.append(ui->lineEdit_y->text().toLatin1());
////    frame = QByteArray::fromHex(frame);
////-------------
////    QThread::msleep(delay);
//}

void MainWindow::_click_PlusButton(){
//    int index = model->rowCount()-1;
//    // add row
//    model->setRowCount(index+2);
//    // add funcCode
//    QComboBox* cmb = new QComboBox();
//    list_funcCode.append(cmb);
//    cmb->addItems({"01", "02","03","04","05","06"});
//    cmb->setCurrentIndex(list_funcCode.at(index-1)->currentIndex());
//    ui->tableView->setIndexWidget(model->index(index, 2), cmb);
//    // add dataType
//    QComboBox* dat = new QComboBox();
//    list_dataType.append(dat);
//    dat->addItems({"整型","小数","布尔型","无符号整数"});
//    dat->setCurrentIndex(list_dataType.at(index-1)->currentIndex());
//    ui->tableView->setIndexWidget(model->index(index, 5), dat);
//    // add minus button
//    QPushButton*pub = new QPushButton();
//    list_minusButton.append(pub);
//    pub->setText("-");
//    ui->tableView->setIndexWidget(model->index(index,0),pub);
//    ui->tableView->setColumnWidth(0, 20);
//    pub->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    connect(pub,&QPushButton::clicked,this,[=](){ // must be '=' instead of '&' !!!
//        bool flag = true;
//        for(int i=0; i<list_minusButton.size(); i++){
//            if(list_minusButton.at(i)==pub){
//                emit click_minus(i);
//                flag = false;
//                break;
//            }
//        }
//        if(flag) qDebug() << "click_minus EMITION ERROR.";
//    });
//    // set default of 从站地址
//    int unit = model->item(index-1,4)->text().toInt(nullptr,16);
//    QString str = QString("%1").arg(unit,2,16,QLatin1Char('0'));
//    model->setItem(index, 1, new QStandardItem(str));
//    // set default of 单位长度
//    unit = model->item(index-1,4)->text().toInt();
//    str = QString::number(unit);
//    model->setItem(index, 4, new QStandardItem(str));
//    // set default of 起始地址
//    str = QString::number(unit,16);
//    int x = model->item(index-1,3)->text().toInt(nullptr,16);
//    int y = str.toInt(nullptr,16);
//    str =  QString("%1").arg(x+y,4,16,QLatin1Char('0'));//value为int型或char型都可
//    model->setItem(index,3,new QStandardItem(str));
//    // set default of 变比
//    QString str2 = QString("%1").arg(model->item(index-1,6)->text().toFloat());
//    model->setItem(index, 6, new QStandardItem(str2));
//    // reset plusButton
//    plusButton = new QPushButton();
//    plusButton->setText("+");
//    ui->tableView->setIndexWidget(model->index(index+1,0),plusButton);
//    connect(plusButton,SIGNAL(clicked()),this,SLOT(_click_PlusButton()));
//    // mv the slider of verticalScrollBar to the bottom
//    ui->tableView->scrollToBottom();
////    // debug
////    if(model->data(model->index(index-1,6)).isValid()){
////    qDebug() << model->item(index-1,6)->text();
////    }else{
////        qDebug() << "data invalid!";
////    }
//    qDebug() << model->data(model->index(index,2));
//    qDebug() << "add line:" << index+1;
}

void MainWindow::_click_MinusButton(int msg){
// flow: clicked() -> lambda func -> emit click_minus(index) -> _click_MinusButton(index)
    if(!model->removeRow(msg)){
        qDebug() << "REMOVEROW ERROR";
    }

    qDebug() << "remove line:" << msg+1;
}

void MainWindow::_click_ExportButton(){
    Dialog* d = new Dialog(this);
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->exec();
}

void MainWindow::_click_EndianButton(){
    Endianness* d = new Endianness(this);
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->exec();
}

void MainWindow::_receiveData(){
//    QString x = "19";
//    int y = x.toInt(nullptr,16);
//    x=x.setNum(y,2);
//    x=QString("%1").arg(x,8,QLatin1Char('0'));
//    qDebug() << x;


//    QByteArray x = QByteArray::fromHex("9911");
//    int y = x.at(0)&0xff; // byte存的是char，会变成负数，这样可以转正
//    QString z = QString("%1").arg(y,8,2,QLatin1Char('0'));
//    qDebug() << x;
    QByteArray buf;
    int mode = ui->tabWidget->currentIndex();
    int prefix_length;
    if(ui->tabWidget->currentIndex()==1){
        buf = socket->readAll();
        prefix_length = 6; // MBAP
    }
    else if(ui->tabWidget->currentIndex()==0){
        buf = serial->readAll();
        prefix_length = 0;
    }
    if(buf.isEmpty()){return;}
    receiveBuf.append(buf);

    while(true){ // loop to process data in receiveBuf in a timely manner
        qDebug() << "size: " << receiveBuf.size();
        qDebug() << "reciveBuf:" << receiveBuf;
        if(receiveBuf.size()<prefix_length){break;}
        int num = receiveBuf.at(prefix_length-1)&0xff; // guarantee positive
        qDebug() << "num: " << num;
        if(receiveBuf.size()<prefix_length+num){break;}
    //                    qDebug() << "bytearray:" << receiveBuf;
    //                    QString tmp = receiveBuf.sliced(6+num-2,2).toHex();
    //                    qDebug() << "string:" << tmp;
    //                    int val = tmp.toInt(nullptr,16);
            // actually need data type identification

        qDebug() << "----------data received--------------";
        // ----------log info-----------
        ui->textBrowser->append("R:"+receiveBuf.sliced(0,prefix_length+num).toHex(' '));
        //ui->textBrowser->append("\n");
        // -----------------------------
        qDebug() << "???";
        int validbyte = receiveBuf.at(prefix_length-1+3)&0xff;
//        QString curr_data = QString::number(prefix_length,16);
        int tcp_index = (receiveBuf.at(0)&0xff)*32;
        tcp_index+=receiveBuf.at(1)&0xff;
        //        tcp_index=row_mp[tcp_index];
        int index=ui->tabWidget->currentIndex()==1?tcp_index:receive_index;

        QString func = model->data(index,2).toString();
        qDebug() << "func:" << func;
        if(func=="01"||func=="02"){
            QString curr_data;
            QString biArray = "";
            for(int i=prefix_length-1+4;i<prefix_length-1+4+validbyte;i++)
            {
                qDebug() << "loop1";
                int int_byte = receiveBuf.at(i)&0xff;
                QString str_byte = QString("%1").arg(int_byte,8,2,QLatin1Char('0'));
                biArray = str_byte + biArray;
            }
            qDebug() << "biArray:" << biArray;
            int i=biArray.size()-1;
//            int len;
            while(i>=0&&index<model->rowCount()){
                qDebug() << "loop2";
//                if(func=="01"||func=="02"){len=1;}
//                else if(func=="03"||func=="04"){len = model->data(index,4).toString().toInt()*8;}
//                qDebug() << "len:" << len;
                QString str_data = biArray.at(i);
                int int_data = str_data.toInt(nullptr,2);
                model->setData(model->index(index,9),int_data);
//                QString str_data = biArray.mid(i-len+1,len);
//                str_data = QString("%1").arg(str_data,len,QLatin1Char('0'));
//                int int_data = str_data.toInt(nullptr,2);
//                model->setData(model->index(index,9),QString::number(int_data));
                if(isValidIndex(index,6)){
                    double ratio = model->data(index,6).toDouble();
                    QString real_data = QString::number(ratio*int_data);
                    model->setData(model->index(index,10),real_data);
                }
                index+=1;
                i-=1;
            }
        }
        else if(func=="03"||func=="04"){
            int i=prefix_length-1+4;
            while(i<prefix_length-1+4+validbyte&&index<model->rowCount()){
                int len_byte = ceil(model->data(index,4).toString().toInt()/8.0);
                qDebug() << "len_byte" << len_byte;

                if(model->data(index,5).toString()=="浮点型"){
                    float result = 0;
                    //    char* char_d = byte.data();
                    char* p = (char*)&result;
                    if(master_endianness^slave_endianness){
                        for(int index = 0; index < len_byte; index++)
                        {
                            *(p + index) = receiveBuf[i+len_byte - 1 - index];
                        }
                    }else{
                        for(int index = 0; index < len_byte; index++)
                        {
                            *(p + index) = receiveBuf[i+index];
                        }
                    }
                    i+=len_byte;
                    model->setData(model->index(index,9),result);
                    if(isValidIndex(index,6)){
                        double ratio = model->data(index,6).toDouble();
                        QString real_data = QString::number(ratio*result);
                        model->setData(model->index(index,10),real_data);
                    }
                }else{
                    int res=0;
                    while(len_byte>0&&i<prefix_length-1+4+validbyte){
                        int curr =(receiveBuf.at(i)&0xff)*pow(32,len_byte-1);
                        qDebug() << "curr:" << curr;
                        res+=curr;
                        i++;
                        len_byte--;
                    }
                    model->setData(model->index(index,9),res);
                    qDebug() << "res" << res;
                    if(isValidIndex(index,6)){
                        double ratio = model->data(index,6).toDouble();
                        QString real_data = QString::number(ratio*res);
                        model->setData(model->index(index,10),real_data);
                    }
                }
                index++;
            }
        }
        else if(func=="05" || func=="06"){
            num=6; // to determine the length to be deleted
            if(isValidIndex(index,6)){
                double ratio = model->data(index,6).toDouble();
                QString real_data;
                if(model->data(index,5).toString()=="浮点型"){
                    double res = model->data(index,9).toString().toInt();
                    real_data = QString::number(ratio*res);
                }else{
                    int res = model->data(index,9).toString().toInt();
                    real_data = QString::number(ratio*res);
                }
                model->setData(model->index(index,10),real_data);
            }
        }else{
            qDebug() << "ERROR CODE";
        }
//        for(int i=prefix_length-1+4;i<prefix_length-1+4+validbyte;i++){
//            int int_byte = receiveBuf.at(i)&0xff;

//            // analyze the int_byte. 1 bit each by default
//            // 可能返回的这组数据中，既有要按1bit解析，也有要按8/16解析的
//            QString str_byte = QString("%1").arg(int_byte,8,2,QLatin1Char('0'));
//            qDebug() << "str_byte" << str_byte;
//            for(int it=7;it>=0;it--){
//                if(str_byte[it]=='1'){
//                    curr_data = "1"; // for 8/16/32 bit, curr_data = setNum(...);
//                }else{
//                    curr_data = "0";
//                }
//                model->setData(model->index(index,9),curr_data);
//                if(isValidIndex(index,6)){
//                    double ratio = model->data(index,6).toDouble();
//                    QString real_data = QString::number(ratio*curr_data.toDouble());
//                    model->setData(model->index(index,10),real_data);
//                }
//                index++;
//            }
//        }
        if(mode==0){receive_index++;} // for serial
        receiveBuf.remove(0,6+num);
        qDebug() << "------------------------------";

//                QString y = x.sliced(0,2).toHex();
//                int z=y.toInt(nullptr,16);
//        if(mode==0){
//        buf = serial->readAll();
//        if (!buf.isEmpty()){
//            QString str = ui->textBrowser_receive->toPlainText().toLatin1();
//            str += " ";
//            str += buf.toHex(' ');
//            // str += tr(buf);
//            ui->textBrowser_receive->clear();
//            ui->textBrowser_receive->append(str);
//        }

//    int x=QString("0b").toInt(nullptr,16); // 16转10进制数字
//    qDebug() << x;
//    qDebug() << QString::number(x); //10进制数字转10进制字符串
    }

}

void MainWindow::setEndianness(bool msg){
    if(msg){slave_endianness=1;}
    else{slave_endianness=0;}
}

void MainWindow::exportData(int msg)
{
    QString name;
    QString postfix;
    QString split = "";
    switch (msg) {
    case 0:
        name = "CSV";
        postfix = "(*.csv)";
        split = ",";
        break;
    case 1:
        name = "EXCEL";
        postfix = "(*.xls)";
        split = "\t";
        break;
    case 2:
        name = "XML";
        postfix = "(*.xml)";
        break;
    default:
        qDebug() << "msg type from exprtData is invalid!";
        return;
    }
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                  QString(), name+" files "+postfix);
    if(filepath==""){QMessageBox::critical(this, "错误","数据导出为"+name+"失败");}
    if(name=="XML"){
        QDomDocument doc;

        QDomProcessingInstruction instruction;
        instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
        doc.appendChild(instruction);

        QDomElement root = doc.createElement("ModbusInfo");
        doc.appendChild(root);
// =============================config==============================
        QDomElement config = doc.createElement("Configuration");
        root.appendChild(config);

        QDomElement mode = doc.createElement("Mode");
        QDomElement config1 = doc.createElement("Config1");
        QDomElement config2 = doc.createElement("Config2");
        QDomElement config3 = doc.createElement("Config3");
        config.appendChild(mode);
        config.appendChild(config1);
        config.appendChild(config2);
        config.appendChild(config3);

        QString str0,str1,str2,str3;
        QDomText text0,text1,text2,text3;
        if(ui->tabWidget->currentIndex()==0){
            str0 = "RTU";
            str1 = ui->comboBox_port->currentText();
            str2 = ui->comboBox_parity->currentText();
            str3 = ui->comboBox_baudRate->currentText();
        }else{
            str0 = "TCP";
            str1 = ui->lineEdit_delay->text();
            str2 = ui->lineEdit_address->text();
            str3 = ui->lineEdit_port->text();
        }
        text0 = doc.createTextNode(str0);
        text1 = doc.createTextNode(str1);
        text2 = doc.createTextNode(str2);
        text3 = doc.createTextNode(str3);
        mode.appendChild(text0);
        config1.appendChild(text1);
        config2.appendChild(text2);
        config3.appendChild(text3);
// =============================table==============================
        QDomElement data = doc.createElement("Data");
        root.appendChild(data);
        QDomText text4,text5,text6,text7;
        for(int i=0; i<model->rowCount();i++){
            QDomElement row = doc.createElement("Row"+QString::number(i));
            QDomElement ele1 = doc.createElement("Slave");
            QDomElement ele2 = doc.createElement("FunctionCode");
            QDomElement ele3 = doc.createElement("Address");
            QDomElement ele4 = doc.createElement("Width");
            QDomElement ele5 = doc.createElement("Type");
            QDomElement ele6 = doc.createElement("Ratio");
            QDomElement ele7 = doc.createElement("Variable");
            QDomElement ele8 = doc.createElement("Unit");
            QDomElement ele9 = doc.createElement("Value");
            QDomElement ele10 = doc.createElement("RealValue");
            text1 = doc.createTextNode(model->data(i,1).toString());
            text2 = doc.createTextNode(model->data(i,2).toString());
            text3 = doc.createTextNode(model->data(i,3).toString());
            text4 = doc.createTextNode(model->data(i,4).toString());
            text5 = doc.createTextNode(model->data(i,5).toString());
            text6 = doc.createTextNode(model->data(i,6).toString());
            text7 = doc.createTextNode(model->data(i,7).toString());
            QDomText text8 = doc.createTextNode(model->data(i,8).toString());
            QDomText text9 = doc.createTextNode(model->data(i,9).toString());
            QDomText text10 = doc.createTextNode(model->data(i,10).toString());

            data.appendChild(row);

            row.appendChild(ele1);
            row.appendChild(ele2);
            row.appendChild(ele3);
            row.appendChild(ele4);
            row.appendChild(ele5);
            row.appendChild(ele6);
            row.appendChild(ele7);
            row.appendChild(ele8);
            row.appendChild(ele9);
            row.appendChild(ele10);

            ele1.appendChild(text1);
            ele2.appendChild(text2);
            ele3.appendChild(text3);
            ele4.appendChild(text4);
            ele5.appendChild(text5);
            ele6.appendChild(text6);
            ele7.appendChild(text7);
            ele8.appendChild(text8);
            ele9.appendChild(text9);
            ele10.appendChild(text10);
        }
// =============================write==============================
        QFile file(filepath);
        if (file.open(QFile::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            doc.save(stream, 4);		// indent = 4
            file.close();
            QMessageBox::information(this, "提示","数据成功导出为"+name);
            qDebug() << "数据导出成功";
        }else{
            QMessageBox::critical(this, "错误","数据导出为"+name+"失败");
            qDebug() << "数据导出失败";
        }
// ================================================================
    }else{
        int row = model->rowCount();
        int col = model->columnCount();
        QList<QString> list;
        //添加列标题
        QString HeaderRow;
        for (int i = 1; i < col; i++)
        {
            HeaderRow.append(model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() + split);
        }
        list.push_back(HeaderRow);
        for (int i = 0; i < row; i++)
        {
            QString rowStr = "";
            for (int j = 1; j < col; j++){
                QModelIndex index = model->index(i, j);
                rowStr += model->data(index).toString() + split;
            }
            list.push_back(rowStr);
        }
        QTextEdit textEdit;
        for (int i = 0; i < list.size(); i++)
        {
            textEdit.append(list.at(i));
        }

        QFile file(filepath);
        if (file.open(QFile::WriteOnly | QIODevice::Text))
        {
            QTextStream ts(&file);
            ts << textEdit.document()->toPlainText();
            file.close();
            QMessageBox::information(this, "提示","数据成功导出为"+name);
            qDebug() << "数据导出成功";
        }else{
            QMessageBox::critical(this, "错误","数据导出为"+name+"失败");
            qDebug() << "数据导出失败";
        }
    }
}

bool MainWindow::isValidIndex(int x, int y){
    return model->index(x,y).isValid() && x<model->rowCount() && y<model->columnCount() && model->data(model->index(x,y)).toString()!="";
}

void append_CRC(QByteArray& frame){
    unsigned char uchCRCHi = 0xFF ; // register for the significant 8 CRC bit
    unsigned char uchCRCLo = 0xFF ; // register for the least 8 CRC bit
    unsigned char uIndex ; // index of CRC look-up table
    for(int pushMsg=0;pushMsg<frame.size();pushMsg++)
    {
        uIndex = uchCRCLo ^ frame[pushMsg];
        ++pushMsg;
        uchCRCLo = uchCRCHi ^ CRC_hi[uIndex] ;
        uchCRCHi = CRC_lo[uIndex] ;
    }
    frame.append(uchCRCLo);
    frame.append(uchCRCHi);
}

void MainWindow::initSetup(){
    // set mode name
    ui->tabWidget->setTabText(0,"RTU");
    ui->tabWidget->setTabText(1,"TCP");

    QString description;
    QString manufacturer;
    QString serialNumber;
    // set size
    ui->comboBox_port->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    ui->comboBox_baudRate->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    ui->comboBox_parity->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    // find available ports
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();
    // uncomment the line below to output the number of available ports
//    qDebug() << "Total numbers of ports: " << serialPortInfos.count();
    // fill comboBox_ports
    qDebug() << "available serial port:";
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos){
        QStringList list;
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        list << serialPortInfo.portName()
             << (!description.isEmpty() ? description : "")
             << (!manufacturer.isEmpty() ? manufacturer : "")
             << (!serialNumber.isEmpty() ? serialNumber : "")
             << serialPortInfo.systemLocation()
             << (serialPortInfo.vendorIdentifier() ? QString::number(serialPortInfo.vendorIdentifier(), 16) : "")
             << (serialPortInfo.productIdentifier() ? QString::number(serialPortInfo.productIdentifier(), 16) : "");
        qDebug() << "[name]-" << serialPortInfo.portName() << "[address]-" << serialPortInfo.systemLocation();
        ui->comboBox_port->addItem(list.first(), list);
    }
    ui->comboBox_port->addItem(tr("custom"));
    // fill comboBox_baudRate
    ui->comboBox_baudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->comboBox_baudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->comboBox_baudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->comboBox_baudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->comboBox_baudRate->addItem(tr("Custom"));
    // fill comboBox_parity
    ui->comboBox_parity->addItem(tr("Even"), 2);
    ui->comboBox_parity->addItem(tr("Odd"), 3);
    ui->comboBox_parity->addItem(tr("None"), 0);



    ui->ModbusButton->setEnabled(false);
//    // fill comboBox_func
//    ui->comboBox_func->addItem(QStringLiteral("01"),1);
//    ui->comboBox_func->addItem(QStringLiteral("02"),2);
//    ui->comboBox_func->addItem(QStringLiteral("03"),3);
//    ui->comboBox_func->addItem(QStringLiteral("04"),4);
//    ui->comboBox_func->addItem(QStringLiteral("05"),5);
//    ui->comboBox_func->addItem(QStringLiteral("06"),6);
//    // set default
//    ui->label_x->setText(QStringLiteral("起始地址："));
//    ui->label_y->setText(QStringLiteral("线圈数量："));
    // set unable

//    ui->lineEdit_address->setEnabled(false);
//    ui->lineEdit_address->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
//    ui->lineEdit_port->setEnabled(false);
//    ui->lineEdit_port->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
//    ui->lineEdit_delay->setEnabled(false);
//    ui->lineEdit_delay->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
//    QString x = "19";
//    int y = x.toInt(nullptr,16);
//    x=x.setNum(y,2);
//    x=QString("%1").arg(x,8,QLatin1Char('0'));
//    qDebug() << x;
//    QByteArray ba = QByteArray::fromHex(x.toLocal8Bit());
//    qDebug() << ba;
//    QByteArray x = QByteArray::fromHex("9911");
//    int y = x.at(0)&0xff; // byte存的是char，会变成负数，这样可以转正
//    QString z = QString("%1").arg(y,8,2,QLatin1Char('0'));
//    qDebug() << x;

//    int x = 0x0011;
//    int y = 16;
//    int z = x+y;
//    qDebug() << z;
//    int x = 11;
//    QString y = QString("%1").arg(x,4,16);
//    qDebug() << y;
//    QString str = "1122";
//    QByteArray frame = QByteArray::fromHex(str.toLatin1());
//    QByteArray f;
//    f.append('\x01');
//    f.append('\x02');
//    f.append(QByteArray::fromHex(str.toLatin1()));

//    float data = 12.1;
//    QByteArray byte;
//    char* data_char = (char*)&data;
//    for(int index = 3; index >= 0; index--)
//    {
//        byte.append(data_char[index]);
//    }

//    float result = 0;
//    int size = byte.size();
////    char* char_d = byte.data();
//    char* p = (char*)&result;
//    for(int index = 0; index < size; index++)
//    {
//        *(p + index) = byte[size - 1 - index];
//    }


    // check and set the endianness of the modbus master
//    int a = 0x1234;
//    char b = *(char *)&a;
#if Q_BYTE_ORDER == LITTLE_ENDIAN
    master_endianness = 0;
#else
    master_endianness = 1;
#endif
//    if( b == 0x12){master_endianness = 1;}
//    else master_endianness = 0;
}

void MainWindow::setModel(TModel &m){
    ui->tableView->setModel(&m);
    model = &m;
    qDebug() << &*model;
    ui->tableView->setColumnWidth(0,22);
    for(int i=1;i<m.columnCount()-2;i++){
        ui->tableView->setColumnWidth(i,60);
    }
    ui->tableView->setColumnWidth(1,80);
    ui->tableView->setColumnWidth(3,80);
    ui->tableView->setColumnWidth(4,80);
    ui->tableView->setColumnWidth(5,70);
    ui->tableView->setColumnWidth(m.columnCount()-2,90);
    ui->tableView->setColumnWidth(m.columnCount()-1,90);
    model->appendRow();
    connect(ui->plusButton,&QPushButton::clicked,model,&TModel::appendRow);
    connect(ui->plusButton,&QPushButton::clicked,ui->tableView,&QTableView::scrollToBottom);
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),model,SLOT(remove_when_click(QModelIndex)));
    //connect(ui->tableView,&QTableView::clicked,this,[&](QModelIndex i){qDebug() << "mouse click:" << i.row() << i.column() << "行数："<<i.row()+1 ;}); // for debug
}

void MainWindow::setDelegate(int index,DataTypeDelegate&x){
    ui->tableView->setItemDelegateForColumn(index,&x);
}
void MainWindow::setDelegate(int index,FunctionCodeDelegate&x){
    ui->tableView->setItemDelegateForColumn(index,&x);
}
void MainWindow::setDelegate(int index,DeleteDelegate&x){
    ui->tableView->setItemDelegateForColumn(index,&x);
}
void MainWindow::setDelegate(int index,LengthDelegate&x){
    ui->tableView->setItemDelegateForColumn(index,&x);
}
void MainWindow::initTabelview(){
    ui->tableView->setSortingEnabled(true);
    // -----------mvc
//    ui->tableView->setModel(model);
//    ui->tableView->setItemDelegateForColumn(0,&x);
//    ui->tableView->setItemDelegateForColumn(2, &fcd);
//    ui->tableView->setItemDelegateForColumn(5,&dtd);

//    ui->tableView->setMinimumWidth(700);
//    for(int i=1;i<model->columnCount();i++){
//        ui->tableView->setColumnWidth(i,70);
//    }
//    ui->tableView->setColumnWidth(0,25);
//    model->appendRow();
//    model->appendRow();
   // --------------

//    model = new QStandardItemModel(2,11);
    //ui->tableView->setFocusPolicy(Qt::NoFocus); //去掉选中单元格时的虚框
//    model->setHorizontalHeaderLabels({"","从站地址(0x)","功能码", "起始地址(0x)", "单位长度","数据类型","变比","数据库变量", "单位","当前值","实际值"});
//    model->setItem(0,1,new QStandardItem("01"));
//    model->setItem(0, 3, new QStandardItem("0000"));
//    model->setItem(0,4,new QStandardItem("1"));
//    model->setItem(0,6,new QStandardItem("1"));
//    model->setRowCount(2);
//    //ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);// 自适应第一列？？
//     //initialize the first comboBox and list_funcCode



//    QComboBox* func = new QComboBox();
//    list_funcCode.append(func);
//    func->addItems({"01", "02","03","04","05","06"});
//    ui->tableView->setIndexWidget(model->index(0, 2), func);

//    //initialize the second comboBox and list_dataType
//    QComboBox* dat = new QComboBox();
//    list_dataType.append(dat);
//    dat->addItems({"整型","小数","布尔型","无符号整数"});
//    ui->tableView->setIndexWidget(model->index(0, 5), dat);

//    // initialize the minus button filler and list_minusButton
//    QPushButton*pub = new QPushButton();
//    list_minusButton.append(pub);
//    ui->tableView->setColumnWidth(0, 20);
//    model->setItem(0,0,new QStandardItem(""));
//    model->item(0,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

//    // initialize the plus button
//    plusButton = new QPushButton();
//    plusButton->setText("+");
//    ui->tableView->setIndexWidget(model->index(1,0),plusButton);
}

void MainWindow::initConnection(){
    //connect(ui->tabWidget,&QTabWidget::currentChanged,this,&MainWindow::_click_PortButton);
    connect(ui->tcp_button,&QPushButton::clicked,this,&MainWindow::openPort);
    connect(ui->rtu_button,&QPushButton::clicked,this,&MainWindow::openPort);
    connect(ui->ModbusButton,&QPushButton::clicked,this,&MainWindow::sendData);
    // change text corresponding to different
    // connect(ui->comboBox_func,SIGNAL(currentTextChanged(QString)),this,SLOT(_changeText()));
//    // change mode

//    connect(ui->comboBox_mode,SIGNAL(currentTextChanged(QString)),this,SLOT(_changeMode(QString)));
//    // PortButton connection
//    connect(ui->PortButton,SIGNAL(clicked()),this,SLOT(_click_PortButton()));
//    ui->ModbusButton->setEnabled(false);
//    // Modbus Test
//    connect(ui->ModbusButton,SIGNAL(clicked()),this,SLOT(_click_ModbusButton_Test()));
//    // ModusButton connection
//    connect(ui->ModbusButton,SIGNAL(clicked()),this,SLOT(_click_ModbusButton()));
//    // PlusButton connection

//    // MinusButton connection
//    connect(this,&MainWindow::click_minus,this,&MainWindow::_click_MinusButton);
//    // DataButton connection
    connect(ui->exportButton,SIGNAL(clicked()),this,SLOT(_click_ExportButton()));
    connect(ui->EndianButton,SIGNAL(clicked()),this,SLOT(_click_EndianButton()));
    connect(ui->logButton,&QPushButton::clicked,this,[&](){ui->textBrowser->clear();});
}

void MainWindow::_changeMode(QString msg){
//    qDebug() << msg << "mode";
//    if(msg=="RTU"){
//        mode = 0;
//        ui->PortButton->setText("打开串口");
//        ui->lineEdit_address->setEnabled(false);
//        ui->lineEdit_address->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
//        ui->lineEdit_port->setEnabled(false);
//        ui->lineEdit_port->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
//        ui->lineEdit_delay->setEnabled(false);
//        ui->lineEdit_delay->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
//        ui->comboBox_port->setEnabled(true);
//        ui->comboBox_baudRate->setEnabled(true);
//        ui->comboBox_parity->setEnabled(true);
//    }else if(msg=="TCP"){
//        mode = 1;
//        ui->PortButton->setText("TCP连接");
//        ui->lineEdit_address->setEnabled(true);
//        ui->lineEdit_address->setStyleSheet("QLineEdit { background: rgb(255, 255, 255)}");
//        ui->lineEdit_port->setEnabled(true);
//        ui->lineEdit_port->setStyleSheet("QLineEdit { background: rgb(255, 255, 255)}");
//        ui->lineEdit_delay->setEnabled(true);
//        ui->lineEdit_delay->setStyleSheet("QLineEdit { background: rgb(255, 255, 255)}");
//        ui->comboBox_port->setEnabled(false);
//        ui->comboBox_baudRate->setEnabled(false);
//        ui->comboBox_parity->setEnabled(false);
//    }else{
//        qDebug() << "CHANGE MODE ERROR";
//    }
}

void MainWindow::_hostError(){
    QMessageBox::critical(this,"TCP连接断开","The host is disconnected. Plz check whether the host is closed.");
    ui->lineEdit_address->setEnabled(true);
    ui->lineEdit_port->setEnabled(true);
    ui->lineEdit_delay->setEnabled(true);
    ui->tabWidget->tabBar()->setTabEnabled(0,true);
    ui->ModbusButton->setEnabled(false);
    ui->tcp_button->setText(tr("TCP连接"));
    disconnect(socket, &QTcpSocket::readyRead, this, &MainWindow::_receiveData);
}

bool MainWindow::constructFrame(int row_index){
    QString str;
    if(ui->tabWidget->currentIndex()==1){
        str.append(QString("%1").arg(transaction,4,16,QLatin1Char('0')));
        // why use a mapping? why use transaction? just use row_index!!
        row_mp[transaction] = row_index;
        transaction+=1;
        str.append("0000"); // label: modbus
        str.append("0006"); // for single read/write
    }
    int res = isValidIndex(row_index,1) && isValidIndex(row_index,3) && isValidIndex(row_index,4);
    if(!res){qDebug() << "Invalid index! Fail to send.";return false;}
    str.append(model->data(model->index(row_index,1)).toString()); // address of slave
    str.append(model->data(model->index(row_index,2)).toString()); // function code
    str.append(model->data(model->index(row_index,3)).toString()); // start address
    str.append("0001"); // one for each query
//    str.append(model->item(index,4)->text()->toInt(16)); // data length
    QByteArray frame = QByteArray::fromHex(str.toLatin1());
    res=0;
    if(ui->tabWidget->currentIndex()==0){
        append_CRC(frame);
        res = serial->write(frame);
        serial->flush();
    }else if(ui->tabWidget->currentIndex()==1){
        res = socket->write(frame);
        socket->flush();
    }
    if(res){
        qDebug() << "Successfully sent.";
        return true;
    }
    else{qDebug() << "Fail to send.";return false;}
}
bool MainWindow::constructFrame(int begin, int count){
    // both transaction and start_address is begin
    QByteArray frame;
    //QString str;
    if(ui->tabWidget->currentIndex()==1){
//        str.append(QString("%1").arg(transaction,4,16,QLatin1Char('0')));
//        // why use a mapping? why use transaction? just use row_index!!
//        row_mp[transaction] = row_index;
//        transaction+=1;
//        str.append(QString("%1").arg(begin,4,16,QLatin1Char('0')));
//        str.append("0000"); // label: modbus
//        str.append("0006"); // for single read/write
        frame.append(QByteArray::fromHex(QString("%1").arg(begin,4,16,QLatin1Char('0')).toLatin1()));
        frame.append('\x00');frame.append('\x00');
        frame.append('\x00');frame.append('\x06');
    }
//    //check input
    bool res = isValidIndex(begin,1) && model->data(begin,1).toString().size()==2 && isValidIndex(begin,3) && model->data(begin,3).toString().size()==4;
    if(!res){qDebug() << "Invalid index! Fail to send.";return false;}
//    str.append(model->data(begin,1).toString()); // address of slave
//    str.append(model->data(begin,2).toString()); // function code
//    str.append(model->data(begin,3).toString()); // start address
    frame.append(QByteArray::fromHex(model->data(begin,1).toString().toLatin1()));
//    if(model->data(begin,5).toString()=="浮点型"){
//        frame.append('\x10');
//    }else{
//        frame.append(QByteArray::fromHex(model->data(begin,2).toString().toLatin1()));
//    }
    frame.append(QByteArray::fromHex(model->data(begin,2).toString().toLatin1()));
    frame.append(QByteArray::fromHex(model->data(begin,3).toString().toLatin1()));
//    if(model->data(begin,5).toString()=="浮点型"){
//        frame.append('\00');
//        frame.append('\02');
//        frame.append('\04');
//    }
    int funcCode = model->data(begin,2).toString().toInt(nullptr,16);
    if(funcCode>0&&funcCode<5){
        frame.append(QByteArray::fromHex(QString("%1").arg(count,4,16,QLatin1Char('0')).toLatin1()));
        //str.append(QString("%1").arg(count,4,16,QLatin1Char('0')));
    } // the number of coils for each query is <count>
    else if(funcCode==5||funcCode==6){
        res = isValidIndex(begin,9);
        if(!res){qDebug() << "Invalid index! Fail to send.";return false;}
        int val = model->data(begin,9).toString().toInt();
        if(funcCode==5){
//            if(val==1){str.append("ff00");}
//            else if(val==0){str.append("0000");}
            if(val==1){frame.append('\xff');frame.append('\x00');}
            else if(val==0){frame.append('\x00');frame.append('\x00');}
            else{qDebug() << "Invalid coil value! please input '1' or '0'.";return false;}
        }else{// funcCode = 6
            if(model->data(begin,5).toString()=="浮点型"){
                float f_val = model->data(begin,9).toString().toFloat(&res);
                if(!res){qDebug() << "Invalid value! please input a float value";return false;}
                char * char_val = (char*) &f_val;
                if(slave_endianness^master_endianness){
                    for(int index = 3; index >= 0; index--)
                    {
                        frame.append(char_val[index]);
                    }
                }else{
                    for(int index = 0; index < 4; index++)
                    {
                        frame.append(char_val[index]);
                    }
                }
            }else{
                frame.append(QByteArray::fromHex(QString("%1").arg(val,4,16,QLatin1Char('0')).toLatin1()));
//            str.append(QString("%1").arg(val,4,16,QLatin1Char('0')));
            }
        }
    }else{
        if(!res){qDebug() << "Invalid function code! Fail to send.";return false;}
    }
//    str.append("0001"); // one for each query
    //    str.append(model->item(index,4)->text()->toInt(16)); // data length
    //QByteArray frame = QByteArray::fromHex(str.toLatin1());
    qDebug() << "发送报文" << frame;
    res=0;
    if(ui->tabWidget->currentIndex()==0){
        append_CRC(frame);
        res = serial->write(frame);
        serial->flush();
    }else if(ui->tabWidget->currentIndex()==1){
        res = socket->write(frame);
        socket->flush();
    }
    if(res){
        qDebug() << "Successfully sent.";
        ui->textBrowser->append("S:"+frame.toHex(' '));
        return true;
    }
    else{
        qDebug() << "Fail to send.";
        ui->textBrowser->append("S: fail to send.");
        return false;
    }
}
