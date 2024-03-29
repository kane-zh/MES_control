﻿#include "containermanage.h"
#include <QTime>
#include <QThread>
#include <QCoreApplication>
ContainerManage::ContainerManage(QWidget *parent)
{
    loadConfig();        //加载配置信息
    m_time=new QTimer(this);
    connect(m_time,SIGNAL(timeout()),this,SLOT(timeOut()));
    connect(this,SIGNAL(updateToServer(int)),this,SLOT(autoUpdate(int)));
    m_time->start(100);
}


ContainerManage::~ContainerManage()
{
    m_time->stop();
    delete  m_time;
    QJsonObject json;
    QJsonDocument document;
    json.insert("result","err");
    json.insert("value","服务关闭");
    document.setObject(json);
    QString json_str(document.toJson(QJsonDocument::Compact));
    for(int i=0;i<MaxRecord;i++){
       recordInfor[i].getValueResult=json_str;
    }
}
/*从插件管理器接收消息(回调)*/
void ContainerManage::receiveMsgFromPluginManager(ResponseMetaData response)
{
    /*判断消息是否发送给对话框界面的*/
    if(response.target=="dialog"){
      ResponseMetaData_dialog data;
      if(response.type==getDrivesInfor){
         data.type="getDrivesInfor";
      }
      if(response.type==getDataSetInfor){
         data.type="getDataSetInfor";
      }
      if(response.type==setValue){
         data.type="setValue";
      }
      if(response.type==getValue){
         data.type="getValue";
      }
      data.value=response.value;
      if(m_config!=nullptr){
      m_config->receiveMsgFromPluginInterface(data);
      }
      return;
    }
    switch(response.type){
    case getDrivesInfor:
        driveInfor=response.value;
        break;
    case getDataSetInfor:
        dateSetInfor=response.value;
        break;
    case getValue:
        recordInfor[response.target.toInt()].getValueResult=response.value;
        break;
    case setValue:
        recordInfor[response.target.toInt()].getValueResult=response.value;
        break;
    }
}
/*处理对话框发送的信号*/
void ContainerManage::dealSignalOfDialog(RequestMetaData_dialog request)
{
    RequestMetaData data;
    data.from="dialog";
    data.target="pluginManage";
    data.drive=request.drive;
    data.id=request.id;
    data.value=request.value;
    if(request.type=="getDrivesInfor"){
       data.type=getDrivesInfor;
    }
    if(request.type=="getDataSetInfor"){
       data.type=getDataSetInfor;
    }
    if(request.type=="setValue"){
       data.type=setValue;
    }
    if(request.type=="getValue"){
       data.type=getValue;
    }
    emit sendMsgToPluginManager(data); //发送信号到插件管理器
}

