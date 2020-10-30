#ifndef METADATA_H
#define METADATA_H
#define MaxServer   2
#define MaxReport   20
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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
/*汇报信息*/
struct  REPORT
{
    QString name;              //名称
    QString serverName;        //服务器名称
    QString serverIndex;       //服务器索引
    QString stateIndex;       //数据集索引
    QString programIndex;       //数据集索引
    bool    enable=true;       //使能
    QString desc;              //备注
    bool    getValueEnable=true;
    QString getValueResult="";

 };
#endif // METADATA_H
