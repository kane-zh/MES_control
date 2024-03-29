TEMPLATE        = lib           #表示这个makefile是一个lib的makefile
CONFIG         += plugin  c++17      #应用程序是一个插件
QT             += widgets
QT             += core gui concurrent network
TARGET          = influxdb      #生成插件的名称
win32:CONFIG(release, debug|release): DESTDIR         = ../../mainApp/release/plugins/libs
else:win32:CONFIG(debug, debug|release): DESTDIR         = ../../mainApp/debug/plugins/libs
else:unix: DESTDIR         = ../../mainApp/plugins/libs
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += INFLUXDB_WITH_BOOST
DEFINES += ONLY_HTTP
DISTFILES += \
    programmer.json

FORMS += \
    configdialog.ui

HEADERS += \
    ItemDelegate.h \
    configdialog.h \
    containermanage.h \
    httprequest.h \
    metaData.h \


SOURCES += \
    ItemDelegate.cpp \
    configdialog.cpp \
    containermanage.cpp \
    httprequest.cpp \

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

RESOURCES += \
    image.qrc
