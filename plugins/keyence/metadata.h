#ifndef METADATA_H
#define METADATA_H
#define  MaxDataSource     10
#define  MaxDataSet        1000
#include <QString>
#include <QTcpSocket>
#include <QMutex>
/*数据源信息*/
struct  DATASOURCE
{
    QString name;       //名称
    bool    enable=true;     //使能
    QString desc;       //备注
    QString host;       //主机
    QString port;       //端口号 
 };
/*数据集信息*/
struct  DATASET
{
    QString name;       //名称
    QString sourceName;     //数据源名称
    QString sourceIndex;    //数据源索引
    bool    writeEnable=false;    //写使能
    bool    enable=true;     //使能
    QString desc;       //备注
    QString type;       //类型
    QString number;     //编号
    QString timeout;    //超时时间
    QString value;
    QTcpSocket *socket = nullptr;
    QMutex m_mutex;
 };
#endif // METADATA_H
