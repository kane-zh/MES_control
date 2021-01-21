#include "sourcemanage.h"
#include <QTime>
SourceManage::SourceManage()
{
   loadConfig();
}

SourceManage::~SourceManage()
{
    for(int index=0;index<MaxDataSource;index++  ){
       if(dataSourceInfor[index].uaInfor.client!=nullptr){
         UA_Client_delete(dataSourceInfor[index].uaInfor.client);
         }
    }
}
void SourceManage::showForm(QWidget *parent)
{
  ConfigDialog *m_config=new ConfigDialog(parent);
  m_config->exec();
  loadConfig();            //加载配置信息
}
/*获取数据集信息*/
QString SourceManage::getDataSetInfor()
{
    QJsonArray array;
    for(int index=0;index<MaxDataSet;index++){
        if(dataSetInfor[index].name!=""){
            QJsonObject json;
            json.insert("id",index);
            json.insert("name",dataSetInfor[index].name);
            json.insert("desc",dataSetInfor[index].desc);
            if(dataSetInfor[index].enable==false ||
               dataSourceInfor[dataSetInfor[index].sourceId.toInt()].enable==false){
               json.insert("enable","false");
            }
            else{
              json.insert("enable","true");
            }
            if(dataSetInfor[index].writeEnable==false){
               json.insert("writeEnable","false");
            }
            else{
              json.insert("writeEnable","true");
            }
            array.push_back(json);
        }
    }
    QJsonDocument document;
    document.setArray(array);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    return json_str;
}
/*获取指定数据集数据*/
QString SourceManage::getValue(QString id)
{
    QJsonDocument document;
    QJsonObject json;
    UA_StatusCode retval;
    int sourceId=dataSetInfor[id.toInt()].sourceId.toInt();
    /*判断索引范围*/
    if(id>MaxDataSet){
        json.insert("result","err");
        json.insert("value","数据集索引超出了最大值");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    /*判断数据集配置情况*/
    if(dataSetInfor[id.toInt()].uaNode.identifier==""){
        json.insert("result","err");
        json.insert("value","指定数据集信息未被配置");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    /*判断数据集使能情况*/
    if(dataSetInfor[id.toInt()].enable!=true){
        json.insert("result","err");
        json.insert("value","指定数据集未使能");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
     /*判断数据源配置情况*/
    if(dataSourceInfor[sourceId].uaInfor.address==""){
        json.insert("result","err");
        json.insert("value","指定数据集的数据源未配置");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    /*判断数据源使能情况*/
    if(dataSourceInfor[sourceId].enable!=true){
        json.insert("result","err");
        json.insert("value","指定数据集的数据源未使能");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
     /*判断数据源连接状态(如果已经连接)*/
    if(dataSourceInfor[sourceId].uaInfor.client!=nullptr){
        UA_SecureChannelState channelState;
        UA_SessionState sessionState;
        UA_StatusCode connectStatus;
        UA_Client_getState(dataSourceInfor[sourceId].uaInfor.client,&channelState,&sessionState,&connectStatus);
        if((channelState != UA_SECURECHANNELSTATE_OPEN) ||
           (sessionState!=UA_SESSIONSTATE_ACTIVATED)    ||
           (connectStatus!= UA_STATUSCODE_GOOD)){
         UA_Client_delete(dataSourceInfor[sourceId].uaInfor.client);
         dataSourceInfor[sourceId].uaInfor.client=nullptr;
        }
    }
    /*判断数据源是否连接*/
    if(dataSourceInfor[sourceId].uaInfor.client==nullptr){
        /*连接指定的服务器*/
        UA_Client *client =UA_Client_new();
        UA_ClientConfig_setDefault(UA_Client_getConfig(client));
        retval=UA_Client_connect(client,dataSourceInfor[sourceId].uaInfor.address.toLatin1());
        if(retval != UA_STATUSCODE_GOOD) {
         UA_Client_delete(client);
         json.insert("result","err");
         json.insert("value","连接数据集的服务器失败");
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
        }
        else {
         dataSourceInfor[sourceId].uaInfor.client=client;
        }
    }
    dataSourceInfor[sourceId].m_mutex.lock();
    UA_Variant value;
    UA_Variant_init(&value);
    QString namespaceIndex=dataSetInfor[id.toInt()].uaNode.namespaceIndex;
    QString identifier=dataSetInfor[id.toInt()].uaNode.identifier;
     /*根据数据集节点类型，构建不同的节点信息*/
    if(dataSetInfor[id.toInt()].uaNode.identifierType=="String"){
       const UA_NodeId nodeId = UA_NODEID_STRING(namespaceIndex.toInt(),identifier.toLocal8Bit().data());
       retval = UA_Client_readValueAttribute(dataSourceInfor[sourceId].uaInfor.client, nodeId, &value);
    }
    else if(dataSetInfor[id.toInt()].uaNode.identifierType=="Numeric"){
       const UA_NodeId nodeId =UA_NODEID_NUMERIC(namespaceIndex.toInt(),identifier.toLong());
       retval = UA_Client_readValueAttribute(dataSourceInfor[sourceId].uaInfor.client, nodeId, &value);
    }
    else if(dataSetInfor[id.toInt()].uaNode.identifierType=="ByteString"){
       const UA_NodeId nodeId =UA_NODEID_BYTESTRING(namespaceIndex.toInt(),identifier.toLocal8Bit().data());
       retval = UA_Client_readValueAttribute(dataSourceInfor[sourceId].uaInfor.client, nodeId, &value);
    }
    else if(dataSetInfor[id.toInt()].uaNode.identifierType=="String_Alloc"){
       const UA_NodeId nodeId =UA_NODEID_STRING_ALLOC(namespaceIndex.toInt(),identifier.toLocal8Bit().data());
       retval = UA_Client_readValueAttribute(dataSourceInfor[sourceId].uaInfor.client, nodeId, &value);
    }
    else{
        json.insert("result","err");
        json.insert("value","不支持的节点类型");
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        QString json_str(byte_array);
        return json_str;
    }
     dataSourceInfor[sourceId].m_mutex.unlock();
    /*判断读取信息结果*/
    if(retval != UA_STATUSCODE_GOOD) {
        json.insert("result","err");
        json.insert("value","读取节点值失败");
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        QString json_str(byte_array);
        return json_str;
    }
    /*数据解码及返回*/
    QString result = dataDecoding(value);
    UA_Variant_clear(&value);
    json.insert("result","ok");
    json.insert("value",result);
    document.setObject(json);
    QString json_str(document.toJson(QJsonDocument::Compact));
    return json_str;
}

QString SourceManage::setValue(QString id, QString value)
{
    return "设置数据ok";
}

/*数据解码，将OPCUA数据格式转换成字符串*/
QString SourceManage::dataDecoding(UA_Variant value)
{
     QString   result="";
     if(strcmp(value.type->typeName, "Boolean")==0){
         UA_Boolean   raw_date = *(UA_Boolean *) value.data;
         if(raw_date==false){
             result="False";
         }
         else{
             result="True";
         }
     }
     else if(strcmp(value.type->typeName, "Int16")==0){
         UA_Int16  raw_date = *(UA_Int16 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "UInt16")==0){
         UA_UInt16  raw_date = *(UA_UInt16 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "Int32")==0){
         UA_Int32  raw_date = *(UA_Int32 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "UInt32")==0){
         UA_UInt32  raw_date = *(UA_UInt32 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "Int64")==0){
         UA_Int64  raw_date = *(UA_Int64 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "UInt64")==0){
         UA_UInt64  raw_date = *(UA_UInt64 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "Float")==0){
         UA_Float   raw_date = *(UA_Float *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "Double")==0){
         UA_Double   raw_date = *(UA_Double *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "String")==0){
         UA_String   raw_date = *(UA_String *) value.data;
         UA_Byte *p;
         char data[raw_date.length+1];
         p=raw_date.data;
         for(int i=0;i<raw_date.length;i++)
         {
             data[i]=*p;
             p++;
         }
         data[raw_date.length]='\0';
         result=QString::fromLocal8Bit(data);
     }
     else if(strcmp(value.type->typeName, "DateTime")==0){
             UA_DateTime raw_date = *(UA_DateTime *) value.data;
             UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
             result.append(QString::number(dts.year));
             result.append("-");
             result.append(QString::number(dts.month));
             result.append("-");
             result.append(QString::number(dts.day));
             result.append("T");
             result.append(QString::number(dts.hour));
             result.append("-");
             result.append(QString::number(dts.min));
             result.append("-");
             result.append(QString::number(dts.sec));
             result.append("-");
             result.append(QString::number(dts.milliSec));
             result.append("-");
             result.append(QString::number(dts.microSec));
     }
     else{
          result="null";
         }
     return  result;
}
/*数据编码，将字符串转换成OPCUA数据格式*/
QString SourceManage::dataEncoding(QString value, QString DataType, UA_Variant *result)
{
    UA_Variant  newValue;
      UA_Variant_init(&newValue);
      if(strcmp(DataType.toLatin1(), "Boolean")==0){
            UA_Boolean  data;
           if(strcmp(value.toLatin1(), "False")==0){
              data=false;
          }else{
             data=true;
          }
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_BOOLEAN]);
      }
      else  if(strcmp(DataType.toLatin1(), "SByte")==0){
        UA_SByte data=value.toShort();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_SBYTE]);
      }
       else if(strcmp(DataType.toLatin1(), "Int16")==0){
        UA_Int16 data=value.toInt();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_INT16]);
      }
       else if(strcmp(DataType.toLatin1(), "UInt16")==0){
        UA_UInt16 data=value.toUInt();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_UINT16]);
      }
      else if(strcmp(DataType.toLatin1(), "Int32")==0){
        UA_Int32 data=value.toLong();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_INT32]);
      }
     else if(strcmp(DataType.toLatin1(), "UInt32")==0){
        UA_UInt32 data=value.toULong();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_UINT32]);
      }
     else if(strcmp(DataType.toLatin1(), "Int64")==0){
        UA_Int64 data=value.toLongLong();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_INT64]);
      }
     else if(strcmp(DataType.toLatin1(), "UInt64")==0){
        UA_UInt64 data=value.toULongLong();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_UINT64]);
      }
      else if(strcmp(DataType.toLatin1(), "Float")==0){
        UA_Float data=value.toFloat();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_FLOAT]);
      }
      else if(strcmp(DataType.toLatin1(), "Double")==0){
        UA_Double data=value.toDouble();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_DOUBLE]);
      }
      else if(strcmp(DataType.toLatin1(), "String")==0){
//          UA_String  data=value.toStdString();
//          UA_Variant_setScalar(&newValue, &value, &UA_TYPES[UA_TYPES_STRING]);
      }
      else if(strcmp(DataType.toLatin1(), "DateTime")==0){
      }
      else{

      }
       UA_Variant_copy(&newValue,result);
}

