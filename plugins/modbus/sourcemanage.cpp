#include "sourcemanage.h"
#include <QTime>
#include "configdialog.h"
SourceManage::SourceManage()
{
    loadConfig();    //加载配置信息
}

SourceManage::~SourceManage()
{

}
void SourceManage::showForm(QWidget *parent)
{
  ConfigDialog *m_dialog=new ConfigDialog(parent);
  m_dialog->exec();
  loadConfig();    //加载配置信息
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
/*获取数据集值*/
QString SourceManage::getValue(QString id)
{
    QJsonDocument document;
    QJsonObject json;
    int sourceId=dataSetInfor[id.toInt()].sourceIndex.toInt();
    dataSetInfor[id.toInt()].readData.value="";
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
    /*判断数据集读信息配置情况*/
    if(dataSetInfor[id.toInt()].readData.stringCode==""){
        json.insert("result","err");
        json.insert("value","指定数据集读信息未被配置");
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
   if(dataSourceInfor[sourceId].modbusDevice!=nullptr){
       if(dataSourceInfor[sourceId].modbusDevice->state()==QModbusDevice::UnconnectedState ||
          dataSourceInfor[sourceId].modbusDevice->state()==QModbusDevice::ClosingState ){
           dataSourceInfor[sourceId].modbusDevice->disconnect();
           dataSourceInfor[sourceId].modbusDevice=nullptr;
       }
   }
   /*判断数据源是否连接*/
   if(dataSourceInfor[sourceId].modbusDevice==nullptr){
       /*连接指定的服务器*/
       QModbusClient  *modbusDevice = new QModbusRtuSerialMaster(this);
       if (modbusDevice){
           modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, dataSourceInfor[sourceId].com);
           modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
               dataSourceInfor[sourceId].parity);
           modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
              dataSourceInfor[sourceId].baudRate);
           modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
               dataSourceInfor[sourceId].dataBits);
           modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
               dataSourceInfor[sourceId].stopBits);
           modbusDevice->setTimeout(dataSourceInfor[sourceId].responseTime.toInt());
           modbusDevice->setNumberOfRetries(dataSourceInfor[sourceId].numberOfRetries.toInt());
           modbusDevice->connectDevice();
           if(modbusDevice->state()==QModbusDevice::UnconnectedState ||
              modbusDevice->state()==QModbusDevice::ClosingState ){
               modbusDevice->disconnectDevice();
               delete  modbusDevice;
               json.insert("result","err");
               json.insert("value","连接数据源失败");
               document.setObject(json);
               QString json_str(document.toJson(QJsonDocument::Compact));
               return json_str;
           }
           else{
                dataSourceInfor[sourceId].modbusDevice=modbusDevice;
                connect(dataSourceInfor[sourceId].modbusDevice,SIGNAL(errorOccurred(QModbusDevice::Error)),this,SLOT(handleSerialError(QModbusDevice::Error )));
           }
       }
       else{
        json.insert("result","err");
        json.insert("value","建立数据源对象失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
       }
   }
   /*构建moubus读数据块*/
   QModbusDataUnit requestData=QModbusDataUnit(static_cast<QModbusDataUnit::RegisterType>(dataSetInfor[id.toInt()].readData.code),
                     dataSetInfor[id.toInt()].readData.startAddress.toInt(),
                     dataSetInfor[id.toInt()].readData.length.toInt());
   dataSourceInfor[sourceId].m_mutex.lock();
   QModbusReply *reply = dataSourceInfor[sourceId].modbusDevice->sendReadRequest(requestData,
                        dataSetInfor[id.toInt()].readData.serverAddress.toInt());
   if (reply){
       if (!reply->isFinished()){
           dataSetInfor[id.toInt()].readData.reply=reply;
           connect(reply, SIGNAL(finished()), this, SLOT(onReadReady()));
       }
       else{
           json.insert("result","err");
           json.insert("value","发送请求失败");
           document.setObject(json);
           QByteArray byte_array = document.toJson(QJsonDocument::Compact);
           QString json_str(byte_array);
           delete reply;
           dataSourceInfor[sourceId].m_mutex.unlock();
           return json_str;
       }
   }
   else {
       json.insert("result","err");
       json.insert("value","发送请求失败");
       document.setObject(json);
       QByteArray byte_array = document.toJson(QJsonDocument::Compact);
       QString json_str(byte_array);
       delete reply;
       dataSourceInfor[sourceId].m_mutex.unlock();
       return json_str;
   }
   //添加事件循环机制，返回后再运行后面的
    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    while (dataSetInfor[id.toInt()].readData.value=="") {
         QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
    }
    delete reply;
    dataSourceInfor[sourceId].m_mutex.unlock();
    dataSetInfor[id.toInt()].readData.reply=nullptr;
    return  dataSetInfor[id.toInt()].readData.value;
}
/*设置数据集值*/
QString SourceManage::setValue(QString id, QString value)
{
    return "设置数据ok";
}
/*数据读取槽*/
void SourceManage::onReadReady()
{
    QJsonDocument document;
    QJsonObject json;
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit result = reply->result();
        for (int i=0;i<MaxDataSet;i++) {
           if(reply==dataSetInfor[i].readData.reply)
           {
               QString response="";
               for (int j = 0;j<result.valueCount();j++) {
                   response.append(QString::number(result.value(j)));
               }
               json.insert("result","ok");
               json.insert("value",response);
               document.setObject(json);
               QByteArray byte_array = document.toJson(QJsonDocument::Compact);
               QString json_str(byte_array);
               dataSetInfor[i].readData.value=json_str;
               reply->deleteLater();
               return;
           }
        }
    }
    else{
        for (int i=0;i<MaxDataSet;i++) {
           if(reply==dataSetInfor[i].readData.reply)
           {
               json.insert("result","err");
               json.insert("value",reply->errorString());
               document.setObject(json);
               QByteArray byte_array = document.toJson(QJsonDocument::Compact);
               QString json_str(byte_array);
               dataSetInfor[i].readData.value=json_str;
               reply->deleteLater();
               return;
           }
        }

    }
}

