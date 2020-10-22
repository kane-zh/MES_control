#ifndef METADATA_H
#define METADATA_H
#define MaxDataSource   10
#define MaxDataSet      1000
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>
#include "Fwlib32.h"
/*数据源信息*/
struct  DATASOURCE
{
    QString    name;          //名称
    bool       enable=true;   //使能
    QString    desc;          //备注
    QString    host;          //主机
    QString    port;          //端口号
    QString    timeout = "";
    QString    flibhndl= "";
    QMutex     m_mutex;
 };

/*数据集信息*/
struct  DATASET
{
    QString name;              //名称
    QString sourceName;        //数据源名称
    QString sourceIndex;       //数据源索引
    bool    writeEnable=false; //写使能
    bool    enable=true;       //使能
    QString desc;              //备注
    QString group;             //功能组
    QString function;          //功能
    QString parameter1;        //参数1
    QString parameter2;        //参数2
    QString parameter3;        //参数3
    QString parameter4;        //参数4
    QString writeData1;        //参数4
    QString writeData2;        //参数4
    QString writeData3;        //参数4
    QString flibhndl= "";

 };
#endif // METADATA_H
