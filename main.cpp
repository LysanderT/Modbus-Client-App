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
    DeleteDelegate dd;
    FunctionCodeDelegate fcd;
    LengthDelegate ld;
    DataTypeDelegate dtd;
    TModel model;
    w.setModel(model);
    w.setDelegate(0,dd);
    w.setDelegate(2,fcd);
    w.setDelegate(4,ld);
    w.setDelegate(5,dtd);
    //----

    w.show();
    return a.exec();
}
