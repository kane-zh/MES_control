TEMPLATE        = lib           #表示这个makefile是一个lib的makefile
CONFIG         += plugin        #应用程序是一个插件
QT             += widgets network
#include <QNetworkReply>
#include <QNetworkAccessManager>

QT             += core gui concurrent
TARGET          = mes      #生成插件的名称
win32:CONFIG(release, debug|release): DESTDIR         = ../../mainApp/release/plugins/libs
else:win32:CONFIG(debug, debug|release): DESTDIR         = ../../mainApp/debug/plugins/libs
else:unix: DESTDIR         = ../../mainApp/plugins/libs

FORMS += \
    configdialog.ui

HEADERS += \
    ItemDelegate.h \
    configdialog.h \
    containermanage.h \
    httprequest.h \
    metaData.h

SOURCES += \
    ItemDelegate.cpp \
    configdialog.cpp \
    containermanage.cpp \
    httprequest.cpp

DISTFILES += \
    programmer.json
