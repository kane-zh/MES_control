#pragma once

#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QDir>
#include "metaData.h"
#include "configdialog.h"
#include "../../mainApp/pluginManage/debugcontrolinterface.h"
namespace Ui {
class mainForm;
}

class mainForm : public QWidget,public DebugControlInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID InterfacePlugin_iid FILE "programmer.json")
    Q_INTERFACES(DebugControlInterface)
public:
    explicit mainForm(QWidget *parent = nullptr);
    ~mainForm();
    void showEvent(QShowEvent *) ;
signals:
    virtual void sendMsgToManager(RequestMetaData request) Q_DECL_OVERRIDE;
public:
    virtual void receiveMsgFromManager(ResponseMetaData response) Q_DECL_OVERRIDE; 
private slots:
    void showDebugForm() ;
    void startSerial();
    void loadConfig();                         //加载配置信息
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
private:
    Ui::mainForm *ui;
};

