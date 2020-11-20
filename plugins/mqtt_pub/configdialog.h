#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QtPlugin>
#include <QStandardItemModel>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QMenu>
#include <QtMqtt/qmqttclient.h>
#include "ItemDelegate.h"
#include "metaData.h"


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
    void contextMenuEvent(QContextMenuEvent* e);
signals:
    void SendMsgToContainerManage(RequestMetaData_dialog request);
public slots:
    void receiveMsgFromContainerManage(ResponseMetaData_dialog response); //从ContainerManage接收信号
private slots:
    void showService();                          //显示服务器
    void showRules();                            //显示主题匹配规则
    void setService();                           //设置指定的服务器配置
    void setTopic();                             //设置指定的主题配置
    void clearService();                         //清除指定的服务器配置
    void clearTopic();                           //清除指定的主题配置
    void fillServiceForm();                      //填充服务器表单
    void fillTopicForm();                        //填充主题表单
    void fillServiceBox();                       //填充服务器下拉框
    void connectTest();                          //连接测试
    void publishTest();                          //发布主题测试
    void saveConfig();                           //保存配置信息
    void loadConfig();                           //加载配置信息
    void createActions();//创建右键菜单
    void RemoveRow();//移除选中行
    void AddRow();   //在选中行下添加行
private:
    SERVICE  serviceInfor[MaxService];
    TOPIC    topicInfor[MaxTopic];
    QStandardItemModel    *serviceModel;
    QStandardItemModel    *topicModel;
    ItemDelegate *m_delegate;
    QString  driveInfor="";
    QString  dateSetInfor="";
    QString  getValueResult="";
    QMenu                 *menu;
    QAction               *add_row;
    QAction               *del_row;
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
