#pragma once

#include <QString>
#include <QtPlugin>
#include <QObject>
#include <QDir>
#include "pluginMetaData.h"

//定义接口
class AutoControlInterface
{
public:
    virtual ~AutoControlInterface() {}
signals:
    virtual void sendMsgToPluginManager(RequestMetaData request)=0;
public:
    virtual void receiveMsgFromPluginManager(ResponseMetaData response)=0;
    virtual void showForm(QWidget *parent = nullptr)=0;
};


#define InterfacePlugin_iid "Container.Plugin.ContainerInterface"

Q_DECLARE_INTERFACE(AutoControlInterface, InterfacePlugin_iid)

