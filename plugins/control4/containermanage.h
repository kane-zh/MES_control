#pragma once
#include <QtPlugin>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QtConcurrent>
#include "../../mainApp/pluginManage/containerinterface.h"
#include "httprequest.h"
#include "metadata.h"
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
    virtual void sendMsgToManager(RequestMetaData request) Q_DECL_OVERRIDE;
    void sendMsgToDialog(ResponseMetaData_dialog response);
public:
    virtual void receiveMsgFromManager(ResponseMetaData response) Q_DECL_OVERRIDE;
    virtual void showForm(QWidget *parent = nullptr) Q_DECL_OVERRIDE;
public slots:
    void receiveMsgFromDialog(RequestMetaData_dialog request) ;
    void loadConfig();                         //加载配置信息
    void autoSave(int id);                      //自动保存
    void timeOut();
    QString readFromDrive(QString index,QString  reportId);
    QString writeToDrive(QString index,QString reportId, QString value);
private:
    SERVERINFOR  serverInfor[MaxServer];
    REPORT     reportInfor[MaxReport];
    QString   driveInfor="";
    QString   dateSetInfor="";
    QTimer *m_time;
    qlonglong  time_count=0;
};
