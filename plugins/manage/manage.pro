TEMPLATE        = lib           #表示这个makefile是一个lib的makefile
CONFIG         += plugin        #应用程序是一个插件
QT             += widgets serialport
QT             += core gui sql
TARGET          = manage      #生成插件的名称
win32:CONFIG(release, debug|release): DESTDIR         = ../../mainApp/release/plugins/libs
else:win32:CONFIG(debug, debug|release): DESTDIR         = ../../mainApp/debug/plugins/libs
else:unix: DESTDIR         = ../../mainApp/plugins/libs

FORMS += \
     configdialog.ui


HEADERS += \
    configdialog.h \
    containermanage.h \
    metaData.h


SOURCES += \
    configdialog.cpp \
    containermanage.cpp

DISTFILES += \
    programmer.json

win32:LIBS += -L$$PWD/./ -lBZClass_c

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32: LIBS += -L$$PWD/./ -lCQUpdate_c

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
