#ifndef  METADATA_H
#define  METADATA_H
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
struct  SERVERINFOR
{
    QString    name;         //名称
    bool       enable=true;  //使能
    QString    desc;         //备注
    QString    address;      //地址
    QString    username;     //用户名
    QString    password;     //密码
    QString    token="";        //登录秘钥
 };
/*记录信息*/
struct  RECORDINFOR
{
//    QString name;          //名称
//    QString server;        //服务器索引
//    bool    enable=false;     //使能
//    QString desc;       //备注
//    QString task;       //任务信息
//    QString taskItem;   //任务子项
//    QString product;    //产品信息
//    QString batch;      //批次
//    QString station;      //工位
//    QString station_num;  //工位号
//    QString driver;       //监控变量索引
    bool    runFlag=false;
    QString getValueResult="";

 };
#endif // METADATA_H
