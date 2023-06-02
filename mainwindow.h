#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort>
#include <QStandardItemModel>
#include <QVector>
#include <QComboBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals: void click_minus(int);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void exportData(int);

private slots:
    void _changeText();
    // open the selected serial port
    void _click_PortButton();
    // send modbus frame
    void _click_ModbusButton();
    // add new row
    void _click_PlusButton();
    // remove the specified row
    void _click_MinusButton(int);
    // export data from the table
    void _click_DataButton();
    // process the data send back from slave
    void _receiveData();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QStandardItemModel* model;
    QVector<QComboBox*> list_funcCode;
    QVector<QPushButton*> list_minusButton;
    QVector<QComboBox*> list_dataType;
    QPushButton * plusButton;
};
#endif // MAINWINDOW_H
