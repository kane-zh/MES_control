#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H
#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtDebug>
#include "QDesktopWidget"
#include <QDir>
#include "httprequest.h"
#include "metadata.h"
namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();
    void showEvent(QShowEvent *);
signals:
    void SendMsgToPluginInterface(RequestMetaData_dialog request);
public slots:
    void receiveMsgFromPluginInterface(ResponseMetaData_dialog response); //从ContainerManage接收消息
private slots:
    void showServer();                 //显示服务器
    void showReport();                //显示汇报源
    void setServer();                  //设置指定的服务器配置
    void setReport();                 //设置指定的汇报源配置
    void clearServer();                //清除指定的服务器配置
    void clearReport();                //清除指定的汇报源配置
    void fillServerForm();             //填充服务器表单
    void fillReportForm();              //填充汇报源表单
    void fillServerBox();              //填充服务器下拉框
    void connectTest();                //连接测试
    void saveConfig();                 //保存配置信息
    void loadConfig();                 //加载配置信息
private:
    SERVERINFOR  serverInfor[MaxServer];
    REPORT     reportInfor[MaxReport];
    QStandardItemModel    *serverModel;
    QStandardItemModel    *reportModel;
    QMap<QString,QString> functionList;
    QString  driveInfor="";
    QString  dateSetInfor="";
    QString  getValueResult="";
    httpRequest *request;
    Ui::ConfigDialog *ui;
};
#endif // CONFIGDIALOG_H
