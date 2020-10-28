#include "configdialog.h"
#include "ui_configdialog.h"
#include <iostream>
#include "QDesktopWidget"
#include "configdialog.h"
#include <QThread>
ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();  //可用区域
    this->resize(deskRect.width()/2,deskRect.height()/1.2);
    loadConfig();        //加载配置信息
    connect(ui->connectTest,SIGNAL(clicked()),this,SLOT(connectTest()));
    connect(ui->clearService,SIGNAL(clicked()),this,SLOT(clearService()));
    connect(ui->saveService,SIGNAL(clicked()),this,SLOT(setService()));
    connect(ui->publish,SIGNAL(clicked()),this,SLOT(publishTest()));
    connect(ui->clearTopic,SIGNAL(clicked()),this,SLOT(clearTopic()));
    connect(ui->saveTopic,SIGNAL(clicked()),this,SLOT(setTopic()));
    connect(ui->index1,SIGNAL(valueChanged(int)),this,SLOT(fillServiceForm()));
    connect(ui->index2,SIGNAL(valueChanged(int)),this,SLOT(fillTopicForm()));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}
/*窗体显示事件*/
void ConfigDialog::showEvent(QShowEvent *)
{
    RequestMetaData_dialog request;
    request.type="getDrivesInfor";
    emit SendMsgToContainerManage(request);
    m_delegate=new ItemDelegate(this);
    serviceModel=new QStandardItemModel(this);  //建立服务器显示model实例
    topicModel=new QStandardItemModel(this);     //建立主题显示model实例
    ui->tableView1->setModel(serviceModel);
    ui->tableView2->setModel(topicModel);
    ui->tableView2->setItemDelegateForColumn(1,m_delegate);
    ui->tableView2->setItemDelegateForColumn(2,m_delegate);
    ui->index1->setMaximum(MaxService-1);
    ui->index2->setMaximum(MaxTopic-1);
     showService();        //显示服务器信息
     fillServiceForm();    //填充服务器表单
     fillServiceBox();     //填充服务器下拉框
     createActions();
     fillTopicForm();   //填充主题表单
    while(driveInfor==""){

    }
    QJsonDocument document = QJsonDocument::fromJson(driveInfor.toUtf8());
    QJsonObject json = document.object();
    QStringList list;
    list.clear();
    for(QJsonObject::Iterator it=json.begin();it!=json.end();it++)
    {
      list.append(it.key());
        }
        m_delegate->setDriveInfor(list);
        driveInfor="";
        QJsonObject json_all;
        for(int i = 0; i< list.size();++i)
        {
            request.type="getDataSetInfor";
            request.drive=list.at(i);
            emit SendMsgToContainerManage(request);
            while(dateSetInfor==""){
            }
            document = QJsonDocument::fromJson(dateSetInfor.toUtf8());
            QJsonArray array= document.array();
            json_all.insert(list.at(i),array);
        }
        m_delegate->setDataSetInfor(json_all);
}
/*从容器管理器接收消息(曹)*/
void ConfigDialog::receiveMsgFromContainerManage(ResponseMetaData_dialog response)
{
    if(response.type=="getDrivesInfor"){
        driveInfor=response.value;
    }
    if(response.type=="getDataSetInfor"){
        dateSetInfor=response.value;
    }
    if(response.type=="getValue"){
        topicInfor[ui->index2->text().toInt()].getValueResult=response.value;
    }
}
void ConfigDialog::contextMenuEvent(QContextMenuEvent *e)
{
    menu->exec (QCursor::pos ());
}
void ConfigDialog::createActions()
{
    topicModel->clear();
    QStringList list;
    list<<tr("字段名")<<tr("驱动")<<tr("数据集")<<tr("数据集索引");
    topicModel->setHorizontalHeaderLabels(list);
    topicModel->insertRow(0);
    menu=new QMenu(this);
    add_row=new QAction(tr("添加行"),this);
    del_row=new QAction(tr("删除行"),this);
    menu->addAction(add_row);
    menu->addAction(del_row);
    connect (add_row,SIGNAL(triggered(bool)),this,SLOT(AddRow()));
    connect (del_row,SIGNAL(triggered(bool)),this,SLOT(RemoveRow()));
}

void ConfigDialog::RemoveRow()
{
    topicModel->removeRow(ui->tableView2->currentIndex().row());
}

void ConfigDialog::AddRow()
{
    topicModel->insertRow(ui->tableView2->currentIndex().row()+1);
}

