#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include "ItemDelegate.h"
#include "metaData.h"
#include "httprequest.h"
#include "QDesktopWidget"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();
    void showEvent(QShowEvent *) ;
signals:
    void SendMsgToPluginInterface(RequestMetaData_dialog request);
public slots:
    void receiveMsgFromPluginInterface(ResponseMetaData_dialog response); //从ContainerManage接收消息
private slots:
    void showServerInfor();                         //显示服务器
    void setServerInfor();                          //设置指定的服务器配置
    void setRecordInfor();                         //设置指定的记录信息配置
    void clearServerInfor();                        //清除指定的服务器配置
    void clearRecordInfor();                       //清除指定的记录信息配置
    void fillServerInforForm();                     //填充服务器表单
    void fillRecordInforForm();                    //填充记录信息表单
    void fillServerInforBox();                      //填充服务器下拉框
    void connectTest();                          //连接测试
    void saveValueTest();                        //保存数值测试
    void saveConfig();                             //保存配置信息
    void loadConfig();                            //加载配置信息
private:
    SERVERINFOR  serverInfor[MaxServer];
    RECORDINFOR  recordInfor[MaxRecord];
    QStandardItemModel    *serverModel;
    QStandardItemModel    *recordModel;
    ItemDelegate *m_delegate;
    QString  driveInfor="";
    QString  dateSetInfor="";
    QString  getValueResult="";
    httpRequest *request;
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
