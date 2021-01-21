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
    request=new httpRequest(this);
    loadConfig();           //加载配置信息
    connect(ui->index1,SIGNAL(valueChanged(int)),this,SLOT(fillServerForm()));
    connect(ui->index2,SIGNAL(valueChanged(int)),this,SLOT(fillReportForm()));
    connect(ui->clearServer,SIGNAL(clicked()),this,SLOT(clearServer()));
    connect(ui->saveServer,SIGNAL(clicked()),this,SLOT(setServer()));
    connect(ui->clearReport,SIGNAL(clicked()),this,SLOT(clearReport()));
    connect(ui->saveReport,SIGNAL(clicked()),this,SLOT(setReport()));
    connect(ui->connectTest,SIGNAL(clicked()),this,SLOT(connectTest()));
}


ConfigDialog::~ConfigDialog()
{  
     delete request;
     delete ui;
}

/*窗体显示事件*/
void ConfigDialog::showEvent(QShowEvent *)
{
    serverModel=new QStandardItemModel(this);  //建立服务器显示model实例
    reportModel=new QStandardItemModel(this);  //建立汇报源显示model实例
    ui->tableView1->setModel(serverModel);
    ui->tableView2->setModel(reportModel);
    ui->index1->setMaximum(MaxServer-1);
    ui->index2->setMaximum(MaxReport-1);
    RequestMetaData_dialog request;
    request.type="getDataSetInfor";
    request.drive="FNAUC";
    dateSetInfor="";
    emit SendMsgToPluginInterface(request);
    while(dateSetInfor==""){
    }
    QJsonDocument document = QJsonDocument::fromJson(dateSetInfor.toUtf8());
    QJsonArray array=document.array();
    ui->state->clear();
    ui->program->clear();
    for (int i = 0; i < array.count(); i++)
    {
        QJsonObject value = array.at(i).toObject();
        ui->state->addItem(value["name"].toString(),value["id"].toDouble());
        ui->program->addItem(value["name"].toString(),value["id"].toDouble());
    }
    showServer();    //显示服务器信息
    showReport();       //显示汇报源信息
    fillServerForm();//填充服务器表单
    fillReportForm();   //填充汇报源表单
    fillServerBox(); //填充服务器下拉框
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
void ConfigDialog::showServer()
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
/*显示汇报源信息*/
void ConfigDialog::showReport()
{
    reportModel->clear();
    QStringList list;
    list<<tr("索引")<<tr("名称")<<tr("服务器名称")<<tr("服务器索引")<<tr("设备状态索引")<<tr("程序号索引")<<tr("使能")<<tr("说明");
    reportModel->setHorizontalHeaderLabels(list);
    for(int index=0;index<MaxReport;index++){
        if(reportInfor[index].name!=""){
            QList<QStandardItem *>  list;
            QString enable="True";
            if(reportInfor[index].enable==false){
               enable="False";
            }
              list<<new QStandardItem(QString::number(index))<<
                    new QStandardItem(reportInfor[index].name)<<
                    new QStandardItem(reportInfor[index].serverName)<<
                    new QStandardItem(reportInfor[index].serverIndex)<<
                    new QStandardItem(reportInfor[index].stateIndex)<<
                    new QStandardItem(reportInfor[index].programIndex)<<
                    new QStandardItem(enable)<<
                    new QStandardItem(reportInfor[index].desc);
              reportModel->appendRow(list);
        }
    }
}
/*设置服务器信息*/
void ConfigDialog::setServer()
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
    showServer();
    fillServerBox();
}
/*设置汇报源信息*/
void ConfigDialog::setReport()
{
    int index=ui->index2->value();
    if(reportInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号汇报源已有配置信息，是否更新??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    if(ui->name2->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("名称不能为空!!!"),QMessageBox::Yes);
      return;
    }
    if(ui->server->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("服务器不能为空!!!"),QMessageBox::Yes);
      return;
       }
    reportInfor[index].name=ui->name2->text();
    reportInfor[index].serverName=ui->server->currentText();
    reportInfor[index].serverIndex=ui->server->currentData().toString();
    reportInfor[index].stateIndex=ui->state->currentData().toString();
    reportInfor[index].programIndex=ui->program->currentData().toString();
    reportInfor[index].enable=ui->enable2->isChecked();
    reportInfor[index].desc=ui->desc2->text();
    QMessageBox::information(this,tr("提示"),tr("设置汇报源成功"),QMessageBox::Yes);
    saveConfig();      //保存配置信息
    showReport(); //显示汇报源信息(更新显示)
}
/*清除服务器内容*/
void ConfigDialog::clearServer()
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
    showServer();
    fillServerBox();
}
/*清除汇报源内容*/
void ConfigDialog::clearReport()
{
    int index=ui->index2->value();
    if(reportInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号汇报源已有配置信息，是否清除??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    reportInfor[index].name="";
    reportInfor[index].serverName="";
    reportInfor[index].serverIndex="";
    reportInfor[index].stateIndex="";
    reportInfor[index].programIndex="";
    reportInfor[index].enable=true;
    reportInfor[index].desc="";
    ui->name2->setText("");
    ui->desc2->setText("");
    QMessageBox::information(this,tr("提示"),tr("清除汇报源信息成功"),QMessageBox::Yes);
    saveConfig(); //保存配置信息
    showReport(); //显示汇报源信息(更新显示)
}
/*填充服务器表单(槽)*/
void ConfigDialog::fillServerForm()
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
/*填充汇报源表单(槽)*/
void ConfigDialog::fillReportForm()
{
    int index=ui->index2->value();
    if(reportInfor[index].name!=""){   //如果已经有配置信息，则填充回表单
    ui->name2->setText(reportInfor[index].name);
    ui->server->setCurrentIndex(ui->server->findData(reportInfor[index].serverIndex.toInt()));
    ui->state->setCurrentIndex(ui->state->findData(reportInfor[index].stateIndex.toInt()));
    ui->program->setCurrentIndex(ui->program->findData(reportInfor[index].programIndex.toInt()));
    ui->enable2->setChecked(reportInfor[index].enable);
    ui->desc2->setText(reportInfor[index].desc);
   }
    else{   //如果已经有配置信息，则填充回表单
    ui->name2->setText(reportInfor[index].name);
    ui->server->setCurrentIndex(ui->server->findData(reportInfor[index].serverIndex.toInt()));
    ui->enable2->setChecked(reportInfor[index].enable);
    ui->desc2->setText(reportInfor[index].desc);
   }
}
/*填充服务器下拉框*/
void ConfigDialog::fillServerBox()
{
    ui->server->clear();
    for(int index=0;index<MaxServer;index++)
    {
      if(serverInfor[index].name!="")
      {
          ui->server->addItem(serverInfor[index].name,id);
      }
    }
}
/*连接到服务器测试*/
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
/*保存信息*/
void ConfigDialog::saveConfig()
{
    QJsonArray serverArray;
    for(int index=0;index<MaxServer;index++){
        QJsonObject json;
        json.insert("name",serverInfor[index].name);
        json.insert("enable",serverInfor[index].enable);
        json.insert("desc",serverInfor[index].desc);
        json.insert("username",serverInfor[index].username);
        json.insert("desc",serverInfor[index].desc);
        json.insert("password",serverInfor[index].password);
        json.insert("address",serverInfor[index].address);
        serverArray.push_back(json);
    }
    QJsonArray ReportArray;
    for(int id=0;id<MaxReport;id++){
        QJsonObject json;
        json.insert("name",reportInfor[index].name);
        json.insert("serverName",reportInfor[index].serverName);
        json.insert("serverIndex",reportInfor[index].serverIndex);
        json.insert("stateIndex",reportInfor[index].stateIndex);
        json.insert("programIndex",reportInfor[index].programIndex);
        json.insert("dataSetIndex",reportInfor[index].serverIndex);
        json.insert("enable",reportInfor[index].enable);
        json.insert("desc",reportInfor[index].desc);
        ReportArray.push_back(json);
    }
    QJsonObject object;
    object.insert("server",serverArray);
    object.insert("report",ReportArray);
    QJsonDocument document;
    document.setObject(object);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/control4.ini";
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
   for(int id=0;id<MaxReport;id++){
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


