TEMPLATE        = lib           #表示这个makefile是一个lib的makefile
CONFIG         += plugin        #应用程序是一个插件
QT             += widgets
QT             += core gui serialbus  serialport
TARGET          = modbusRTU      #生成插件的名称
win32:CONFIG(release, debug|release): DESTDIR         = ../../mainApp/release/plugins/libs
else:win32:CONFIG(debug, debug|release): DESTDIR         = ../../mainApp/debug/plugins/libs
else:unix: DESTDIR         = ../../mainApp/plugins/libs

DISTFILES += \
    programmer.json

SUBDIRS += \
    modbus.pro

FORMS += \
    configdialog.ui

HEADERS += \
    configdialog.h \
    metadata.h \
    sourcemanage.h

SOURCES += \
    configdialog.cpp \
    sourcemanage.cpp

RESOURCES += \
    image.qrc

