QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += serialport
QT += network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    export_dialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    delegate.hpp \
    export_dialog.h \
    mainwindow.h \
    tmodel.hpp

FORMS += \
    export_dialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    ModbusQT_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations
CONFIG += sdk_no_version_check

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
