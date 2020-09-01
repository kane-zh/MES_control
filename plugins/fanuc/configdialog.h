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
#include "metadata.h"
#include "axisinfor.h"
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
private:
    QString GroupType="CNC_NC文件数据相关;CNC控制轴主轴相关;CNC_程序相关;CNC_刀具寿命管理数据相关;CNC_刀具管理数据相关;CNC_与操作历史数据相关;CNC_3D干涉检查;CNC_故障诊断;"
                      "CNC_其他;CNC_图形指令数据;CNC_伺服学习数据;CNC_NC显示功能;CNC_远程诊断功能;双重检查安全功能;连续位置数据输出功能;程序重新启动;辅助状态;安全IO信号历史;"
                      "机器状态监控器_记录器;环保模式";
private slots:
    void showDataSource();             //显示数据源
    void showDataSet();                //显示数据集
    void setDataSource();              //设置指定的数据源配置
    void setDataSet();                 //设置指定的数据集配置
    void clearDataSource();            //清除指定的数据源配置
    void clearDataSet();               //清除指定的数据集配置
    void fillDataSourceForm();         //填充数据源表单
    void fillDataSetForm();            //填充数据集表单
    void fillDataSourceBox();          //填充数据源下拉框
    void connectTest();                //连接测试
    void getValueTest();               //读取数值测试
    void saveConfig();                 //保存配置信息
    void loadConfig();                 //加载配置信息
private:
    DATASOURCE  dataSourceInfor[MaxDataSource];
    DATASET     dataSetInfor[MaxDataSet];
    QStandardItemModel    *dataSourceModel;
    QStandardItemModel    *dataSetModel;
    Ui::ConfigDialog *ui;
};
#endif // CONFIGDIALOG_H
