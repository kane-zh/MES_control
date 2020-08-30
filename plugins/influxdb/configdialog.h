#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QtPlugin>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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
    void fillDataTableBox();                     //填充数据表下拉框
    void connectTest();                          //连接测试
    void saveValueTest();                        //保存数值测试
    void saveDataBase();                         //保存数据库
    void loadDataBase();                         //加载数据库
    void saveDataTable();                        //保存数据表
    void loadDataTable();                        //加载数据表
private:
    DATABASE  dataBaseInfor[MaxDataBase];
    DATATABLE dataTableInfor[MaxDataTable];
    QStandardItemModel    *dataBaseModel;
    QStandardItemModel    *dataTableModel;
    ItemDelegate *m_delegate;
    QString  driveInfor="";
    QString  dateSetInfor="";
    QString  getValueResult="";
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
