#include "sourcemanage.h"

SourceManage::SourceManage()
{
   loadConfig();
}

SourceManage::~SourceManage()
{
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
            json.insert("index",index);
            json.insert("name",dataSetInfor[index].name);
            json.insert("desc",dataSetInfor[index].desc);
            if(dataSetInfor[index].enable==false ||
               dataSourceInfor[dataSetInfor[index].sourceIndex.toInt()].enable==false){
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
    int sourceId=dataSetInfor[id.toInt()].sourceIndex.toInt();
    /*判断索引范围*/
    if(id>MaxDataSet){
        json.insert("result","err");
        json.insert("value","数据集索引超出了最大值");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    /*判断数据集配置情况*/
    if(dataSetInfor[id.toInt()].name==""){
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
    if(dataSourceInfor[sourceId].name==""){
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
//   if(dataSourceInfor[sourceId].flibhndl==nullptr){
//       QtConcurrent::run(this,&SourceManage::connectToHost,sourceId);
//       while( dataSourceInfor[sourceId].flibhndl==""){
//            QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
//       }
//       if (dataSourceInfor[sourceId].flibhndl=="err")
//       {
//           dataSourceInfor[sourceId].flibhndl="";
//           json.insert("result","err");
//           json.insert("value","连接服务器失败");
//           document.setObject(json);
//           QString json_str(document.toJson(QJsonDocument::Compact));
//           return json_str;
//       }
//   }
    /*判断数据源连接状态(如果已经连接)*/
   if(dataSourceInfor[sourceId].flibhndl==nullptr){
       ushort Flibhndl = 0;
       short ret= cnc_allclibhndl3(dataSourceInfor[sourceId].host.toUtf8().data(),dataSourceInfor[sourceId].port.toUInt(), dataSourceInfor[sourceId].timeout.toInt(), &Flibhndl);
       if (ret != EW_OK)
       {
           dataSourceInfor[sourceId].errCount++;
           if (dataSourceInfor[sourceId].errCount>=dataSourceInfor[sourceId].repeat.toUInt())
           {
               dataSourceInfor[sourceId].enable=false;
               dataSourceInfor[sourceId].errCount=0;
               json.insert("result","err");
               json.insert("value","连接服务器连续失败"+dataSourceInfor[sourceId].repeat+"次,服务器已被失能");
               document.setObject(json);
               QString json_str(document.toJson(QJsonDocument::Compact));
               return json_str;
           }
           json.insert("result","err");
           json.insert("value","连接服务器失败");
           document.setObject(json);
           QString json_str(document.toJson(QJsonDocument::Compact));
           return json_str;
       }
       else{
           dataSourceInfor[sourceId].errCount=0;
           dataSourceInfor[sourceId].flibhndl=QString::number(Flibhndl);
       }
   }
    dataSetInfor[id.toInt()].flibhndl=dataSourceInfor[sourceId].flibhndl;
    QString result;
    dataSourceInfor[sourceId].m_mutex.lock();
    if(dataSetInfor[id.toInt()].group=="控制轴主轴相关"){
        axisInfor m_axiInfor=axisInfor();
        result=m_axiInfor.getValue(dataSetInfor[id.toInt()]);
        QJsonDocument document1=QJsonDocument::fromJson(result.toUtf8());
         if(document1.object().value("result").toString()=="err"){
         dataSourceInfor[sourceId].flibhndl="";
        }
    }
    if(dataSetInfor[id.toInt()].group=="程序相关"){
        programInfor m_programInfor=programInfor();
        result=m_programInfor.getValue(dataSetInfor[id.toInt()]);
        QJsonDocument document1=QJsonDocument::fromJson(result.toUtf8());
         if(document1.object().value("result").toString()=="err"){
         dataSourceInfor[sourceId].flibhndl="";
        }
    }
    if(dataSetInfor[id.toInt()].group=="NC文件数据相关"){
    }
    if(dataSetInfor[id.toInt()].group=="刀具寿命管理数据相关"){
        toolLifeManagement m_toolLifeManagement=toolLifeManagement();
        result=m_toolLifeManagement.getValue(dataSetInfor[id.toInt()]);
        QJsonDocument document1=QJsonDocument::fromJson(result.toUtf8());
         if(document1.object().value("result").toString()=="err"){
         dataSourceInfor[sourceId].flibhndl="";
        }
    }
    if(dataSetInfor[id.toInt()].group=="刀具管理数据相关"){
    }
    if(dataSetInfor[id.toInt()].group=="操作历史数据相关"){
    }
    if(dataSetInfor[id.toInt()].group=="3D干涉检查"){
    }
    if(dataSetInfor[id.toInt()].group=="故障诊断"){
    }
    if(dataSetInfor[id.toInt()].group=="其他"){
        otherInfor m_otherInfor=otherInfor();
        result=m_otherInfor.getValue(dataSetInfor[id.toInt()]);
        QJsonDocument document1=QJsonDocument::fromJson(result.toUtf8());
         if(document1.object().value("result").toString()=="err"){
         dataSourceInfor[sourceId].flibhndl="";
        }
    }
    if(dataSetInfor[id.toInt()].group=="图形指令数据"){
    }
    if(dataSetInfor[id.toInt()].group=="伺服学习数据"){
    }
    if(dataSetInfor[id.toInt()].group=="NC显示功能"){
    }
    if(dataSetInfor[id.toInt()].group=="远程诊断功能"){
    }
    if(dataSetInfor[id.toInt()].group=="双重检查安全功能"){
    }
    if(dataSetInfor[id.toInt()].group=="连续位置数据输出功能"){
    }
    if(dataSetInfor[id.toInt()].group=="程序重新启动"){
    }
    if(dataSetInfor[id.toInt()].group=="辅助状态"){
    }
    if(dataSetInfor[id.toInt()].group=="安全IO信号历史"){
    }
    if(dataSetInfor[id.toInt()].group=="机器状态监控器_记录器"){
    }
    if(dataSetInfor[id.toInt()].group=="环保模式"){
    }
    dataSourceInfor[sourceId].m_mutex.unlock();
    return result;
}

QString SourceManage::setValue(QString id, QString value)
{
    return "设置数据ok";
}



/*加载信息*/
void SourceManage::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/fanuc.ini";
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
        dataSourceInfor[index].host=json.value("host").toString();
        dataSourceInfor[index].port=json.value("port").toString();
        dataSourceInfor[index].timeout=json.value("timeout").toString();
        dataSourceInfor[index].repeat=json.value("repeat").toString();
   }
   QJsonValue dataSet=object.value("dataSet");
   QJsonArray dataSetArray=dataSet.toArray();
   for(int index=0;index<MaxDataSet;index++){
        QJsonObject json=dataSetArray.at(index).toObject();
        dataSetInfor[index].name=json.value("name").toString();
        dataSetInfor[index].sourceName=json.value("sourceName").toString();
        dataSetInfor[index].sourceIndex=json.value("sourceIndex").toString();
        dataSetInfor[index].enable=json.value("enable").toBool();
        dataSetInfor[index].writeEnable=json.value("writeEnable").toBool();
        dataSetInfor[index].desc=json.value("desc").toString();
        dataSetInfor[index].group=json.value("group").toString();
        dataSetInfor[index].function=json.value("function").toString();
        dataSetInfor[index].parameter1=json.value("parameter1").toString();
        dataSetInfor[index].parameter2=json.value("parameter2").toString();
        dataSetInfor[index].parameter3=json.value("parameter3").toString();
        dataSetInfor[index].parameter4=json.value("parameter4").toString();
   }
}

void SourceManage::connectToHost(int id)
{
    ushort Flibhndl = 0;
    short ret= cnc_allclibhndl3(dataSourceInfor[id].host.toUtf8().data(),dataSourceInfor[id].port.toUInt(), dataSourceInfor[id].timeout.toInt(), &Flibhndl);
    if (ret != EW_OK)
    {
        dataSourceInfor[id].flibhndl="err";
    }
    else{
        dataSourceInfor[id].flibhndl=QString::number(Flibhndl);
    }
}
