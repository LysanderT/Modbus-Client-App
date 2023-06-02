QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += serialport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    dialog.h \
    mainwindow.h

FORMS += \
    dialog.ui \
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
