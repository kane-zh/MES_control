TEMPLATE        = lib           #表示这个makefile是一个lib的makefile
CONFIG         += plugin        #应用程序是一个插件
QT             += widgets  concurrent network
QT             += core gui
TARGET          = control4      #丰光加工记录插件
win32:CONFIG(release, debug|release): DESTDIR         = ../../mainApp/release/plugins/libs
else:win32:CONFIG(debug, debug|release): DESTDIR         = ../../mainApp/debug/plugins/libs
else:unix: DESTDIR         = ../../mainApp/plugins/libs

FORMS += \
    configdialog.ui

HEADERS += \
    configdialog.h \
    containermanage.h \
    httprequest.h \
    metadata.h

SOURCES += \
    configdialog.cpp \
    containermanage.cpp \
    httprequest.cpp


DISTFILES += \
    programmer.json


