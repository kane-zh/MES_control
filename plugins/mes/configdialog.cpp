#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();  //可用区域
    this->resize(deskRect.width()/2,deskRect.height()/1.2);
    loadConfig();                            //加载配置信息
    request=new httpRequest(this);
    connect(ui->index1,SIGNAL(valueChanged(int)),this,SLOT(fillServerInforForm()));
    connect(ui->index2,SIGNAL(valueChanged(int)),this,SLOT(fillRecordInforForm()));
    connect(ui->clearServerInfor,SIGNAL(clicked()),this,SLOT(clearServerInfor()));
    connect(ui->saveServerInfor,SIGNAL(clicked()),this,SLOT(setServerInfor()));
    connect(ui->getDataTest,SIGNAL(clicked()),this,SLOT(saveValueTest()));
    connect(ui->clearRecordInfor,SIGNAL(clicked()),this,SLOT(clearRecordInfor()));
    connect(ui->saveRecordInfor,SIGNAL(clicked()),this,SLOT(setRecordInfor()));
    connect(ui->connectTest,SIGNAL(clicked()),this,SLOT(connectTest()));
}

ConfigDialog::~ConfigDialog()
{
    delete  request;
    delete ui;
}

/*窗体显示事件*/
void ConfigDialog::showEvent(QShowEvent *)
{
    RequestMetaData_dialog request;
    request.type="getDrivesInfor";
    driveInfor="";
    emit SendMsgToPluginInterface(request);
    m_delegate=new ItemDelegate(this);
    serverModel=new QStandardItemModel(this);  //建立服务器显示model实例
    recordModel=new QStandardItemModel(this);     //建立记录信息显示model实例
    ui->tableView1->setModel(serverModel);
    ui->tableView2->setModel(recordModel);
    ui->tableView2->setItemDelegateForColumn(1,m_delegate);
    ui->tableView2->setItemDelegateForColumn(2,m_delegate);
    ui->index1->setMaximum(MaxServer-1);
    ui->index2->setMaximum(MaxRecord-1);
     showServerInfor();        //显示服务器信息
     fillServerInforForm();    //填充服务器表单
     fillServerInforBox();     //填充服务器下拉框
     fillRecordInforForm();   //填充记录信息表单
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
    QJsonObject json_all;
    for(int i = 0; i< list.size();++i)
    {
        request.type="getDataSetInfor";
        request.drive=list.at(i);
        dateSetInfor="";
        emit SendMsgToPluginInterface(request);
        while(dateSetInfor==""){
        }
        document = QJsonDocument::fromJson(dateSetInfor.toUtf8());
        QJsonArray array= document.array();
        json_all.insert(list.at(i),array);
    }
     m_delegate->setDataSetInfor(json_all);
}
/*从容器管理器接收消息(回调)*/
void ConfigDialog::receiveMsgFromPluginInterface(ResponseMetaData_dialog response)
{
    if(response.type=="getDrivesInfor"){
        driveInfor=response.value;
    }
    if(response.type=="getDataSetInfor"){
        dateSetInfor=response.value;
    }
    if(response.type=="getValue"){
        getValueResult=response.value;
    }
}

