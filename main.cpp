#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTranslator translator;
//    const QStringList uiLanguages = QLocale::system().uiLanguages();
//    for (const QString &locale : uiLanguages) {
//        const QString baseName = "ModbusQT_" + QLocale(locale).name();
//        if (translator.load(":/i18n/" + baseName)) {
//            a.installTranslator(&translator);
//            break;
//        }
//    }
    MainWindow w;

    //----
    DeleteDelegate x;
    FunctionCodeDelegate fcd;
    DataTypeDelegate dtd;
    TModel model;
    qDebug() << &model;
    w.setModel(model);
    w.setDelegate(0,x);
    w.setDelegate(2,fcd);
    w.setDelegate(5,dtd);
    model.appendRow();
    //----

    w.show();
    return a.exec();
}
