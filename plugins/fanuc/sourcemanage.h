#pragma once
#include <QtPlugin>
#include <QDir>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtDebug>
#include "metadata.h"
#include "../../mainApp/pluginManage/driverinterface.h"
#include "configdialog.h"
#include "axisinfor.h"
#include "toollifemanagement.h"
#include "otherinfor.h"
#include "programinfor.h"


class SourceManage:public QObject, public DriveInterface
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
private:
    void  loadConfig();
private:
    DATASOURCE  dataSourceInfor[MaxDataSource];
    DATASET     dataSetInfor[MaxDataSet];
};

