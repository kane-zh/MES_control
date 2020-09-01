#ifndef METADATA_H
#define METADATA_H
#define  MaxDataSource     10
#define  MaxDataSet        1000
#include <QString>
#include "open62541.h"
/*UA服务器信息*/
struct  UA_INFOR
{
    QString address;   //地址
    QString username;  //用户名
    QString password;  //密码
    UA_Client *client=nullptr; //服务器连接句柄
 };
/*UA节点结构*/
struct  UA_NODEID
{
    QString namespaceIndex;  //命名空间
    QString identifierType;  //变量类型
    QString identifier;      //标识
    QString dataType;        //数据类型
 };
/*数据源信息*/
struct  DATASOURCE
{
    QString    name;         //名称
    bool       enable=true;       //使能
    QString    desc;         //备注
    UA_INFOR   uaInfor;      //OPCUA连接信息
 };
/*数据集信息*/
struct  DATASET
{
    QString name;            //名称
    QString sourceName;      //数据源名称
    QString sourceIndex;     //数据源索引
    bool    writeEnable=false;     //写使能
    bool    enable=true;          //使能
    QString desc;            //备注
    struct UA_NODEID uaNode;   //UA节点标识
 };
#endif // METADATA_H
