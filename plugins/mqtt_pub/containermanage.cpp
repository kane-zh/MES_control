#include "containermanage.h"
#include <QTime>
#include <QThread>
ContainerManage::ContainerManage(QWidget *parent)
{
    loadConfig();        //加载配置信息
    m_time=new QTimer();
    connect(m_time,SIGNAL(timeout()),this,SLOT(timeOut()));
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
        topicInfor[response.target.toInt()].getValueResult=response.value;
        break;
    case setValue:
        topicInfor[response.target.toInt()].getValueResult=response.value;
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
/*时间定时器超时(槽)*/
void ContainerManage::timeOut()
{
  time_count++;
  for(int i=0;i<MaxTopic;i++)
  {
    if(serviceInfor[topicInfor[i].service.toInt()].enable==true){
        if(topicInfor[i].enable==true && topicInfor[i].name!=""){
           if(100*time_count%(topicInfor[i].frequency.toLongLong())==0){
               if(topicInfor[i].getValueEnable==true){
                  topicInfor[i].getValueEnable=false;
                QtConcurrent::run(this,&ContainerManage::autoSave,i);
               }
           }
        }
    }
  }
}
void ContainerManage::autoSave(int id)
{
    int server=topicInfor[id].service.toInt();
    /*判断服务器连接状态(如果已经连接)*/
   if(serviceInfor[server].client!=nullptr){
       if(serviceInfor[server].client->state()!=QMqttClient::Connected){
           serviceInfor[server].client->disconnectFromHost();
           serviceInfor[server].client=nullptr;
       }
   }
   /*判断服务器是否连接*/
   if(serviceInfor[server].client==nullptr){
       QMqttClient *client = new QMqttClient(this);
       client->setHostname(serviceInfor[server].address);
       client->setPort(serviceInfor[server].port.toUInt());
       client->setClientId(QString::number(server));
       client->setUsername(serviceInfor[server].username);
       client->setPassword(serviceInfor[server].password);
       client->connectToHost();
       QTime dieTime = QTime::currentTime().addMSecs(serviceInfor[server].timeOut.toUInt());
       while (QTime::currentTime() < dieTime&&(client->state()!=QMqttClient::Connected)) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
       }
       if(!(client->state()==QMqttClient::Connected)){
            client->disconnectFromHost();
            delete  client;
            qDebug()<<"连接服务器失败";
            topicInfor[id].getValueEnable=true;
            return;
           }
           else{
              serviceInfor[server].client=client;
           }
   }
   QJsonDocument document = QJsonDocument::fromJson(topicInfor[id].rules.toUtf8());
   QJsonArray array= document.array();
   QJsonObject  values;
   for (int i = 0; i < array.count(); i++)
    {
       QJsonObject value = array.at(i).toObject();
       if(value["dataIndex"].toString()!=""){
           RequestMetaData data;
           data.type=getValue;
           data.from=QString::number(id);
           data.target="manage";
           data.drive=value["drive"].toString();
           data.index=value["dataIndex"].toString();
           emit sendMsgToManager(data);
           while(topicInfor[id].getValueResult==""){
                QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
           }
           QJsonDocument document1=QJsonDocument::fromJson(topicInfor[id].getValueResult.toUtf8());
           topicInfor[id].getValueResult="";
            if(document1.object().value("result").toString()=="err"){
            qDebug()<<"获取数据"+value["field"].toString()+"失败："+document1.object().value("value").toString();
           }
           else{
                 if(document1.object().value("value").isObject()){
                     QJsonDocument docment3;
                     docment3.setObject(document1.object().value("value").toObject());
                     QString json_str(docment3.toJson(QJsonDocument::Compact));
                     values.insert(value["field"].toString(),json_str);
                 }
                 else{
                    values.insert(value["field"].toString(),document1.object().value("value").toString());
                 }
            }
        }
    }
    if(values.isEmpty()){
       qDebug()<<"未成功获取到有效可发布数据!!!";
    }
    else{
        QJsonDocument document2;
        document2.setObject(values);
        QByteArray byte_array = document2.toJson(QJsonDocument::Compact);
        QString json_str(byte_array);
        serviceInfor[server].m_mutex.lock();
        if (serviceInfor[server].client->publish(topicInfor[id].topic,json_str.toUtf8()) == -1){
           qDebug()<<"发布主题失败!!!";
        }
        else{
          qDebug()<<"发布主题成功!!!"<<QTime::currentTime();
        }
        serviceInfor[server].m_mutex.unlock();
    }
    topicInfor[id].getValueEnable=true;
}

/*加载信息*/
void ContainerManage::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/mqtt_pub.ini";
    QFile file(fileName);
   if (!file.open(QFile::ReadOnly)) {   //如果文件不存在则新建文件
       file.open( QIODevice::ReadWrite | QIODevice::Text );
      }
   QByteArray data=file.readAll();
   file.close();
   QJsonDocument doc=QJsonDocument::fromJson(data);
   QJsonObject object=doc.object();
   QJsonValue Service=object.value("service");
   QJsonArray ServiceArray=Service.toArray();
   for(int index=0;index<MaxService;index++){
    QJsonObject json=ServiceArray.at(index).toObject();
    serviceInfor[index].name=json.value("name").toString();
    serviceInfor[index].enable=json.value("enable").toBool();
    serviceInfor[index].desc=json.value("desc").toString();
    serviceInfor[index].username=json.value("username").toString();
    serviceInfor[index].password=json.value("password").toString();
    serviceInfor[index].address=json.value("address").toString();
    serviceInfor[index].port=json.value("port").toString();
    serviceInfor[index].timeOut=json.value("timeOut").toString();
   }
   QJsonValue topic=object.value("topic");
   QJsonArray topicArray=topic.toArray();
   for(int index=0;index<MaxTopic;index++){
    QJsonObject json=topicArray.at(index).toObject();
    topicInfor[index].name=json.value("name").toString();
    topicInfor[index].service=json.value("service").toString();
    topicInfor[index].enable=json.value("enable").toBool();
    topicInfor[index].frequency=json.value("frequency").toString();
    topicInfor[index].topic=json.value("topic").toString();
    topicInfor[index].desc=json.value("desc").toString();
    topicInfor[index].rules=json.value("rules").toString();
   }
}
