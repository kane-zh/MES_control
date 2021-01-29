#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QtConcurrent>
#include <QHash>
#include "httprequest.h"
#include "configdialog.h"
#include "metaData.h"
#include "globalvariable.h"
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
    virtual void sendMsgToPluginManager(RequestMetaData request) Q_DECL_OVERRIDE;
public:
    virtual void receiveMsgFromPluginManager(ResponseMetaData response) Q_DECL_OVERRIDE;
private slots:
    void dealSignalOfDialog(RequestMetaData_dialog request) ;
    void showDebugForm();
    void loadConfig();
    void timeOut();
    void autoSave();
private:
    QString   driveInfor="";
    QString   dateSetInfor="";
    QString   getValueResult="";
    bool   getValueEnable=true;
    ConfigDialog *m_config=nullptr;
    DATAPOINT m_datapoint[1000];
    QTimer *m_time;
private:
    Ui::mainForm *ui;

};
