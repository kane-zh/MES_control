#include "containermanage.h"
#include <QTime>
#include <QThread>
ContainerManage::ContainerManage(QWidget *parent)
{
    loadConfig();        //加载配置信息
    m_time=new QTimer(this);
    connect(m_time,SIGNAL(timeout()),this,SLOT(timeOut()));
    m_time->start(100);
}


ContainerManage::~ContainerManage()
{
     m_time->stop();
     delete m_time;
     QJsonObject json;
     QJsonDocument document;
     json.insert("result","err");
     json.insert("value","服务关闭");
     document.setObject(json);
     QString json_str(document.toJson(QJsonDocument::Compact));
     for(int i=0;i<MaxDataTable;i++){
        dataTableInfor[i].getValueResult=json_str;
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
        dataTableInfor[response.target.toInt()].getValueResult=response.value;
        break;
    case setValue:
        dataTableInfor[response.target.toInt()].getValueResult=response.value;
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
  for(int i=0;i<MaxDataTable;i++)
  {
    if(dataBaseInfor[dataTableInfor[i].dataBase.toInt()].enable==true){
        if(dataTableInfor[i].enable==true && dataTableInfor[i].name!=""){
           if(100*time_count%(dataTableInfor[i].frequency.toLongLong())==0){
               if(dataTableInfor[i].getValueEnable==true){
                  dataTableInfor[i].getValueEnable=false;
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
    int dataBase=dataTableInfor[index].dataBase.toInt();
    QString  value1=dataTableInfor[index].name+",index="+QString::number(index)+" ";
    QString  value2="";
    QJsonDocument document = QJsonDocument::fromJson(dataTableInfor[index].rules.toUtf8());
    QJsonArray array= document.array();
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
             dataTableInfor[index].getValueResult="";
             emit sendMsgToPluginManager(data);
             while(dataTableInfor[index].getValueResult==""){
                  QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
             }
             QJsonDocument document1=QJsonDocument::fromJson(dataTableInfor[index].getValueResult.toLocal8Bit().data());
              if(document1.object().value("result").toString()=="err"){
              qDebug()<<"获取数据"+value["field"].toString()+"失败："+document1.object().value("value").toString();
             }
             else{
                value2.append(value["field"].toString()+"=");
                value2.append("\"");
                value2.append(document1.object().value("value").toString());
                value2.append("\"");
                value2.append(",");
             }
           }
       }
        if(value2!=""){
             value2.remove(value2.length()-1,2);
             dataBaseInfor[dataBase].m_mutex.lock();
             httpRequest  *myrequest=new httpRequest(this);
             QString result=myrequest->post(dataBaseInfor[dataBase].address,dataBaseInfor[dataBase].port,
                                            dataBaseInfor[dataBase].username,dataBaseInfor[dataBase].password,
                                            dataBaseInfor[dataBase].name,value1+value2);
             delete  myrequest;
             dataBaseInfor[dataBase].m_mutex.unlock();
             if(result=="err"){
               qDebug()<<"写数据失败!!!";
             }
             else{
               qDebug()<<"写数据成功!!!";
             }
        }
        else{
            qDebug()<<"未成功获取到有效可写数据!!!";
        }
    dataTableInfor[index].getValueEnable=true;

}
/*加载信息*/
void ContainerManage::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/influxdb.ini";
    QFile file(fileName);
   if (!file.open(QFile::ReadOnly)) {   //如果文件不存在则新建文件
       file.open( QIODevice::ReadWrite | QIODevice::Text );
      }
   QByteArray data=file.readAll();
   file.close();
   QJsonDocument doc=QJsonDocument::fromJson(data);
   QJsonObject object=doc.object();
   QJsonValue dataBase=object.value("dataBase");
   QJsonArray dataBaseArray=dataBase.toArray();
   for(int index=0;index<MaxDataBase;index++){
    QJsonObject json=dataBaseArray.at(index).toObject();
    dataBaseInfor[index].name=json.value("name").toString();
    dataBaseInfor[index].enable=json.value("enable").toBool();
    dataBaseInfor[index].desc=json.value("desc").toString();
    dataBaseInfor[index].username=json.value("username").toString();
    dataBaseInfor[index].password=json.value("password").toString();
    dataBaseInfor[index].address=json.value("address").toString();
    dataBaseInfor[index].port=json.value("port").toString();
   }
   QJsonValue dataTable=object.value("dataTable");
   QJsonArray dataTableArray=dataTable.toArray();
   for(int index=0;index<MaxDataTable;index++){
    QJsonObject json=dataTableArray.at(index).toObject();
    dataTableInfor[index].name=json.value("name").toString();
    dataTableInfor[index].dataBase=json.value("dataBase").toString();
    dataTableInfor[index].enable=json.value("enable").toBool();
    dataTableInfor[index].frequency=json.value("frequency").toString();
    dataTableInfor[index].desc=json.value("desc").toString();
    dataTableInfor[index].rules=json.value("rules").toString();
   }
}
