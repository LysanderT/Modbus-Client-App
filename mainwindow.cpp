#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <iostream>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

// static look-up table (CRC-16)
QVector<unsigned char> CRC_lo = {
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
QVector<unsigned char> CRC_hi = {
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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serial;
    delete model;
    delete plusButton;
}

void MainWindow::_changeText(){
    switch (ui->comboBox_func->currentText().toInt()) {
    case 1:
        ui->label_x->setText(QStringLiteral("起始地址："));
        ui->label_y->setText(QStringLiteral("线圈数量："));
        break;
    case 2:
        ui->label_x->setText(QStringLiteral("起始地址："));
        ui->label_y->setText(QStringLiteral("输入数量："));
        break;
    case 3:
        ui->label_x->setText(QStringLiteral("字节数："));
        ui->label_y->setText(QStringLiteral("寄存器值："));
        break;
    case 4:
        ui->label_x->setText(QStringLiteral("起始地址："));
        ui->label_y->setText(QStringLiteral("输入寄存器数量："));
        break;
    case 5:
        ui->label_x->setText(QStringLiteral("输出地址："));
        ui->label_y->setText(QStringLiteral("输出值："));
        break;
    case 6:
        ui->label_x->setText(QStringLiteral("寄存器地址："));
        ui->label_y->setText(QStringLiteral("寄存器值："));
        break;
    default:
        qDebug() << "error in slot: changeText().";
    }
}

void MainWindow::_click_PortButton(){
    qDebug() << ui->PortButton->text();
    if(mode==0){
        if (ui->PortButton->text() == tr("打开串口")){
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
                ui->comboBox_mode->setEnabled(false);
                ui->ModbusButton->setEnabled(true);
                ui->PortButton->setText(tr("关闭串口"));
                // ***
                connect(serial, &QSerialPort::readyRead, this, &MainWindow::_receiveData);
                qDebug() << "serial handle:" << serial->handle();
            }else{
                qDebug() << "error: fail to open";
            }
        }else{
            //close the serial port
            //serial->clear();
            serial->close();
            //serial->deleteLater();
            ui->comboBox_port->setEnabled(true);

            ui->comboBox_baudRate->setEnabled(true);
            ui->comboBox_parity->setEnabled(true);
            ui->comboBox_mode->setEnabled(true);
            ui->PortButton->setText(tr("打开串口"));
            ui->ModbusButton->setEnabled(false);
            disconnect(serial, &QSerialPort::readyRead, this, &MainWindow::_receiveData);
        }
    }else if(mode==1){
        if(ui->PortButton->text()==tr("TCP连接")){
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

                ui->comboBox_mode->setEnabled(false);
                ui->ModbusButton->setEnabled(true);
                ui->PortButton->setText(tr("断开连接"));
                connect(socket, &QTcpSocket::readyRead, this, &MainWindow::_receiveData);
            }else{
                qDebug() << "fail to connet TCP";
                //socket->disconnectFromHost();
                //QMessageBox::critical(this, "错误","连接失败！请检查ip地址/端口号是否正确。");
            }
        }else{
            // break the TCP connection
            socket->abort();
            if(socket->state()==QTcpSocket::UnconnectedState){
                qDebug() << "TCP connection broken";
                ui->lineEdit_address->setEnabled(true);
                ui->lineEdit_port->setEnabled(true);
                ui->lineEdit_delay->setEnabled(true);

                ui->comboBox_mode->setEnabled(true);
                ui->ModbusButton->setEnabled(false);
                ui->PortButton->setText(tr("TCP连接"));
                disconnect(socket, &QTcpSocket::readyRead, this, &MainWindow::_receiveData);
            }else{
                qDebug() << "fail to break TCP connection";
                QMessageBox::critical(this,"错误","断开TCP连接失败！");
            }
        }
        ui->ModbusButton->setEnabled(true);//！！！！！！！！
    }


    // TCP
    // 单元标识符必须用0xFF
}

void MainWindow::_click_ModbusButton(){}

