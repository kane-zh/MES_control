#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QtConcurrent>
#include <QHash>
#include <QWebEngineView>
#include "httprequest.h"
#include "configdialog.h"
#include "metaData.h"
#include "globalvariable.h"
#include "Automatic/runThread01.h"
#include "Automatic/runThread02.h"
#include "Automatic/runThread03.h"
#include "Automatic/runThread04.h"
#include "Automatic/runThread05.h"
#include "Automatic/runThread06.h"
#include "../../mainApp/pluginManage/debugcontrolinterface.h"
#include "Automatic/baseprocess.h"
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
    virtual void sendMsgToPluginManager(RequestMetaData request) Q_DECL_OVERRIDE;
public:
    virtual void receiveMsgFromPluginManager(ResponseMetaData response) Q_DECL_OVERRIDE;
private slots:
    void dealSignalOfDialog(RequestMetaData_dialog request) ;
    void dealSignalOfThread(QString request) ;
    void showDebugForm();
    void loadConfig();
    void timeOut();
    void autoSave();
signals:
    void ShowMessage(QString Text);
private slots:
    void Init();
    void Run();
    void Alarm();
    void Pause();
    void Reset();
    void Estop();
    void Restart();
private:
    void StartAllThread();//启动运行线程
    RunThread01  *pThread01=nullptr;
    RunThread02  *pThread02=nullptr;
    RunThread03  *pThread03=nullptr;
    RunThread04  *pThread04=nullptr;
    RunThread05  *pThread05=nullptr;
    RunThread06  *pThread06=nullptr;
    QString   driveInfor="";
    QString   dateSetInfor="";
    QString   getValueResult="";
    bool   getValueEnable=true;
    ConfigDialog *m_config=nullptr;
    DATAPOINT m_datapoint[200];
    QTimer *m_time;
private:
    Ui::mainForm *ui;
};
