TEMPLATE        = lib           #表示这个makefile是一个lib的makefile
CONFIG         += plugin        #应用程序是一个插件
QT             += core gui widgets network concurrent  webenginewidgets
TARGET          = control5     #生成插件的名称
win32:CONFIG(release, debug|release): DESTDIR         = ../../mainApp/release/plugins/libs
else:win32:CONFIG(debug, debug|release): DESTDIR         = ../../mainApp/debug/plugins/libs
else:unix: DESTDIR         = ../../mainApp/plugins/libs


DISTFILES += \
    Automatic/1.py \
    programmer.json

FORMS += \
    configdialog.ui \
    mainform.ui

HEADERS += \
    Automatic/baseprocess.h \
    Automatic/runThread01.h \
    Automatic/runThread02.h \
    Automatic/runThread03.h \
    Automatic/runThread04.h \
    Automatic/runThread05.h \
    ItemDelegate.h \
    configdialog.h \
    globalvariable.h \
    httprequest.h \
    mainform.h \
    metaData.h

SOURCES += \
    Automatic/runThread01.cpp \
    Automatic/runThread02.cpp \
    Automatic/runThread03.cpp \
    Automatic/runThread04.cpp \
    Automatic/runThread05.cpp \
    ItemDelegate.cpp \
    configdialog.cpp \
    globalvariable.cpp \
    httprequest.cpp \
    mainform.cpp



