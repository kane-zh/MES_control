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
    void SendMsgToContainerManage(RequestMetaData_dialog request);
public slots:
    void receiveMsgFromContainerManage(ResponseMetaData_dialog response); //从ContainerManage接收信号
private slots:
    void setServerInfor();                          //设置指定的服务器配置
    void clearServerInfor();                        //清除指定的服务器配置
    void fillServerInforForm();                     //填充服务器表单
    void connectTest();                          //连接测试
    void saveValueTest();                        //保存数值测试
    void saveConfig();                             //保存配置信息
    void loadConfig();                            //加载配置信息
private:
    SERVERINFOR  serverInfor;
    RECORDINFOR  recordInfor;
    QString  driveInfor="";
    QString  dateSetInfor="";
    QString  getValueResult="";
    httpRequest *request;
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
