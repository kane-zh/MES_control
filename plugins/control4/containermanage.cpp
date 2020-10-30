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
     for(int i=0;i<MaxReport;i++){
        reportInfor[i].getValueResult=json_str;
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
        reportInfor[response.target.toInt()].getValueResult=response.value;
        break;
    case setValue:
        reportInfor[response.target.toInt()].getValueResult=response.value;
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
  delete m_config;
  loadConfig();        //加载配置信息
}
/*时间定时器超时(槽)*/
void ContainerManage::timeOut()
{
  for(int i=0;i<MaxReport;i++)
  {
    if(serverInfor[reportInfor[i].serverIndex.toInt()].enable==true){
        if(reportInfor[i].enable==true && reportInfor[i].name!=""){
           if(reportInfor[i].getValueEnable==true){
              reportInfor[i].getValueEnable=false;
              QtConcurrent::run(this,&ContainerManage::autoSave,i);
           }
        }
    }
  }
}
void ContainerManage::autoSave(int id)
{
    QString result;
    int step=0;
    int errCount=0;
    QString startTime,stopTime,timeLong;
    QTime  time;
    httpRequest *m_request=new httpRequest(this);
    while( reportInfor[id].getValueEnable==false){
        switch (step) {
        case 0: //获取设备状态,当设备状态变为运行中开始计时,并转入下一步
        {
           result=readFromDrive(reportInfor[id].stateIndex,QString::number(id));
           if(result=="err"){  //如果获取失败连续超过3次,报警退出
               errCount++;
               if(errCount>3){
                 qDebug()<<"读取设备状态累计3次失败,请检查原因！！！";
                 reportInfor[id].getValueEnable=true;
                 break;
               }
           }
           else{ //获取数值成功,则判断设备状态
             errCount=0;
             QJsonDocument document_result = QJsonDocument::fromJson(result.toUtf8());
             if(document_result.object().value("alarm")=="0"&&document_result.object().value("run")=="3")
                {
                  startTime=QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz");
                  time.start();
                  qDebug()<<"开始计时";
                  step=1;
                }
           }
          break;
        }
        case 1: //获取设备状态,当设备状态变为暂停中结束计时,并转入下一步
        {
          result=readFromDrive(reportInfor[id].stateIndex,QString::number(id));
           if(result=="err"){  //如果获取失败连续超过3次,报警退出
               errCount++;
               if(errCount>3){
                 qDebug()<<"读取设备状态累计3次失败,请检查原因！！！";
                 reportInfor[id].getValueEnable=true;
                 break;
               }
           }
           else{ //获取数值成功,则判断设备状态
             errCount=0;
             QJsonDocument document_result = QJsonDocument::fromJson(result.toUtf8());
             if(document_result.object().value("alarm")=="0"&&document_result.object().value("run")=="0")
                {
                  stopTime=QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz");
                  timeLong=QString::number(time.elapsed(),10);
                  step=2;
                }
             if(document_result.object().value("alarm")!="0")
                {
                  stopTime=QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz");
                  timeLong=QString::number(time.elapsed(),10);
                  step=10;
                }
           }
          break;
        }
        case 2: //自动报工
        {
            if(serverInfor[id].token==""){
                 result=m_request->login(serverInfor[reportInfor[id].serverIndex.toInt()].address,serverInfor[reportInfor[id].serverIndex.toInt()].username,serverInfor[reportInfor[id].serverIndex.toInt()].password);
                 if(result=="err"){
                    qDebug()<<"连接服务器失败!!!";
                    reportInfor[id].getValueEnable=true;
                    break;
                 }
                 serverInfor[id].token=result;
            }
           result=readFromDrive(reportInfor[id].programIndex,QString::number(id));
            //发送数据
           QJsonObject json;
           json.insert("type","1");
//           json.insert("product_id","");
//           json.insert("task_id","");
//           json.insert("batch","");
           json.insert("dataTime",QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz"));
           json.insert("attribute1",reportInfor[id].name);
           json.insert("attribute2",result);
           json.insert("attribute3",startTime);
           json.insert("attribute4",stopTime);
           json.insert("attribute5",timeLong);
            QJsonDocument document;
            document.setObject(json);
            QByteArray qByteHttpData = document.toJson(QJsonDocument::Compact);
            result=m_request->post(serverInfor[reportInfor[id].serverIndex.toInt()].address+"/production/productData/",serverInfor[id].token,qByteHttpData);
            if(result=="err"){
                qDebug()<<"写入产品生产记录数据失败!!!";
                serverInfor[id].token="";
            }
         reportInfor[id].getValueEnable=true;
         break;
        }
        case 10: //自动报工
        {
            if(serverInfor[id].token==""){
                result=m_request->login(serverInfor[reportInfor[id].serverIndex.toInt()].address,serverInfor[reportInfor[id].serverIndex.toInt()].username,serverInfor[reportInfor[id].serverIndex.toInt()].password);
                 if(result=="err"){
                    qDebug()<<"连接服务器失败!!!";
                    reportInfor[id].getValueEnable=true;
                    break;
                 }
                 serverInfor[id].token=result;
            }
           result=readFromDrive(reportInfor[id].programIndex,QString::number(id));
            //发送数据
           QJsonObject json;
           json.insert("type","1");
//           json.insert("product_id","");
//           json.insert("task_id","");
//           json.insert("batch","");
           json.insert("dataTime",QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz"));
           json.insert("attribute1",reportInfor[id].name);
           json.insert("attribute2",result);
           json.insert("attribute3",startTime);
           json.insert("attribute4",stopTime);
           json.insert("attribute5",timeLong);
           json.insert("attribute6","设备报警");
            QJsonDocument document;
            document.setObject(json);
            QByteArray qByteHttpData = document.toJson(QJsonDocument::Compact);
            result=m_request->post(serverInfor[reportInfor[id].serverIndex.toInt()].address+"/production/productData/",serverInfor[id].token,qByteHttpData);
            if(result=="err"){
                qDebug()<<"写入产品生产记录数据失败!!!";
                serverInfor[id].token="";
            }
         reportInfor[id].getValueEnable=true;
         break;
        }
     }
    if(reportInfor[id].getValueEnable==false){
      QThread::msleep(1000);
    }
    else{
      qDebug()<<"写入产品生产记录数据成功";
      delete  m_request;
    }
  }
}

QString ContainerManage::readFromDrive(QString index,QString  reportId){
    RequestMetaData data;
    data.type=getValue;
    data.from=reportId;
    data.target="manage";
    data.drive="FNAUC";
    data.index=index;
    emit sendMsgToManager(data);
    while(reportInfor[reportId.toInt()].getValueResult==""){
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    QJsonDocument document=QJsonDocument::fromJson(reportInfor[reportId.toInt()].getValueResult.toLocal8Bit().data());
    reportInfor[reportId.toInt()].getValueResult="";
    if(document.object().value("result").toString()!="ok"){
        return "err";
    }
    else{
        if(document.object().value("value").isObject()){
            QJsonDocument docment3;
            docment3.setObject(document.object().value("value").toObject());
            QString json_str(docment3.toJson(QJsonDocument::Compact));
            return json_str;
        }
        else{
            return  document.object().value("result").toString();
        }
    }
}
QString ContainerManage::writeToDrive(QString index,QString  reportId,QString value){
    RequestMetaData data;
    data.type=setValue;
    data.from=reportId;
    data.target="manage";
    data.drive="FANUC";
    data.index=index;
    data.value=value;
    emit sendMsgToManager(data);
    while(reportInfor[reportId.toInt()].getValueResult==""){
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    QJsonDocument document=QJsonDocument::fromJson(reportInfor[reportId.toInt()].getValueResult.toLocal8Bit().data());
    QJsonObject object=document.object();
    reportInfor[reportId.toInt()].getValueResult="";
    if(object.value("result")=="err"){
        return "err";
    }
    return "ok";
}

/*加载信息*/
void ContainerManage::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/control4.ini";
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
   QJsonValue Report=object.value("report");
   QJsonArray ReportArray=Report.toArray();
   for(int index=0;index<MaxReport;index++){
        QJsonObject json=ReportArray.at(index).toObject();
        reportInfor[index].name=json.value("name").toString();
        reportInfor[index].serverName=json.value("serverName").toString();
        reportInfor[index].serverIndex=json.value("serverIndex").toString();
        reportInfor[index].stateIndex=json.value("stateIndex").toString();
        reportInfor[index].programIndex=json.value("programIndex").toString();
        reportInfor[index].enable=json.value("enable").toBool();
        reportInfor[index].desc=json.value("desc").toString();
   }
}
