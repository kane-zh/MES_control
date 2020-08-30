#include "containermanage.h"
#include <QTime>
#include <QThread>
#include <QCoreApplication>
ContainerManage::ContainerManage(QWidget *parent)
{
    loadConfig();        //加载配置信息
    m_time=new QTimer();
    connect(m_time,SIGNAL(timeout()),this,SLOT(timeOut()));
    connect(this,SIGNAL(updateToServer(int)),this,SLOT(autoUpdate(int)));
    m_time->start(100);
}


ContainerManage::~ContainerManage()
{
    m_time->stop();
}
/*从主程序框架接收消息*/
void ContainerManage::receiveMsgFromManager(ResponseMetaData response)
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
      emit sendMsgToDialog(data);
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
/*处理从对话框接收的信号*/
void ContainerManage::receiveMsgFromDialog(RequestMetaData_dialog request)
{
        RequestMetaData data;
        data.from="dialog";
        data.target="pluginManage";
        data.drive=request.drive;
        data.index=request.index;
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
        emit sendMsgToManager(data); //转发信息到插件管理器
}

void ContainerManage::showForm(QWidget *parent)
{
  ConfigDialog *m_config=new ConfigDialog(parent);
  connect(m_config,SIGNAL(SendMsgToContainerManage(RequestMetaData_dialog)),this,SLOT(receiveMsgFromDialog(RequestMetaData_dialog)));
  connect(this,SIGNAL(sendMsgToDialog(ResponseMetaData_dialog)),m_config,SLOT(receiveMsgFromContainerManage(ResponseMetaData_dialog)));
  m_config->exec();
  loadConfig();        //加载配置信息
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
        if(value["dataIndex"].toString()!=""){
            RequestMetaData data;
            data.type=getValue;
            data.from=QString::number(index);
            data.target="manage";
            data.drive=value["drive"].toString();
            data.index=value["dataIndex"].toString();
            emit sendMsgToManager(data);
            while(recordInfor[index].getValueResult==""){
                QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
            }
           QJsonDocument document_result = QJsonDocument::fromJson(recordInfor[index].getValueResult.toUtf8());
           QJsonObject value_result = document_result.object();
           json.insert(value["field"].toString(),value_result["value"]);
           recordInfor[index].getValueResult="";
         }
     }
   recordInfor[index].getValueEnable=true;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    result=myrequest->update(serverInfor[serverIndex].address+"/equipment/equipmentState/"+recordInfor[index].serverid+"/",
                             serverInfor[serverIndex].token,byte_array);
    if(result=="err"){
      serverInfor[serverIndex].token="";
      qDebug()<<"更新记录失败!!!";
    }
    else {
      qDebug()<<"更新记录成功!!!"<<QTime::currentTime();

    }
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
                  recordInfor[i].getValueEnable==false;
                  emit updateToServer(i);
               }

           }
        }
    }
  }
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
