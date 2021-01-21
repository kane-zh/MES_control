#ifndef METADATA_H
#define METADATA_H
#define  MaxDataBase       10
#define  MaxDataTable      100
#include <QString>
#include <QMutex>
struct RequestMetaData_dialog
{
    QString type;
    QString drive;
    QString id;
    QString value;
};
struct ResponseMetaData_dialog
{
    QString type;
    QString value;
};
/*数据库信息*/
struct  DATABASE
{
    QString    name;         //名称
    bool       enable=true;  //使能
    QString    desc;         //备注
    QString    address;      //地址
    QString    username;     //用户名
    QString    password;     //密码
    QMutex     m_mutex;
 };
/*数据表信息*/
struct  DATATABLE
{
    QString name;       //名称
    QString dataBase;   //数据库索引
    bool    enable=false;     //使能
    QString frequency;  //采集频率
    QString desc;       //备注
    QString rules;      //字段--数据表对应规则(json字符串)
    bool    getValueEnable=true;
    QString getValueResult="";
 };
#endif // METADATA_H
