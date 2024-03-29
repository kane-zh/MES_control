#pragma once

#include <QString>
#include <QtPlugin>
#include <QObject>
#include <QDir>
#include "pluginMetaData.h"

//定义接口
class DebugControlInterface
{
public:
    virtual ~DebugControlInterface() {}
signals:
    virtual void sendMsgToPluginManager(RequestMetaData request)=0;
public:
    virtual void receiveMsgFromPluginManager(ResponseMetaData response)=0;
};


#define InterfacePlugin_iid "Container.Plugin.ContainerInterface"

Q_DECLARE_INTERFACE(DebugControlInterface, InterfacePlugin_iid)

