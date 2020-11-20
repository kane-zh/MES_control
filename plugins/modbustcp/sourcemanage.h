#pragma once
#include <QModbusDataUnit>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QTime>
#include "../../mainApp/pluginManage/driverinterface.h"
#include "metadata.h"

class SourceManage : public QObject,public DriveInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID InterfacePlugin_iid FILE "programmer.json")
    Q_INTERFACES(DriveInterface)
public:
    explicit SourceManage();
    ~SourceManage();
public:
    virtual QString getDataSetInfor() Q_DECL_OVERRIDE;
    virtual QString getValue(QString id) Q_DECL_OVERRIDE;                //获取数值
    virtual QString setValue(QString id,QString  value) Q_DECL_OVERRIDE; //设置数值
    virtual void showForm(QWidget *parent = nullptr)  Q_DECL_OVERRIDE;
public slots:
    void onReadReady();
private:
    void loadConfig();                       //加载配置信息
private:
    DATASOURCE  dataSourceInfor[MaxDataSource];
    DATASET     dataSetInfor[MaxDataSet];
};

