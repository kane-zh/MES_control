#pragma once

#include <QWidget>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QtConcurrent>
#include "httprequest.h"
#include "configdialog.h"
#include "metaData.h"
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
    void sendMsgToDialog(ResponseMetaData_dialog response);
    void errOccur(QString message);
public:
    virtual void receiveMsgFromManager(ResponseMetaData response) Q_DECL_OVERRIDE;
private slots:
    void receiveMsgFromDialog(RequestMetaData_dialog request) ;
    QString readFromDrive(QString index);
    QString writeToDrive(QString index, QString value);
    void showDebugForm();
    void loadConfig();
    void autoReport();
    void test();
    void showMsg(QString message);
    void fillTaskTypeBox();               //填充任务类型下拉框
    void fillTaskBox();                   //填充任务下拉框
    void showTaskItem();                  //显示订单项
private:
    QString   driveInfor="";
    QString   dateSetInfor="";
    QString   getValueResult="";
    QStandardItemModel    *taskModel;
    SERVERINFOR serverInfor;
    RECORDINFOR recordInfor;
    httpRequest *m_request;
private:
    Ui::mainForm *ui;

};