/*加载信息*/
void SourceManage::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/opcua.ini";
    QFile file(fileName);
   if (!file.open(QFile::ReadOnly)) {   //如果文件不存在则新建文件
       file.open( QIODevice::ReadWrite | QIODevice::Text );
      }
   QByteArray data=file.readAll();
   file.close();
   QJsonDocument doc=QJsonDocument::fromJson(data);
   QJsonObject object=doc.object();
   QJsonValue dataSource=object.value("dataSource");
   QJsonArray dataSourceArray=dataSource.toArray();
   for(int index=0;index<MaxDataSource;index++){
        QJsonObject json=dataSourceArray.at(index).toObject();
        dataSourceInfor[index].name=json.value("name").toString();
        dataSourceInfor[index].enable=json.value("enable").toBool();
        dataSourceInfor[index].desc=json.value("desc").toString();
        dataSourceInfor[index].uaInfor.username=json.value("uaInfor_username").toString();
        dataSourceInfor[index].uaInfor.password=json.value("uaInfor_password").toString();
        dataSourceInfor[index].uaInfor.address=json.value("uaInfor_address").toString();
   }
   QJsonValue dataSet=object.value("dataSet");
   QJsonArray dataSetArray=dataSet.toArray();
   for(int index=0;index<MaxDataSet;index++){
        QJsonObject json=dataSetArray.at(index).toObject();
        dataSetInfor[index].name=json.value("name").toString();
        dataSetInfor[index].sourceName=json.value("sourceName").toString();
        dataSetInfor[index].sourceId=json.value("sourceId").toString();
        dataSetInfor[index].enable=json.value("enable").toBool();
        dataSetInfor[index].writeEnable=json.value("writeEnable").toBool();
        dataSetInfor[index].desc=json.value("desc").toString();
        dataSetInfor[index].uaNode.namespaceIndex=json.value("uaNode_namespaceIndex").toString();
        dataSetInfor[index].uaNode.identifierType=json.value("uaNode_identifierType").toString();
        dataSetInfor[index].uaNode.identifier=json.value("uaNode_identifier").toString();
        dataSetInfor[index].uaNode.dataType=json.value("uaNode_dataType").toString();
   }
}