void ContainerManage::showForm(QWidget *parent)
{
  m_config=new ConfigDialog(parent);
  connect(m_config,SIGNAL(SendMsgToPluginInterface(RequestMetaData_dialog)),this,SLOT(dealSignalOfDialog(RequestMetaData_dialog)));
  m_config->exec();
  delete m_config;
  m_config=nullptr;
  loadConfig();        //加载配置信息
}
/*时间定时器超时(槽)*/
void ContainerManage::timeOut()
{
  time_count++;
  for(int i=0;i<MaxRecord;i++)
  {
    if(serverInfor[recordInfor[i].server.toInt()].enable==true){
        if(recordInfor[i].enable==true && recordInfor[i].name!=""){
           if(100*time_count%(recordInfor[i].frequency.toLongLong())==0){
               if(recordInfor[i].getValueEnable==true){
                  recordInfor[i].getValueEnable=false;
                QtConcurrent::run(this,&ContainerManage::autoUpdate,i);
               }
           }
        }
    }
  }
}
void ContainerManage::autoUpdate(int id)
{
    int index=id;
    httpRequest  *myrequest=new httpRequest(this);
    int serverIndex=recordInfor[index].server.toInt();
    QString result;
    if(serverInfor[serverIndex].token==""){
        result=myrequest->login(serverInfor[serverIndex].address,serverInfor[serverIndex].username,serverInfor[serverIndex].password);
        if(result=="err"){
            qDebug()<<"连接服务器失败!!!";
            delete  myrequest;
            return;
        }
        else {
          serverInfor[serverIndex].token=result;
        }
    }
    QJsonDocument document = QJsonDocument::fromJson(recordInfor[index].rules.toUtf8());
    QJsonArray array= document.array();
    QJsonObject json;
    for (int i = 0; i < array.count(); i++)
     {
        QJsonObject value = array.at(i).toObject();
        if(value["dataId"].toString()!=""){
            RequestMetaData data;
            data.type=getValue;
            data.from=QString::number(index);
            data.target="manage";
            data.drive=value["drive"].toString();
            data.id=value["dataId"].toString();
            recordInfor[index].getValueResult="";
            emit sendMsgToPluginManager(data);
            while(recordInfor[index].getValueResult==""){
                QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
            }
           QJsonDocument document_result = QJsonDocument::fromJson(recordInfor[index].getValueResult.toUtf8());
           if(document_result.object().value("result").toString()=="err"){
           qDebug()<<"获取数据"+value["field"].toString()+"失败："+document_result.object().value("value").toString();
          }
          else{
              json.insert(value["field"].toString(),document_result.object().value("value").toString());
           }
         }
     }
    if(json.isEmpty()){
     qDebug()<<"未成功获取到有效可写数据!!!";
    }
    else{
     document.setObject(json);
     QByteArray byte_array = document.toJson(QJsonDocument::Compact);
     serverInfor[serverIndex].m_mutex.lock();
     result=myrequest->update(serverInfor[serverIndex].address+"/equipment/equipmentState/"+recordInfor[index].serverid+"/",
                              serverInfor[serverIndex].token,byte_array);
     delete  myrequest;
     serverInfor[serverIndex].m_mutex.unlock();
     if(result=="err"){
       serverInfor[serverIndex].token="";
       qDebug()<<"更新记录失败!!!";
     }
     else {
       qDebug()<<"更新记录成功!!!";
     }
    }
     recordInfor[index].getValueEnable=true;
}

/*加载信息*/
void ContainerManage::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/mes.ini";
    QFile file(fileName);
   if (!file.open(QFile::ReadOnly)) {   //如果文件不存在则新建文件
       file.open( QIODevice::ReadWrite | QIODevice::Text );
      }
   QByteArray data=file.readAll();
   file.close();
   QJsonDocument doc=QJsonDocument::fromJson(data);
   QJsonObject object=doc.object();
   QJsonValue server=object.value("server");
   QJsonArray serverArray=server.toArray();
   for(int index=0;index<MaxServer;index++){
        QJsonObject json=serverArray.at(index).toObject();
        serverInfor[index].name=json.value("name").toString();
        serverInfor[index].enable=json.value("enable").toBool();
        serverInfor[index].desc=json.value("desc").toString();
        serverInfor[index].username=json.value("username").toString();
        serverInfor[index].password=json.value("password").toString();
        serverInfor[index].address=json.value("address").toString();
   }
   QJsonValue record=object.value("record");
   QJsonArray recordArray=record.toArray();
   for(int index=0;index<MaxRecord;index++){
        QJsonObject json=recordArray.at(index).toObject();
        recordInfor[index].name=json.value("name").toString();
        recordInfor[index].server=json.value("server").toString();
        recordInfor[index].serverid=json.value("serverid").toString();
        recordInfor[index].enable=json.value("enable").toBool();
        recordInfor[index].frequency=json.value("frequency").toString();
        recordInfor[index].desc=json.value("desc").toString();
        recordInfor[index].rules=json.value("rules").toString();
   }
}
