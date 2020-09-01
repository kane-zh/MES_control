TEMPLATE        = lib           #表示这个makefile是一个lib的makefile
CONFIG         += plugin        #应用程序是一个插件
QT             += widgets
QT             += core gui
TARGET          = fanuc      #生成插件的名称
win32:CONFIG(release, debug|release): DESTDIR         = ../../mainApp/release/plugins/libs
else:win32:CONFIG(debug, debug|release): DESTDIR         = ../../mainApp/debug/plugins/libs
else:unix: DESTDIR         = ../../mainApp/plugins/libs

FORMS += \
    configdialog.ui

HEADERS += \
    Fwlib32.h \
    axisinfor.h \
    configdialog.h \
    metadata.h \
    sourcemanage.h

SOURCES += \
    axisinfor.cpp \
    configdialog.cpp \
    sourcemanage.cpp

DISTFILES += \
    programmer.json



win32: LIBS += -L$$PWD/./ -lFwlib32

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
