#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCborError>
#include <QDir>
#include "QDesktopWidget"
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
private slots:
    void fillSerialForm();                       //回填串口表单
    void fillMesForm();                          //回填mes表单
    void setSerialConfig();                      //设置串口信息
    void setMesConfig();                         //设置mes信息
    void fillInfo();                             //填充下拉框
    void fillMesInfo();                          //填充下拉框
    void connectTest();                          //连接测试
    void saveConfig();                           //保存配置信息
    void loadConfig();                           //加载配置信息
private:
    SERIALINFOR  serialinfor;
    MESINFOR     mesinfor;
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
