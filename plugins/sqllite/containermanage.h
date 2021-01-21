#pragma once
#include <QtPlugin>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QtConcurrent>
#include <QCoreApplication>
#include "../../mainApp/pluginManage/containerinterface.h"
#include "metaData.h"
#include "configdialog.h"
namespace Ui {
class ContainerManage;
}

class ContainerManage :public QObject, public ContainerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID InterfacePlugin_iid FILE "programmer.json")
    Q_INTERFACES(ContainerInterface)

public:
    explicit ContainerManage(QWidget *parent = nullptr);
    ~ContainerManage();
signals:
    virtual void sendMsgToPluginManager(RequestMetaData request) Q_DECL_OVERRIDE;
public:
    virtual void receiveMsgFromPluginManager(ResponseMetaData response) Q_DECL_OVERRIDE;
    virtual void showForm(QWidget *parent = nullptr) Q_DECL_OVERRIDE;
public slots:
    void dealSignalOfDialog(RequestMetaData_dialog request) ;
    void loadConfig();                         //加载配置信息
    void autoSave(int id);                      //自动保存
    void timeOut();
private:
    DATABASE  dataBaseInfor[MaxDataBase];
    DATATABLE dataTableInfor[MaxDataTable];
    QString   driveInfor="";
    QString   dateSetInfor="";
    QTimer *m_time;
    qlonglong  time_count=0;
    ConfigDialog *m_config=nullptr;
};

