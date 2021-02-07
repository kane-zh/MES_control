#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QModbusDataUnit>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QTime>
#include <QThread>
#include "QDesktopWidget"
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
    void showEvent(QShowEvent *) Q_DECL_OVERRIDE;
private slots:
    void showDataSource();                       //显示数据源
    void showDataSet();                          //显示数据集
    void setDataSource();                        //设置指定的数据源配置
    void setDataSet();                           //设置指定的数据集配置
    void clearDataSource();                      //清除指定的数据源配置
    void clearDataSet();                         //清除指定的数据集配置
    void fillDataSourceForm();                   //填充数据源表单
    void fillDataSetForm();                      //填充数据集表单
    void fillDataSourceBox();                    //填充数据源下拉框
    void connectTest();                          //连接测试
    void getValueTest();                         //读取数值测试
    void setValueTest();                         //设置数值测试
    void saveConfig();                           //保存配置信息
    void loadConfig();                           //加载配置信息
    void fillInfo();                             //填充下拉框
    void onReadReady();
private:
    DATASOURCE  dataSourceInfor[MaxDataSource];
    DATASET     dataSetInfor[MaxDataSet];
    QStandardItemModel    *dataSourceModel;
    QStandardItemModel    *dataSetModel;
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
