TEMPLATE        = lib           #表示这个makefile是一个lib的makefile
CONFIG         += plugin        #应用程序是一个插件
QT             += core gui widgets network concurrent
TARGET          = control5     #生成插件的名称
win32:CONFIG(release, debug|release): DESTDIR         = ../../mainApp/release/plugins/libs
else:win32:CONFIG(debug, debug|release): DESTDIR         = ../../mainApp/debug/plugins/libs
else:unix: DESTDIR         = ../../mainApp/plugins/libs


DISTFILES += \
    programmer.json

FORMS += \
    configdialog.ui \
    mainform.ui

HEADERS += \
    ItemDelegate.h \
    configdialog.h \
    globalvariable.h \
    httprequest.h \
    mainform.h \
    metaData.h

SOURCES += \
    ItemDelegate.cpp \
    configdialog.cpp \
    globalvariable.cpp \
    httprequest.cpp \
    mainform.cpp



