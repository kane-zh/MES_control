#pragma once

#include <QObject>
#include <QPluginLoader>
#include <QVariant>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "driverinterface.h"
#include "containerinterface.h"
#include "autocontrolinterface.h"
#include "debugcontrolinterface.h"
#include "pluginMetaData.h"
#include "driveinfor.h"

class  PluginManage : public QObject
{
    Q_OBJECT

public:
    static PluginManage &getInstance()
    {
        static PluginManage m_instance;
        return m_instance;
    }
private slots:
     void detalSignaleOfPlugin(RequestMetaData request); //处理插件发出的信号
private:
    void scan(const QString &filepath);             //扫描JSON文件中的插件元数据
    bool check(const QString &filepath);            //插件依赖检测
    void loadPlugin(const QString &filepath);       //加载其中某个插件
    void unloadPlugin(const QString &filepath);     //卸载某个插件
    QList<QPluginLoader *> allPlugins();            //获取所有插件
    QPluginLoader* getPlugin(const QString &name);  //获取某个插件
public:
    void loadAllPlugins();                          //加载所有插件
    void unloadAllPlugins();                        //卸载所有插件
public:
    QHash<QString, QVariant> m_names; //插件路径--插件名称
    QHash<QString, QVariant> m_versions; //插件路径--插件版本
    QHash<QString, QVariant> m_title; //插件路径--插件标题
    QHash<QString, QVariantList>m_dependencies; //插件路径--插件额外依赖的其他插件
    QHash<QString, QPluginLoader *>m_loaders; //插件路径--QPluginLoader实例
    QHash<QString, QString >m_types; //插件路径--插件类型
};

