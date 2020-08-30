#ifndef METADATA_H
#define METADATA_H
#define  MaxDataSource     10
#define  MaxDataSet        1000
#include <QString>
#include <QModbusDataUnit>
#include <QModbusTcpClient>
/*数据源信息*/
struct  DATARTU
{
    QModbusDataUnit::RegisterType code;//功能码
    QString stringCode;    //功能码
    QString serverAddress; //主机地址
    QString startAddress;  //起始地址
    QString length;        //数据长度
    QString value;         //数据
    QModbusReply *reply = nullptr;
 };
/*数据源信息*/
struct  DATASOURCE
{
    QString name;       //名称
    bool    enable;     //使能
    QString desc;       //备注
    QString host;       //主机
    QString port;       //端口号
    QString responseTime = "1000";
    QString numberOfRetries = "3";
    QModbusTcpClient *modbusDevice = nullptr;
 };
/*数据集信息*/
struct  DATASET
{
    QString name;       //名称
    QString sourceName;     //数据源名称
    QString sourceIndex;    //数据源索引
    bool    writeEnable;    //写使能
    bool    enable;     //使能
    QString desc;       //备注
    DATARTU readData;   //读数据
    DATARTU writeData;  //写数据
 };
#endif // METADATA_H