void SourceManage::handleSerialError(QModbusDevice::Error err)
{
    if(err==QModbusDevice::ConnectionError){
        qDebug()<<"RTU连接设备断开";
        auto reply = qobject_cast<QModbusDevice *>(sender());
        for (int i=0;i<MaxDataSource;i++) {
           if(reply==dataSourceInfor[i].modbusDevice)
           {
               dataSourceInfor[i].modbusDevice->disconnectDevice();
               dataSourceInfor[i].modbusDevice=nullptr;
               reply->deleteLater();
               return;
           }
        }
    }
}
/*加载信息*/
void SourceManage::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/modbusRTU.ini";
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
        dataSourceInfor[index].com=json.value("com").toString();
        dataSourceInfor[index].baudRate=static_cast<QSerialPort::BaudRate>(json.value("baudRate").toInt());
        dataSourceInfor[index].stringbaudRate=json.value("stringbaudRate").toString();
        dataSourceInfor[index].parity=static_cast<QSerialPort::Parity>(json.value("parity").toInt());
        dataSourceInfor[index].stringparity=json.value("stringparity").toString();
        dataSourceInfor[index].dataBits=static_cast<QSerialPort::DataBits>(json.value("dataBits").toInt());
        dataSourceInfor[index].stringdataBits=json.value("stringdataBits").toString();
        dataSourceInfor[index].stopBits=static_cast<QSerialPort::StopBits>(json.value("stopBits").toInt());
        dataSourceInfor[index].stringstopBits=json.value("stringstopBits").toString();
        dataSourceInfor[index].responseTime=json.value("responseTime").toString();
        dataSourceInfor[index].numberOfRetries=json.value("numberOfRetries").toString();
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
        dataSetInfor[index].readData.code=static_cast<QModbusDataUnit::RegisterType>(json.value("readData_code").toInt());
        dataSetInfor[index].readData.stringCode=json.value("readData_stringCode").toString();
        dataSetInfor[index].readData.serverAddress=json.value("readData_serverAddress").toString();
        dataSetInfor[index].readData.startAddress=json.value("readData_startAddress").toString();
        dataSetInfor[index].readData.length=json.value("readData_length").toString();
        dataSetInfor[index].writeData.code=static_cast<QModbusDataUnit::RegisterType>(json.value("writeData_code").toInt());
        dataSetInfor[index].writeData.stringCode=json.value("writeData_stringCode").toString();
        dataSetInfor[index].writeData.serverAddress=json.value("writeData_serverAddress").toString();
        dataSetInfor[index].writeData.startAddress=json.value("writeData_startAddress").toString();
        dataSetInfor[index].writeData.length=json.value("writeData_length").toString();

   }
}
