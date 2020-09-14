#ifndef METADATA_H
#define METADATA_H
#define  MaxService       10
#define  MaxTopic     100
#include <QString>
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
struct  SERVICE
{
    QString    name;         //名称
    bool       enable=true;  //使能
    QString    desc;         //备注
    QString    address;      //地址
    QString    port;         //端口号
    QString    username;     //用户名
    QString    password;     //密码

 };
/*主题信息*/
struct  TOPIC
{
    QString name;       //名称
    QString service;    //服务器索引
    bool    enable=false;     //使能
    QString frequency;  //采集频率
    QString desc;       //备注
    QString rules;      //数据源--主题对应规则(json字符串)
    bool    getValueEnable=true;
    QString getValueResult="";
 };
#endif // METADATA_H
