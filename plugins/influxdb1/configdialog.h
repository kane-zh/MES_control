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
#include "httprequest.h"
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
    void showDataBase();                         //显示数据库
    void showRules();                            //显示数据表匹配规则
    void setDataBase();                          //设置指定的数据库配置
    void setDataTable();                         //设置指定的数据表配置
    void clearDataBase();                        //清除指定的数据库配置
    void clearDataTable();                       //清除指定的数据表配置
    void fillDataBaseForm();                     //填充数据库表单
    void fillDataTableForm();                    //填充数据表表单
    void fillDataBaseBox();                      //填充数据库下拉框
    void connectTest();                          //连接测试
    void saveValueTest();                        //保存数值测试
    void saveConfig();                           //保存配置信息
    void loadConfig();                           //加载配置信息
    void createActions();//创建右键菜单
    void RemoveRow();//移除选中行
    void AddRow();   //在选中行下添加行
private:
    DATABASE  dataBaseInfor[MaxDataBase];
    DATATABLE dataTableInfor[MaxDataTable];
    QStandardItemModel    *dataBaseModel;
    QStandardItemModel    *dataTableModel;
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
