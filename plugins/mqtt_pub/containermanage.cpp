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
        dataTableInfor[response.target.toInt()].getValueResult=response.value;
        break;
    case setValue:
        dataTableInfor[response.target.toInt()].getValueResult=response.value;
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
    if(serviceInfor[dataTableInfor[i].service.toInt()].enable==true){
        if(dataTableInfor[i].enable==true && dataTableInfor[i].name!=""){
           if(100*time_count%(dataTableInfor[i].frequency.toLongLong())==0){
               if(dataTableInfor[i].getValueEnable==true){
                  dataTableInfor[i].getValueEnable==false;
                QtConcurrent::run(this,&ContainerManage::autoSave,i);
               }
           }
        }
    }
  }
}
void ContainerManage::autoSave(int id)
{
    int index=id;
    int service=dataTableInfor[index].service.toInt();
    QJsonDocument document = QJsonDocument::fromJson(dataTableInfor[index].rules.toUtf8());
    QJsonArray array= document.array();
//    try {
//        QString url="http://"+serviceInfor[service].address+":"+serviceInfor[service].port+"/?db="+
//                serviceInfor[service].name;
//         auto influxdb = influxdb::InfluxDBFactory::Get(url.toStdString());
//         for (int i = 0; i < array.count(); i++)
//          {
//             QJsonObject value = array.at(i).toObject();
//             if(value["dataIndex"].toString()!=""){
//                 RequestMetaData data;
//                 data.type=getValue;
//                 data.from=QString::number(index);
//                 data.target="manage";
//                 data.drive=value["drive"].toString();
//                 data.index=value["dataIndex"].toString();
//                 emit sendMsgToManager(data);
//                 while(dataTableInfor[index].getValueResult==""){
//                      QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
//                 }
//                 std::string value1=dataTableInfor[index].getValueResult.toStdString();
//                 dataTableInfor[index].getValueResult="";
//                 int i = 0;
//                 while( (i = value1.find(' ',i)) != std::string::npos)
//                    {
//                      value1.erase(i,1);
//                    }
//                 try {
//                    influxdb->write(influxdb::Point{dataTableInfor[index].name.toStdString()}
//                    .addTag("index",QString::number(index).toStdString())
//                    .addField(value["field"].toString().toStdString(),value1));
//                 }
//                catch (...) {
//                    qDebug()<<"写数据失败!!!";
//                }
//             }
//           }
//       }
//    catch (...) {
//           qDebug()<<"连接服务器失败!!!";
//    }
    dataTableInfor[index].getValueEnable=true;

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
   QJsonValue service=object.value("service");
   QJsonArray serviceArray=service.toArray();
   for(int index=0;index<MaxService;index++){
    QJsonObject json=serviceArray.at(index).toObject();
    serviceInfor[index].name=json.value("name").toString();
    serviceInfor[index].enable=json.value("enable").toBool();
    serviceInfor[index].desc=json.value("desc").toString();
    serviceInfor[index].username=json.value("username").toString();
    serviceInfor[index].password=json.value("password").toString();
    serviceInfor[index].address=json.value("address").toString();
    serviceInfor[index].port=json.value("port").toString();
   }
   QJsonValue topic=object.value("topic");
   QJsonArray topicArray=topic.toArray();
   for(int index=0;index<MaxTopic;index++){
    QJsonObject json=topicArray.at(index).toObject();
    dataTableInfor[index].name=json.value("name").toString();
    dataTableInfor[index].service=json.value("service").toString();
    dataTableInfor[index].enable=json.value("enable").toBool();
    dataTableInfor[index].frequency=json.value("frequency").toString();
    dataTableInfor[index].desc=json.value("desc").toString();
    dataTableInfor[index].rules=json.value("rules").toString();
   }
}