void MainWindow::_click_ModbusButton_Test(){
    qDebug() << "发送报文";
//    int delay = 3.5*11*1000/baudRate+1;
    QString str;
    if(mode==1){
        str.append(QString("%1").arg(transaction,4,16,QLatin1Char('0')));
        transaction+=1;
        str.append("0000"); // label: modbus
        str.append("0006"); // for single read/write
    }
    str.append(ui->lineEdit_slave->text());
    str.append(ui->comboBox_func->currentText());
    str.append(ui->lineEdit_x->text());
    str.append(ui->lineEdit_y->text());
    QByteArray frame = QByteArray::fromHex(str.toLatin1());
    int res=0;
    if(mode==0){
        append_CRC(frame);
        res = serial->write(frame);
        serial->flush();
    }else if(mode==1){
        res = socket->write(frame);
        socket->flush();
    }
    if(res){
        qDebug() << "successfully sent.";
    }
    else{qDebug() << "fail to send.";}

//--------another implementation--------
//    QByteArray frame;
//    frame.append(ui->lineEdit_slave->text().toLatin1());
//    frame.append(ui->comboBox_func->currentText().toLatin1());
//    frame.append(ui->lineEdit_x->text().toLatin1());
//    frame.append(ui->lineEdit_y->text().toLatin1());
//    frame = QByteArray::fromHex(frame);
//-------------


//    QThread::msleep(delay);
}

void MainWindow::_click_PlusButton(){
    int index = model->rowCount()-1;
    // add row
    model->setRowCount(index+2);
    // add funcCode
    QComboBox* cmb = new QComboBox();
    list_funcCode.append(cmb);
    cmb->addItems({"01", "02","03","04","05","06"});
    cmb->setCurrentIndex(list_funcCode.at(index-1)->currentIndex());
    ui->tableView->setIndexWidget(model->index(index, 2), cmb);
    // add dataType
    QComboBox* dat = new QComboBox();
    list_dataType.append(dat);
    dat->addItems({"整型","小数","布尔型","无符号整数"});
    dat->setCurrentIndex(list_dataType.at(index-1)->currentIndex());
    ui->tableView->setIndexWidget(model->index(index, 5), dat);
    // add minus button
    QPushButton*pub = new QPushButton();
    list_minusButton.append(pub);
    pub->setText("-");
    ui->tableView->setIndexWidget(model->index(index,0),pub);
    ui->tableView->setColumnWidth(0, 20);
    pub->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(pub,&QPushButton::clicked,this,[=](){ // must be '=' instead of '&' !!!
        bool flag = true;
        qDebug() << pub;
        for(int i=0; i<list_minusButton.size(); i++){
            qDebug() << list_minusButton.at(i);
            if(list_minusButton.at(i)==pub){
                emit click_minus(i);
                flag = false;
                break;
            }
        }
        if(flag) qDebug() << "click_minus EMITION ERROR.";
    });
    // add default of 单位长度
    int unit = model->item(index-1,4)->text().toInt();
    QString str = QString::number(unit);
    model->setItem(index, 4, new QStandardItem(str));
    // add default of 起始地址
    str = QString::number(unit,16);
    int x = model->item(index-1,3)->text().toInt(nullptr,16);
    int y = str.toInt(nullptr,16);
    str =  QString("%1").arg(x+y,4,16,QLatin1Char('0'));//value为int型或char型都可
    model->setItem(index,3,new QStandardItem(str));
    // add default of 变比
    QString str2 = QString("%1").arg(model->item(index-1,6)->text().toFloat());
    model->setItem(index, 6, new QStandardItem(str2));
    // reset plusButton
    plusButton = new QPushButton();
    plusButton->setText("+");
    ui->tableView->setIndexWidget(model->index(index+1,0),plusButton);
    connect(plusButton,SIGNAL(clicked()),this,SLOT(_click_PlusButton()));
    // mv the slider of verticalScrollBar to the bottom
    ui->tableView->scrollToBottom();
//    // debug
//    if(model->data(model->index(index-1,6)).isValid()){
//    qDebug() << model->item(index-1,6)->text();
//    }else{
//        qDebug() << "data invalid!";
//    }
    qDebug() << isValidIndex(index-1,3);
    qDebug() << isValidIndex(index-1,2);
    qDebug() << model->item(index-1,3)->text();
}

