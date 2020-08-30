#pragma once

#include <QString>
#include <QtPlugin>
#include <QObject>
#include <QDir>
//定义接口
class DriveInterface
{
public:
    virtual ~DriveInterface() {}
    virtual QString getDataSetInfor()= 0;
    virtual QString getValue(QString id)= 0;                //获取数值
    virtual QString setValue(QString id,QString  value)= 0; //设置数值
    virtual void showForm(QWidget *parent = nullptr)=0;
};


#define InterfacePlugin_iid "Drive.Plugin.DriveInterface"

Q_DECLARE_INTERFACE(DriveInterface, InterfacePlugin_iid)

