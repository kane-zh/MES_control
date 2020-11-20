#ifndef  METADATA_H
#define  METADATA_H
#define  MaxServer         10
#define  MaxRecord         100
#include <QString>
#include <QMutex>
struct RequestMetaData_dialog
{
    QString type;
    QString drive;
    QString index;
    QString value;
};
struct ResponseMetaData_dialog
{
    QString type;
    QString value;
};
/*服务器信息*/
struct  SERVERINFOR
{
    QString    name;         //名称
    bool       enable=true;       //使能
    QString    desc;         //备注
    QString    address;      //地址
    QString    username;     //用户名
    QString    password;     //密码
    QString    token;        //登录秘钥
    QMutex     m_mutex;
 };
/*记录信息*/
struct  RECORDINFOR
{
    QString name;       //名称
    QString server;     //服务器索引
    QString serverid;   //记录在服务器的索引
    bool    enable=false;     //使能
    QString frequency;  //采集频率
    QString desc;       //备注
    QString rules;      //字段--数据表对应规则(json字符串)
    bool    getValueEnable=true;
    QString getValueResult="";

 };
#endif // METADATA_H
