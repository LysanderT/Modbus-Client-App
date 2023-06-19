#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QStandardItemModel>
#include <QVector>
#include <QComboBox>
#include <QPushButton>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMap>

#include "delegate.hpp"
#include "tmodel.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals: void click_minus(int);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setModel(TModel& m);

    void setDelegate(int index,DataTypeDelegate&x);
    void setDelegate(int index,FunctionCodeDelegate&x);
    void setDelegate(int index,DeleteDelegate&x);
    Ui::MainWindow *ui;
public slots:
    void exportData(int);

private slots:
    // open the selected/input port
    void openPort();
    // send modbus frame
    void sendData();
//    void _click_ModbusButton_Test();
    // add new row
    void _click_PlusButton();
    // remove the specified row
    void _click_MinusButton(int);
    // export data from the table
    void _click_DataButton();
    // process the data send back from slave
    void _receiveData();
    // response to the mode changing
    void _changeMode(QString);
    // response to the host error
    void _hostError();

private:
    QSerialPort *serial;
    TModel* model;
    QTcpSocket * socket;
    QByteArray receiveBuf;
    int baudRate;
    int mode=0;
    int tcpCount=0;
    int transaction = 0x0000; // TCP transaction identifier
    int receive_index = 0;
    bool isValidIndex(int x,int y);
    void initSetup();
    void initTabelview();
    void initConnection();
    bool constructFrame(int);
    QMap<int,int> row_mp;

};
#endif // MAINWINDOW_H
