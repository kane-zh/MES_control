#pragma once
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QDir>
#include "../../mainApp/pluginManage/autocontrolinterface.h"
#include "metaData.h"
#include "configdialog.h"
namespace Ui {
class ContainerManage;
}

class ContainerManage :public QObject, public AutoControlInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID InterfacePlugin_iid FILE "programmer.json")
    Q_INTERFACES(AutoControlInterface)

public:
    explicit ContainerManage(QWidget *parent = nullptr);
    ~ContainerManage();
signals:
    virtual void sendMsgToManager(RequestMetaData request) Q_DECL_OVERRIDE;
public:
    virtual void receiveMsgFromManager(ResponseMetaData response) Q_DECL_OVERRIDE;
    virtual void showForm(QWidget *parent = nullptr) Q_DECL_OVERRIDE;
public slots:
    void startSerial();
    void loadConfig();                         //加载配置信息
private slots:
    void onReadReady();
    void handleSerialError(QSerialPort::SerialPortError  err);
    QString readFromDrive(QString index);
    QString writeToDrive(QString index, QString value);
private:
    QString   driveInfor="";
    QString   dateSetInfor="";
    QString   getValueResult="";
private:
    SERIALINFOR  serialinfor;
    MESINFOR     mesinfor;
};

