#include "containermanage.h"
#include <QTime>
#include <QThread>
ContainerManage::ContainerManage(QWidget *parent)
{
    loadConfig();        //加载配置信息
    m_time=new QTimer();
    connect(m_time,SIGNAL(timeout()),this,SLOT(timeOut()));
    connect(this,SIGNAL(saveDataToDB(int)),this,SLOT(autoSave(int)));
    m_time->start(100);
}


ContainerManage::~ContainerManage()
{
    m_time->stop();
    QSqlDatabase db;
    for(int index=0;index<MaxDataBase;index++  ){
        db=QSqlDatabase::database(dataBaseInfor[index].name);
        if(db.isValid()){
        db.close();
        QSqlDatabase::removeDatabase(dataBaseInfor[index].name);
       }
    }
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

void ContainerManage::autoSave(int id)
{
    int index=id;
    int dataBase=dataTableInfor[index].dataBase.toInt();
    QJsonDocument document = QJsonDocument::fromJson(dataTableInfor[index].rules.toUtf8());
    QJsonArray array= document.array();
    for (int i = 0; i < array.count(); i++)
     {
        qDebug()<<"开始接收";
        QJsonObject value = array.at(i).toObject();
        if(value["dataIndex"].toString()!=""){
            RequestMetaData data;
            data.type=getValue;
            data.from=QString::number(index);
            data.target="manage";
            data.drive=value["drive"].toString();
            data.index=value["dataIndex"].toString();
            emit sendMsgToManager(data);
            while(dataTableInfor[index].getValueResult==""){
                 QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
            }
            qDebug()<<"接收完成";
            std::string value1=dataTableInfor[index].getValueResult.toStdString();
            dataTableInfor[index].getValueResult="";
            int i = 0;
            while( (i = value1.find(' ',i)) != std::string::npos)
               {
                 value1.erase(i,1);
               }
            try {
                QString url="http://"+dataBaseInfor[dataBase].address+":"+dataBaseInfor[dataBase].port+"/?db="+
                        dataBaseInfor[dataBase].name;
               auto influxdb = influxdb::InfluxDBFactory::Get(url.toStdString());
               influxdb->write(influxdb::Point{dataTableInfor[index].name.toStdString()}
               .addTag("index",QString::number(index).toStdString())
               .addField(value["field"].toString().toStdString(),value1));
                qDebug()<<"写入完成";
            }
           catch (...) {
               qDebug()<<"连接服务器失败!!!";
           }
         }
         }
    dataTableInfor[index].getValueEnable=true;
     qDebug()<<"一轮完成";

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
                  dataTableInfor[i].getValueEnable==false;
                  emit saveDataToDB(i);
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
