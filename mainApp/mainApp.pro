#-------------------------------------------------
#
# Project created by QtCreator 2020-05-18T15:35:17
#
#-------------------------------------------------

QT       += core gui  xml concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mes_control
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainForm/mainwindow.cpp \
    pluginManage/driveinfor.cpp \
    pluginManage/pluginmanage.cpp

HEADERS += \
FORMS += \
    mainForm/mainwindow.h \
    pluginManage/autocontrolinterface.h \
    pluginManage/debugcontrolinterface.h \
    pluginManage/driveinfor.h \
    pluginManage/driverinterface.h \
    pluginManage/pluginMetaData.h \
    pluginManage/pluginmanage.h \
    pluginManage/containerinterface.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DEFINES += QT_MESSAGELOGCONTEXT
SUBDIRS += \
    mainApp.pro

FORMS += \
    mainForm/mainwindow.ui
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