/*显示服务器信息*/
void ConfigDialog::showServerInfor()
{
    serverModel->clear();
    QStringList list;
    list<<tr("名称")<<tr("使能")<<tr("说明")<<tr("用户名")
        <<tr("密码")<<tr("地址");
    serverModel->setHorizontalHeaderLabels(list);
    for(int index=0;index<MaxServer;index++  ){
       if(serverInfor[index].name!=""){
           QList<QStandardItem *>  list;
           QString enable="True";
           if(serverInfor[index].enable==false){
              enable="False";
           }
           list<<new QStandardItem(serverInfor[index].name)<<
                 new QStandardItem(enable)<<
                 new QStandardItem(serverInfor[index].desc)<<
                 new QStandardItem(serverInfor[index].username)<<
                 new QStandardItem(serverInfor[index].password)<<
                 new QStandardItem(serverInfor[index].address);
            serverModel->appendRow(list);
        }
    }
}
/*设置服务器信息*/
void ConfigDialog::setServerInfor()
{
    int index=ui->index1->value();
    if(serverInfor[index].name!=""){
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
    serverInfor[index].name=ui->name1->text();
    serverInfor[index].enable=ui->enable1->isChecked();
    serverInfor[index].desc=ui->desc1->text();
    serverInfor[index].username=ui->username->text();
    serverInfor[index].password=ui->password->text();
    serverInfor[index].address=ui->address->text();
    QMessageBox::information(this,tr("提示"),tr("设置服务器成功"),QMessageBox::Yes);
    saveConfig();     //保存配置信息
    showServerInfor();
    fillServerInforBox();
}
/*设置记录信息信息*/
void ConfigDialog::setRecordInfor()
{
    int index=ui->index2->value();
    if(recordInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号记录信息已有配置信息，是否更新??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    if(ui->name2->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("名称不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->serverInfor->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("服务器信息不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->serverId->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("服务地址不能为空!!!"),QMessageBox::Yes);
      return;
       }
    QJsonArray array;
    for(int id=0;id<recordModel->rowCount();id++){
            QJsonObject json;
            json.insert("field",recordModel->data(recordModel->index(id,0)).toString());
            json.insert("drive",recordModel->data(recordModel->index(id,1)).toString());
            json.insert("dataName",recordModel->data(recordModel->index(id,2)).toString());
            json.insert("dataId",recordModel->data(recordModel->index(id,3)).toString());
            array.push_back(json);
    }
    QJsonDocument document;
    document.setArray(array);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    recordInfor[index].server=ui->serverInfor->currentData().toString();
    recordInfor[index].name=ui->name2->text();
    recordInfor[index].serverid=ui->serverId->text();
    recordInfor[index].enable=ui->enable2->isChecked();
    recordInfor[index].frequency=QString::number(ui->frequency->value(),10);
    recordInfor[index].desc=ui->desc2->text();
    recordInfor[index].rules=json_str;
    QMessageBox::information(this,tr("提示"),tr("设置记录信息成功"),QMessageBox::Yes);
    saveConfig();     //保存配置信息
}
/*清除服务器信息*/
void ConfigDialog::clearServerInfor()
{
    int index=ui->index1->value();
    if(serverInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号服务器已有配置信息，是否清除??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    serverInfor[index].name="";
    serverInfor[index].enable=true;
    serverInfor[index].desc="";
    serverInfor[index].address="";
    serverInfor[index].username="";
    serverInfor[index].password="";
    ui->name1->setText("");
    ui->desc1->setText("");
    ui->address->setText("");
    ui->username->setText("");
    ui->password->setText("");
    QMessageBox::information(this,tr("提示"),tr("清除服务器信息成功"),QMessageBox::Yes);
    saveConfig();     //保存配置信息
    showServerInfor();
    fillServerInforBox();
}

void ConfigDialog::clearRecordInfor()
{
    int index=ui->index2->value();
    if(recordInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号数据表已有配置信息，是否清除??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    recordInfor[index].name="";
    recordInfor[index].enable=false;
    recordInfor[index].server="";
    recordInfor[index].serverid="";
    recordInfor[index].frequency="";
    recordInfor[index].desc="";
    recordInfor[index].rules="";
    ui->name2->setText("");
    ui->serverInfor->setCurrentIndex(0);
    ui->serverId->setText("");
    ui->enable2->setCheckState(Qt::Unchecked);
    ui->frequency->setValue(0);
    ui->desc2->setText("");
    recordModel->clear();
    QMessageBox::information(this,tr("提示"),tr("清除信息成功"),QMessageBox::Yes);
    saveConfig();

}
/*填充服务器表单*/
void ConfigDialog::fillServerInforForm()
{
    int index=ui->index1->value();
    if(serverInfor[index].name!=""){      //如果已经有配置信息，则填充回表单
        ui->name1->setText(serverInfor[index].name);
        ui->enable1->setChecked(serverInfor[index].enable);
        ui->desc1->setText(serverInfor[index].desc);
        ui->username->setText(serverInfor[index].username);
        ui->password->setText(serverInfor[index].password);
        ui->address->setText(serverInfor[index].address);
       }
    else{
        ui->name1->setText(serverInfor[index].name);
        ui->enable1->setChecked(serverInfor[index].enable);
        ui->desc1->setText(serverInfor[index].desc);
       }
}
/*填充记录信息表单*/
void ConfigDialog::fillRecordInforForm()
{
    int index=ui->index2->value();
    ui->serverInfor->setCurrentIndex(ui->serverInfor->findData(recordInfor[index].server));
    ui->name2->setText(recordInfor[index].name);
    ui->serverId->setText(recordInfor[index].serverid);
    ui->enable2->setChecked(recordInfor[index].enable);
    ui->frequency->setValue(recordInfor[index].frequency.toInt());
    ui->desc2->setText(recordInfor[index].desc);
    /*如果当前索引下配置的记录信息为数据下拉框当前项 且 当前索引下的匹配规则存在*/
    if(recordInfor[index].rules!=""){
        recordModel->clear();
        QStringList list;
        list<<tr("字段名")<<tr("驱动")<<tr("数据集")<<tr("数据集索引");
        recordModel->setHorizontalHeaderLabels(list);
        QJsonDocument document = QJsonDocument::fromJson(recordInfor[index].rules.toUtf8());
        QJsonArray array= document.array();
        for (int i = 0; i < array.count(); i++)
         {
            QJsonObject value = array.at(i).toObject();
            QList<QStandardItem *>  list;
            list<<new QStandardItem(value["field"].toString())<<
                  new QStandardItem(value["drive"].toString())<<
                  new QStandardItem(value["dataName"].toString())<<
                  new QStandardItem(value["dataId"].toString());
            recordModel->appendRow(list);
        }
       }
        else{
            recordModel->clear();
            QStringList list;
            list<<tr("字段名")<<tr("驱动")<<tr("数据集")<<tr("数据集索引");
            recordModel->setHorizontalHeaderLabels(list);
            QStringList filterList;
            filterList<<tr("type")<<tr("runTime")<<tr("allTime")<<tr("sum")<<tr("task")<<tr("util_rate")<<tr("comp_rate")<<tr("handler")
                      <<tr("attribute1")<<tr("attribute2")<<tr("attribute3")<<tr("attribute4")<<tr("attribute5")
                      <<tr("attribute6")<<tr("attribute7")<<tr("attribute8")<<tr("attribute9")<<tr("attribute10");
            for (int i=0;i<filterList.count();i++) {
                QList<QStandardItem *>  list;
                list<<new QStandardItem(filterList.at(i))<<
                      new QStandardItem("")<<
                      new QStandardItem("")<<
                      new QStandardItem("");
                recordModel->appendRow(list);
            }
        }
}
/*填充服务器下拉框*/
void ConfigDialog::fillServerInforBox()
{
    ui->serverInfor->clear();
    for(int index=0;index<MaxServer;index++)
    {
      if(serverInfor[index].name!="")
      {
          ui->serverInfor->addItem(serverInfor[index].name,index);
      }
    }
}
/*连接服务器测试*/
void ConfigDialog::connectTest()
{
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
    QString result=request->login(ui->address->text(),ui->username->text(),ui->password->text());
    if(result=="err"){
      QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
    }
    else {
      QMessageBox::information(this,tr("提示"),tr("连接服务器成功!!!"),QMessageBox::Yes);
    }

}
/*保存数据测试*/
void ConfigDialog::saveValueTest()
{
    if(ui->serverId->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("记录索引不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->serverInfor->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("服务器信息不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->serverId->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("服务地址不能为空!!!"),QMessageBox::Yes);
      return;
       }
    int serverIndex=ui->serverInfor->currentData().toInt();
    QString result;
    if(serverInfor[serverIndex].token==""){
        result=request->login(serverInfor[serverIndex].address,serverInfor[serverIndex].username,serverInfor[serverIndex].password);
        if(result=="err"){
          QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
          return;
        }
        else {
          serverInfor[serverIndex].token=result;
        }
    }
    int  recordIndex=ui->index2->value();
    QJsonDocument document = QJsonDocument::fromJson(recordInfor[recordIndex].rules.toUtf8());
    QJsonArray array= document.array();
    QJsonObject json;
    for (int i = 0; i < array.count(); i++)
     {
        if(!this->isVisible()){
           return;
        }
        QJsonObject value = array.at(i).toObject();
        if(value["dataId"].toString()!=""){
            RequestMetaData_dialog data;
            data.type="getValue";
            data.drive=value["drive"].toString();
            data.id=value["dataId"].toString();
            getValueResult="";
            emit SendMsgToPluginInterface(data);
            while(getValueResult==""){
            }
           QJsonDocument document_result = QJsonDocument::fromJson(getValueResult.toUtf8());
           if(document_result.object().value("result").toString()=="err"){
           QMessageBox::warning(this,tr("提示"),tr("获取数据")+recordModel->data(recordModel->index(i,0)).toString()+"失败："
                                +document_result.object().value("value").toString(),QMessageBox::Yes);
           }
          else{
              json.insert(value["field"].toString(),document_result.object().value("value").toString());
           }
         }
     }
    if(json.isEmpty()){
        QMessageBox::warning(this,tr("提示"),tr("未成功获取到有效可写数据!!!"),QMessageBox::Yes);
    }
    else{
        document.setObject(json);
        QByteArray byte_array = document.toJson(QJsonDocument::Compact);
        result=request->update(ui->address->text()+"/equipment/equipmentState/"+ui->serverId->text()+"/",serverInfor[serverIndex].token,byte_array);
        if(result=="err"){
          serverInfor[serverIndex].token="";
          QMessageBox::warning(this,tr("提示"),tr("更新记录失败!!!"),QMessageBox::Yes);
        }
        else {
          QMessageBox::information(this,tr("提示"),tr("更新记录成功!!!"),QMessageBox::Yes);
        }
    }
}

/*保存信息*/
void ConfigDialog::saveConfig()
{
    QJsonArray serverArray;
    for(int index=0;index<MaxServer;index++){
        QJsonObject json;
        json.insert("name",serverInfor[index].name);
        json.insert("enable",QString::number(serverInfor[index].enable));
        json.insert("desc",serverInfor[index].desc);
        json.insert("username",serverInfor[index].username);
        json.insert("desc",serverInfor[index].desc);
        json.insert("password",serverInfor[index].password);
        json.insert("address",serverInfor[index].address);
        serverArray.push_back(json);
    }
    QJsonArray recordArray;
    for(int index=0;index<MaxRecord;index++){
        QJsonObject json;
        json.insert("name",recordInfor[index].name);
        json.insert("server",recordInfor[index].server);
        json.insert("serverid",recordInfor[index].serverid);
        json.insert("enable",QString::number(recordInfor[index].enable));
        json.insert("frequency",recordInfor[index].frequency);
        json.insert("desc",recordInfor[index].desc);
        json.insert("rules",recordInfor[index].rules);
        recordArray.push_back(json);
    }
    QJsonObject object;
    object.insert("server",serverArray);
    object.insert("record",recordArray);
    QJsonDocument document;
    document.setObject(object);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/mes.ini";
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