void MainWindow::_click_MinusButton(int msg){
// flow: clicked() -> lambda func -> emit click_minus(index) -> _click_MinusButton(index)
    if(!model->removeRow(msg)){
        qDebug() << "REMOVEROW ERROR";
    }
    delete *(list_minusButton.begin()+msg);
    list_minusButton.erase(list_minusButton.begin()+msg);
    delete *(list_dataType.begin()+msg);
    list_dataType.erase(list_dataType.begin()+msg);
    delete *(list_funcCode.begin()+msg);
    list_funcCode.erase(list_funcCode.begin()+msg);
}

void MainWindow::_click_DataButton(){
    Dialog* d = new Dialog(this);
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->exec();
}

void MainWindow::_receiveData(){
    QByteArray buf;
    qDebug() << "readData: ";
    if(mode==0){
        buf = serial->readAll();
    }else if(mode==1){
        buf = socket->readAll();
    }
    if (!buf.isEmpty()){
        QString str = ui->textBrowser_receive->toPlainText().toLatin1();
        str += ":";
        str += buf.toHex(':');
        // str += tr(buf);
        ui->textBrowser_receive->clear();
        ui->textBrowser_receive->append(str);
    }
}

void MainWindow::exportData(int msg)
{
    QString name;
    QString postfix;
    switch (msg) {
    case 0:
        name = "CSV";
        postfix = "(*.csv)";
        break;
    case 1:
        name = "EXCEL";
        postfix = "(*.xls)";
        break;
    case 2:
        name = "TXT";
        postfix = "(*.txt)";
        break;
    default:
        qDebug() << "msg type from exprtData is invalid!";
    }
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                  QString(), name+" files "+postfix);
    if (filepath != "")
    {
        int row = ui->tableView->model()->rowCount();
        int col = ui->tableView->model()->columnCount();
        QList<QString> list;
        //添加列标题
        QString HeaderRow;
        for (int i = 1; i < col; i++)
        {
            HeaderRow.append(ui->tableView->model()->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() + "\t");
        }
        list.push_back(HeaderRow);
        for (int i = 0; i < row; i++)
        {
            QString rowStr = "";
            for (int j = 1; j < col; j++){
                QModelIndex index = ui->tableView->model()->index(i, j);
                rowStr += ui->tableView->model()->data(index).toString() + "\t";
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
    return model->index(x,y).isValid() && model->item(x,y)->text()!="";
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
    QString description;
    QString manufacturer;
    QString serialNumber;
    // set size
    ui->comboBox_port->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    ui->comboBox_baudRate->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    ui->comboBox_parity->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    ui->comboBox_mode->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    ui->PortButton->setFixedWidth(100);
    // find available ports
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();
    // uncomment the line below to output the number of available ports
//    qDebug() << "Total numbers of ports: " << serialPortInfos.count();
    // fill comboBox_ports
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
        qDebug() << "端口名：" << serialPortInfo.portName();
                                  qDebug() << "端口位置：" << serialPortInfo.systemLocation();
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
    // fill comboBox_mode
    ui->comboBox_mode->addItem(tr("RTU"),0);
    ui->comboBox_mode->addItem(tr("TCP"),1);
    // fill comboBox_func
    ui->comboBox_func->addItem(QStringLiteral("01"),1);
    ui->comboBox_func->addItem(QStringLiteral("02"),2);
    ui->comboBox_func->addItem(QStringLiteral("03"),3);
    ui->comboBox_func->addItem(QStringLiteral("04"),4);
    ui->comboBox_func->addItem(QStringLiteral("05"),5);
    ui->comboBox_func->addItem(QStringLiteral("06"),6);
    // set default
    ui->label_x->setText(QStringLiteral("起始地址："));
    ui->label_y->setText(QStringLiteral("线圈数量："));
    // set unable
    ui->lineEdit_address->setEnabled(false);
    ui->lineEdit_address->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
    ui->lineEdit_port->setEnabled(false);
    ui->lineEdit_port->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
    ui->lineEdit_delay->setEnabled(false);
    ui->lineEdit_delay->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");

}

void MainWindow::initTabelview(){
    model = new QStandardItemModel();
    ui->tableView->setModel(model);
    ui->tableView->setFocusPolicy(Qt::NoFocus); //去掉选中单元格时的虚框
    model->setHorizontalHeaderLabels({"","从站地址(0x)","功能码", "起始地址(0x)", "单位长度","数据类型","变比","数据库变量", "当前值","实际值"});
    model->setItem(0,1,new QStandardItem("01"));
    model->setItem(0, 3, new QStandardItem("0000"));
    model->setItem(0,4,new QStandardItem("1"));
    model->setItem(0,6,new QStandardItem("1"));
    model->setRowCount(2);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);// 自适应第一列？？
    // initialize the first comboBox and list_funcCode
    QComboBox* func = new QComboBox();
    list_funcCode.append(func);
    func->addItems({"01", "02","03","04","05","06"});
    ui->tableView->setIndexWidget(model->index(0, 2), func);

    // initialize the second comboBox and list_dataType
    QComboBox* dat = new QComboBox();
    list_dataType.append(dat);
    dat->addItems({"整型","小数","布尔型","无符号整数"});
    ui->tableView->setIndexWidget(model->index(0, 5), dat);

    // initialize the minus button filler and list_minusButton
    QPushButton*pub = new QPushButton();
    list_minusButton.append(pub);
    ui->tableView->setColumnWidth(0, 20);
    model->setItem(0,0,new QStandardItem(""));
    model->item(0,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    // initialize the plus button
    plusButton = new QPushButton();
    plusButton->setText("+");
    ui->tableView->setIndexWidget(model->index(1,0),plusButton);
}

void MainWindow::initConnection(){
    // change text corresponding to different function code
    connect(ui->comboBox_func,SIGNAL(currentTextChanged(QString)),this,SLOT(_changeText()));
    // change mode
    connect(ui->comboBox_mode,SIGNAL(currentTextChanged(QString)),this,SLOT(_changeMode(QString)));
    // PortButton connection
    connect(ui->PortButton,SIGNAL(clicked()),this,SLOT(_click_PortButton()));
    ui->ModbusButton->setEnabled(false);
    // ModusButton connection
    connect(ui->ModbusButton,SIGNAL(clicked()),this,SLOT(_click_ModbusButton_Test()));
    // PlusButton connection
    connect(plusButton,SIGNAL(clicked()),this,SLOT(_click_PlusButton()));
    // MinusButton connection
    connect(this,&MainWindow::click_minus,this,&MainWindow::_click_MinusButton);
    // DataButton connection
    connect(ui->DataButton,SIGNAL(clicked()),this,SLOT(_click_DataButton()));
}

void MainWindow::_changeMode(QString msg){
    if(msg=="RTU"){
        mode = 0;
        ui->PortButton->setText("打开串口");
        ui->lineEdit_address->setEnabled(false);
        ui->lineEdit_address->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
        ui->lineEdit_port->setEnabled(false);
        ui->lineEdit_port->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
        ui->lineEdit_delay->setEnabled(false);
        ui->lineEdit_delay->setStyleSheet("QLineEdit { background: rgb(240, 240, 240)}");
        ui->comboBox_port->setEnabled(true);
        ui->comboBox_baudRate->setEnabled(true);
        ui->comboBox_parity->setEnabled(true);
    }else if(msg=="TCP"){
        mode = 1;
        ui->PortButton->setText("TCP连接");
        ui->lineEdit_address->setEnabled(true);
        ui->lineEdit_address->setStyleSheet("QLineEdit { background: rgb(255, 255, 255)}");
        ui->lineEdit_port->setEnabled(true);
        ui->lineEdit_port->setStyleSheet("QLineEdit { background: rgb(255, 255, 255)}");
        ui->lineEdit_delay->setEnabled(true);
        ui->lineEdit_delay->setStyleSheet("QLineEdit { background: rgb(255, 255, 255)}");
        ui->comboBox_port->setEnabled(false);
        ui->comboBox_baudRate->setEnabled(false);
        ui->comboBox_parity->setEnabled(false);
    }else{
        qDebug() << "CHANGE MODE ERROR";
    }
}
