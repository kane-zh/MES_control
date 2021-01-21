#pragma once
#include <QtPlugin>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QDir>
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
    void sendMsgToDialog(ResponseMetaData_dialog response);
public:
    virtual void receiveMsgFromManager(ResponseMetaData response) Q_DECL_OVERRIDE;
    virtual void showForm(QWidget *parent = nullptr) Q_DECL_OVERRIDE;
public slots:
    void receiveMsgFromDialog(RequestMetaData_dialog request) ;
    void loadConfig();                         //加载配置信息
    void autoSave(int id);                      //自动保存
    void timeOut();
private:
    SERVICE  serviceInfor[MaxService];
    TOPIC    topicInfor[MaxTopic];
    QString   driveInfor="";
    QString   dateSetInfor="";
    QTimer *m_time;
    qlonglong  time_count=0;
    ConfigDialog *m_config=nullptr;
};