/*显示服务器信息*/
void ConfigDialog::showService()
{
    serviceModel->clear();
    QStringList list;
    list<<tr("名称")<<tr("使能")<<tr("说明")<<tr("用户名")
        <<tr("密码")<<tr("地址")<<tr("端口号")<<tr("超时时间");
    serviceModel->setHorizontalHeaderLabels(list);
    for(int index=0;index<MaxService;index++  ){
       QList<QStandardItem *>  list;
       QString enable="True";
       if(serviceInfor[index].enable==false){
          enable="False";
       }
       list<<new QStandardItem(serviceInfor[index].name)<<
            new QStandardItem(enable)<<
            new QStandardItem(serviceInfor[index].desc)<<
            new QStandardItem(serviceInfor[index].username)<<
            new QStandardItem(serviceInfor[index].password)<<
            new QStandardItem(serviceInfor[index].address)<<
            new QStandardItem(serviceInfor[index].port)<<
            new QStandardItem(serviceInfor[index].timeOut);
        serviceModel->appendRow(list);
    }
}
/*显示匹配规则*/
void ConfigDialog::showRules()
{
       QString Topic=ui->name2->text();
       topicModel->clear();
       QStringList list;
       list<<tr("字段名")<<tr("驱动")<<tr("数据集")<<tr("数据集索引");
       topicModel->setHorizontalHeaderLabels(list);
       /*如果当前索引下配置的主题为数据下拉框当前项 且 当前索引下的匹配规则存在*/
       if(topicInfor[ui->index2->value()].name==Topic and topicInfor[ui->index2->value()].rules!=""){
           QJsonDocument document = QJsonDocument::fromJson(topicInfor[ui->index2->value()].rules.toUtf8());
           QJsonArray array= document.array();
           for (int i = 0; i < array.count(); i++)
            {
               QJsonObject value = array.at(i).toObject();
               QList<QStandardItem *>  list;
               list<<new QStandardItem(value["field"].toString())<<
                     new QStandardItem(value["drive"].toString())<<
                     new QStandardItem(value["dataName"].toString())<<
                     new QStandardItem(value["dataIndex"].toString());
               topicModel->appendRow(list);
            }
       }
       else{
              topicModel->insertRow(0);
       }
}
/*设置服务器信息*/
void ConfigDialog::setService()
{
    int index=ui->index1->value();
    if(serviceInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号服务器已有配置信息，是否更新??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    if(ui->name1->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("名称不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->username->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("用户名不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->password->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("密码不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->address->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("地址不能为空!!!"),QMessageBox::Yes);
      return;
       }
    serviceInfor[index].name=ui->name1->text();
    serviceInfor[index].enable=ui->enable1->isChecked();
    serviceInfor[index].desc=ui->desc1->text();
    serviceInfor[index].username=ui->username->text();
    serviceInfor[index].password=ui->password->text();
    serviceInfor[index].address=ui->address->text();
    serviceInfor[index].port=ui->port->text();
    serviceInfor[index].timeOut=QString::number(ui->timeOut->value());
    QMessageBox::information(this,tr("提示"),tr("设置服务器成功"),QMessageBox::Yes);
    saveConfig();  //保存配置信息
    showService();
    fillServiceBox();
}
/*设置主题信息*/
void ConfigDialog::setTopic()
{
    int index=ui->index2->value();
    if(topicInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号主题已有配置信息，是否更新??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    QJsonArray array;
    for(int index=0;index<topicModel->rowCount();index++){
            QJsonObject json;
            json.insert("field",topicModel->data(topicModel->index(index,0)).toString());
            json.insert("drive",topicModel->data(topicModel->index(index,1)).toString());
            json.insert("dataName",topicModel->data(topicModel->index(index,2)).toString());
            json.insert("dataIndex",topicModel->data(topicModel->index(index,3)).toString());
            array.push_back(json);
    }
    QJsonDocument document;
    document.setArray(array);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    topicInfor[index].service=ui->service->currentData().toString();
    topicInfor[index].name=ui->name2->text();
    topicInfor[index].topic=ui->topic->text();
    topicInfor[index].enable=ui->enable2->isChecked();
    topicInfor[index].frequency=QString::number(ui->frequency->value(),10);
    topicInfor[index].desc=ui->desc2->text();
    topicInfor[index].rules=json_str;
    QMessageBox::information(this,tr("提示"),tr("设置主题成功"),QMessageBox::Yes);
    saveConfig();  //保存配置信息
}

void ConfigDialog::clearService()
{
    int index=ui->index1->value();
    if(serviceInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号服务器已有配置信息，是否清除??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    serviceInfor[index].name="";
    serviceInfor[index].enable=true;
    serviceInfor[index].desc="";
    serviceInfor[index].username="";
    serviceInfor[index].password="";
    serviceInfor[index].address="";
    serviceInfor[index].port="";
    serviceInfor[index].timeOut="";
    ui->name1->setText("");
    ui->desc1->setText("");
    ui->username->setText("");
    ui->password->setText("");
    ui->address->setText("");
    ui->port->setText("");
    ui->timeOut->setValue(0);
    QMessageBox::information(this,tr("提示"),tr("清除服务器信息成功"),QMessageBox::Yes);
    saveConfig();
    showService();
    fillServiceBox();
}

void ConfigDialog::clearTopic()
{
    int index=ui->index2->value();
    if(serviceInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号主题已有配置信息，是否清除??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    topicInfor[index].service="";
    topicInfor[index].name="";
    topicInfor[index].topic="";
    topicInfor[index].enable=false;
    topicInfor[index].frequency="";
    topicInfor[index].desc="";
    topicInfor[index].rules="";
    ui->service->setCurrentIndex(0);
    ui->name2->setText("");
    ui->topic->setText("");
    ui->enable2->setCheckState(Qt::Unchecked);
    ui->frequency->setValue(0);
    ui->desc2->setText("");
    topicModel->clear();
    QStringList list;
    list<<tr("字段名")<<tr("驱动")<<tr("数据集")<<tr("数据集索引");
    topicModel->setHorizontalHeaderLabels(list);
    topicModel->insertRow(0);
    QMessageBox::information(this,tr("提示"),tr("清除服务器信息成功"),QMessageBox::Yes);
    saveConfig();
}
/*填充服务器表单*/
void ConfigDialog::fillServiceForm()
{
    int index=ui->index1->value();
    if(serviceInfor[index].name!=""){      //如果已经有配置信息，则填充回表单
        ui->name1->setText(serviceInfor[index].name);
        ui->enable1->setChecked(serviceInfor[index].enable);
        ui->desc1->setText(serviceInfor[index].desc);
        ui->username->setText(serviceInfor[index].username);
        ui->password->setText(serviceInfor[index].password);
        ui->address->setText(serviceInfor[index].address);
        ui->port->setText(serviceInfor[index].port);
        ui->timeOut->setValue(serviceInfor[index].timeOut.toUInt());
       }
    else{      //如果已经有配置信息，则填充回表单
        ui->name1->setText(serviceInfor[index].name);
        ui->enable1->setChecked(serviceInfor[index].enable);
        ui->desc1->setText(serviceInfor[index].desc);
        ui->timeOut->setValue(serviceInfor[index].timeOut.toUInt());
       }
}
/*填充主题表单*/
void ConfigDialog::fillTopicForm()
{
    int index=ui->index2->value();
    ui->service->setCurrentIndex(ui->service->findData(topicInfor[index].service));
    ui->name2->setText(topicInfor[index].name);
    ui->enable2->setChecked(topicInfor[index].enable);
    ui->topic->setText(topicInfor[index].topic);
    ui->frequency->setValue(topicInfor[index].frequency.toInt());
    ui->desc2->setText(topicInfor[index].desc);
    showRules();
}
/*填充服务器下拉框*/
void ConfigDialog::fillServiceBox()
{
    ui->service->clear();
    for(int index=0;index<MaxService;index++)
    {
      if(serviceInfor[index].name!="")
      {
          ui->service->addItem(serviceInfor[index].name,index);
      }
    }
}

/*连接服务器测试*/
void ConfigDialog::connectTest()
{
    if(ui->address->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("地址不能为空!!!"),QMessageBox::Yes);
      return;
       }
    QMqttClient *client = new QMqttClient(this);
    client->setHostname(ui->address->text());
    client->setPort(ui->port->text().toUInt());
    client->setClientId(QString::number(ui->index1->value()));
    client->setUsername(ui->username->text());
    client->setPassword(ui->password->text());
    client->connectToHost();
    QTime dieTime = QTime::currentTime().addMSecs(ui->timeOut->value());
    while (QTime::currentTime() < dieTime&&(client->state()!=QMqttClient::Connected)) {
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    if(client->state()==QMqttClient::Connected){
        QMessageBox::information(this,tr("提示"),tr("连接服务器成功"),QMessageBox::Yes);
    }
    else{
        QMessageBox::warning(this,tr("提示"),tr("连接服务器失败"),QMessageBox::Yes);
    }
    client->disconnectFromHost();
    delete client;
}
/*发布主题测试*/
void ConfigDialog::publishTest()
{
    int server=ui->service->currentData().toInt();
    QMqttClient *client = new QMqttClient(this);
    client->setHostname(serviceInfor[server].address);
    client->setPort(serviceInfor[server].port.toUInt());
    client->setClientId(QString::number(server));
    client->setUsername(serviceInfor[server].username);
    client->setPassword(serviceInfor[server].password);
    client->connectToHost();
    QTime dieTime = QTime::currentTime().addMSecs(ui->timeOut->value());
    while (QTime::currentTime() < dieTime&&(client->state()!=QMqttClient::Connected)) {
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    if(!client->state()==QMqttClient::Connected){
        QMessageBox::warning(this,tr("提示"),tr("连接服务器失败"),QMessageBox::Yes);
        client->disconnectFromHost();
        delete  client;
        return;
    }
    QJsonObject json;
     for (int i = 0; i < topicModel->rowCount(); i++)
      {
         if(topicModel->data(topicModel->index(i,2)).toString()!=""){
             RequestMetaData_dialog request;
             request.type="getValue";
             request.drive=topicModel->data(topicModel->index(i,1)).toString();
             request.index=topicModel->data(topicModel->index(i,3)).toString();
             emit SendMsgToContainerManage(request);
             while(topicInfor[ui->index2->text().toInt()].getValueResult==""){
                  QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
             }
             QJsonDocument document=QJsonDocument::fromJson(topicInfor[ui->index2->text().toInt()].getValueResult.toLocal8Bit().data());
             topicInfor[ui->index2->text().toInt()].getValueResult="";
              if(document.object().value("result").toString()=="err"){
              QMessageBox::warning(this,tr("提示"),tr("获取数据")+topicModel->data(topicModel->index(i,0)).toString()+"失败："
                                   +document.object().value("value").toString(),QMessageBox::Yes);
             }
             else{
              if(document.object().value("value").isObject()){
                  document.setObject(document.object().value("value").toObject());
                  QString json_str(document.toJson(QJsonDocument::Compact));
                  json.insert(topicModel->data(topicModel->index(i,0)).toString(),json_str);
              }
              else{
                 json.insert(topicModel->data(topicModel->index(i,0)).toString(),document.object().value("value").toString());
              }
              }
          }
      }
     if(json.isEmpty()){
       QMessageBox::warning(this,tr("提示"),tr("未成功获取到有效可发布数据!!!"),QMessageBox::Yes);
     }
     else{
         QJsonDocument document1;
         document1.setObject(json);
         QByteArray byte_array = document1.toJson(QJsonDocument::Compact);
         QString json_str(byte_array);
         if (client->publish(ui->topic->text(),json_str.toUtf8()) == -1){
            QMessageBox::warning(this,tr("提示"),tr("发布主题失败!!!"),QMessageBox::Yes);
         }
         else {
            QMessageBox::information(this,tr("提示"),tr("发布主题成功"),QMessageBox::Yes);
         }
     }

      client->disconnectFromHost();
      delete client;
 }
/*保存信息*/
void ConfigDialog::saveConfig()
{
    QJsonArray serviceArray;
    for(int index=0;index<MaxService;index++){
            QJsonObject json;
            json.insert("name",serviceInfor[index].name);
            json.insert("enable",serviceInfor[index].enable);
            json.insert("desc",serviceInfor[index].desc);
            json.insert("username",serviceInfor[index].username);
            json.insert("desc",serviceInfor[index].desc);
            json.insert("password",serviceInfor[index].password);
            json.insert("address",serviceInfor[index].address);
            json.insert("port",serviceInfor[index].port);
            json.insert("timeOut",serviceInfor[index].timeOut);
            serviceArray.push_back(json);
    }
    QJsonArray topicArray;
    for(int index=0;index<MaxTopic;index++){
            QJsonObject json;
            json.insert("name",topicInfor[index].name);
            json.insert("service",topicInfor[index].service);
            json.insert("enable",topicInfor[index].enable);
            json.insert("topic",topicInfor[index].topic);
            json.insert("frequency",topicInfor[index].frequency);
            json.insert("desc",topicInfor[index].desc);
            json.insert("rules",topicInfor[index].rules);
            topicArray.push_back(json);
    }
    QJsonObject object;
    object.insert("service",serviceArray);
    object.insert("topic",topicArray);
    QJsonDocument document;
    document.setObject(object);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/mqtt_pub.ini";
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly|QIODevice::Text)) { //如果文件不存在则新建文件
        file.open( QIODevice::ReadWrite | QIODevice::Text );
    }
    file.write(data);
    file.close ();
}
/*加载信息*/
void ConfigDialog::loadConfig()
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

