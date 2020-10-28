#include "sourcemanage.h"
#include <QTime>
#include "configdialog.h"
SourceManage::SourceManage()
{
    loadConfig();            //加载配置信息
}

SourceManage::~SourceManage()
{

}
void SourceManage::showForm(QWidget *parent)
{
  ConfigDialog *m_dialog=new ConfigDialog(parent);
  m_dialog->exec();
  delete  m_dialog;
  loadConfig();    //加载配置信息
}
/*准备读数据信号*/
void SourceManage::onReadReady()
{
    QJsonDocument document;
    QJsonObject json;
    auto reply = qobject_cast<QTcpSocket *>(sender());
    for (int i=0;i<MaxDataSet;i++) {
       if(reply==dataSetInfor[i].socket)
       {
        QString result=reply->readAll();
        result.remove(result.length()-2,4);
        json.insert("result","ok");
        json.insert("value",result);
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        QString json_str(byte_array);
        dataSetInfor[i].value=json_str;
        dataSetInfor[i].socket->abort();
        dataSetInfor[i].socket=nullptr;
        reply->deleteLater();
        return;
       }
    }
}
/*通讯故障信号*/
void SourceManage::onError()
{
    auto reply = qobject_cast<QTcpSocket *>(sender());
        for (int i=0;i<MaxDataSet;i++) {
           if(reply==dataSetInfor[i].socket)
           {
            dataSetInfor[i].socket->abort();
            dataSetInfor[i].socket=nullptr;
            reply->deleteLater();
            return;
           }
    }
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
/*读取数据接口*/
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
    int  dataSet=id.toInt();
    int  dataSource=dataSetInfor[dataSet].sourceIndex.toInt();
     /*判断数据源连接状态(如果已经连接)*/
    if(dataSetInfor[dataSet].socket!=nullptr){
      if(dataSetInfor[dataSet].socket->state()!=QAbstractSocket::ConnectedState ){
            dataSetInfor[dataSet].socket->abort();
            delete  dataSetInfor[dataSet].socket;
            dataSetInfor[dataSet].socket=nullptr;
        }
    }
    /*判断数据源是否连接*/
    if(dataSetInfor[dataSet].socket==nullptr){
        /*连接指定的服务器*/
        QTcpSocket  *socket = new QTcpSocket(this);
        if (socket){
           socket->connectToHost(dataSourceInfor[dataSetInfor[dataSet].sourceIndex.toInt()].host,
                   dataSourceInfor[dataSetInfor[dataSet].sourceIndex.toInt()].port.toInt());
           //等待连接成功
           if(!socket->waitForConnected(3000))
           {
               socket->abort();
               delete  socket;
               json.insert("result","err");
               json.insert("value","连接服务器失败");
               document.setObject(json);
               QString json_str(document.toJson(QJsonDocument::Compact));
               return json_str;
           }
            else{
                 dataSetInfor[dataSet].socket=socket;
                 connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
                 connect(socket, SIGNAL(disconnected()), this, SLOT(onError()));
                 connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError()));
            }
        }
        else{
         json.insert("result","err");
         json.insert("value","建立数据集的服务器对象失败");
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
        }
    }
    dataSetInfor[dataSet].m_mutex.lock();
    QString RD = "RD "+dataSetInfor[dataSet].type+dataSetInfor[dataSet].number+"\r";
    dataSetInfor[dataSet].value="";
    dataSetInfor[dataSet].socket->write(RD.toUtf8().data());
    QTime dieTime = QTime::currentTime().addMSecs(dataSetInfor[dataSet].timeout.toInt());
    while (dataSetInfor[dataSet].value==""&&QTime::currentTime() < dieTime) {
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    if(dataSetInfor[dataSet].value==""){
       json.insert("result","err");
       json.insert("value","读取节点值超时");
       document.setObject(json);
       QString json_str(document.toJson(QJsonDocument::Compact));
       dataSetInfor[dataSet].socket->abort();
       dataSetInfor[dataSet].socket=nullptr;
       return json_str;

    }
    dataSetInfor[dataSet].m_mutex.unlock();
    return  dataSetInfor[dataSet].value;
}

QString SourceManage::setValue(QString id, QString value)
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
    /*判断数据集写使能情况*/
    if(dataSetInfor[id.toInt()].writeEnable!=true){
        json.insert("result","err");
        json.insert("value","指定数据集未写使能");
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
    int  dataSet=id.toInt();
    int  dataSource=dataSetInfor[dataSet].sourceIndex.toInt();
     /*判断数据源连接状态(如果已经连接)*/
    if(dataSetInfor[dataSet].socket!=nullptr){
       if(dataSetInfor[dataSet].socket->state()!=QAbstractSocket::ConnectedState ){
            dataSetInfor[dataSet].socket->abort();
            dataSetInfor[dataSet].socket=nullptr;
        }
    }
    /*判断数据源是否连接*/
    if(dataSetInfor[dataSet].socket==nullptr){
        /*连接指定的服务器*/
        QTcpSocket  *socket = new QTcpSocket(this);
        if (socket){
           socket->connectToHost(dataSourceInfor[dataSetInfor[dataSet].sourceIndex.toInt()].host,
                   dataSourceInfor[dataSetInfor[dataSet].sourceIndex.toInt()].port.toInt());
           //等待连接成功
           if(!socket->waitForConnected(3000))
           {
               socket->abort();
               delete  socket;
               json.insert("result","err");
               json.insert("value","连接服务器失败");
               document.setObject(json);
               QString json_str(document.toJson(QJsonDocument::Compact));
               return json_str;
           }
            else{
                 dataSetInfor[dataSet].socket=socket;
                 connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
                 connect(socket, SIGNAL(disconnected()), this, SLOT(onError()));
                 connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError()));
            }
        }
        else{
         json.insert("result","err");
         json.insert("value","建立数据集的服务器对象失败");
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
        }
    }
    dataSetInfor[dataSet].m_mutex.lock();
    QString WR = "WR "+dataSetInfor[dataSet].type+dataSetInfor[dataSet].number+" "+value+"\r";
    dataSetInfor[dataSet].value="";
    dataSetInfor[dataSet].socket->write(WR.toUtf8().data());
    QTime dieTime = QTime::currentTime().addMSecs(dataSetInfor[dataSet].timeout.toInt());
    while (dataSetInfor[dataSet].value==""&&QTime::currentTime() < dieTime) {
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    if(dataSetInfor[dataSet].value==""){
       json.insert("result","err");
       json.insert("value","设置节点值超时");
       document.setObject(json);
       QString json_str(document.toJson(QJsonDocument::Compact));
       dataSetInfor[dataSet].socket->abort();
       dataSetInfor[dataSet].socket=nullptr;
       return json_str;
    }
    dataSetInfor[dataSet].m_mutex.unlock();
    return  dataSetInfor[dataSet].value;
}

/*加载信息*/
void SourceManage::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/keyence.ini";
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
        dataSetInfor[index].type=json.value("type").toString();
        dataSetInfor[index].number=json.value("number").toString();
        dataSetInfor[index].timeout=json.value("timeout").toString();
   }
}
