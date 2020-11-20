#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();  //可用区域
    this->resize(deskRect.width()/4,deskRect.height()/1.2);
    loadConfig();                            //加载配置信息
    request=new httpRequest(this);
    connect(ui->clearServerInfor,SIGNAL(clicked()),this,SLOT(clearServerInfor()));
    connect(ui->saveServerInfor,SIGNAL(clicked()),this,SLOT(setServerInfor()));
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
    emit SendMsgToContainerManage(request);
    fillServerInforForm();    //填充服务器表单
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
        getValueResult=response.value;
    }
}

/*设置服务器信息*/
void ConfigDialog::setServerInfor()
{
    if(serverInfor.name!=""){
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
    serverInfor.name=ui->name1->text();
    serverInfor.enable=ui->enable1->isChecked();
    serverInfor.desc=ui->desc1->text();
    serverInfor.username=ui->username->text();
    serverInfor.password=ui->password->text();
    serverInfor.address=ui->address->text();
    QMessageBox::information(this,tr("提示"),tr("设置服务器成功"),QMessageBox::Yes);
    saveConfig();     //保存配置信息
}
/*清除服务器信息*/
void ConfigDialog::clearServerInfor()
{
    if(serverInfor.name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号服务器已有配置信息，是否清除??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    serverInfor.name="";
    serverInfor.enable=true;
    serverInfor.desc="";
    serverInfor.address="";
    serverInfor.username="";
    serverInfor.password="";
    ui->name1->setText("");
    ui->desc1->setText("");
    ui->address->setText("");
    ui->username->setText("");
    ui->password->setText("");
    QMessageBox::information(this,tr("提示"),tr("清除服务器信息成功"),QMessageBox::Yes);
    saveConfig();     //保存配置信息
}
/*填充服务器表单*/
void ConfigDialog::fillServerInforForm()
{
    if(serverInfor.name!=""){      //如果已经有配置信息，则填充回表单
        ui->name1->setText(serverInfor.name);
        ui->enable1->setChecked(serverInfor.enable);
        ui->desc1->setText(serverInfor.desc);
        ui->username->setText(serverInfor.username);
        ui->password->setText(serverInfor.password);
        ui->address->setText(serverInfor.address);
       }
    else{
        ui->name1->setText(serverInfor.name);
        ui->enable1->setChecked(serverInfor.enable);
        ui->desc1->setText(serverInfor.desc);
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

    httpRequest  *myrequest=new httpRequest(this);
    QString result=myrequest->login(ui->address->text(),ui->username->text(),ui->password->text());
    if(result=="err"){
      QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
    }
    else {
      QMessageBox::information(this,tr("提示"),tr("连接服务器成功!!!"),QMessageBox::Yes);
    }
    delete myrequest;

}
/*保存数据测试*/
void ConfigDialog::saveValueTest()
{
}

/*保存信息*/
void ConfigDialog::saveConfig()
{
    QJsonObject server;
    QJsonObject record;
    server.insert("name",serverInfor.name);
    server.insert("enable",QString::number(serverInfor.enable));
    server.insert("desc",serverInfor.desc);
    server.insert("username",serverInfor.username);
    server.insert("desc",serverInfor.desc);
    server.insert("password",serverInfor.password);
    server.insert("address",serverInfor.address);
    QJsonObject object;
    object.insert("server",server);
    object.insert("record",record);
    QJsonDocument document;
    document.setObject(object);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/control3.ini";
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
    QString fileName=path.path()+"/plugins/config/control3.ini";
    QFile file(fileName);
   if (!file.open(QFile::ReadOnly)) {   //如果文件不存在则新建文件
       file.open( QIODevice::ReadWrite | QIODevice::Text );
      }
    QByteArray data=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(data);
    QJsonObject server=doc.object().value("server").toObject();
    QJsonObject record=doc.object().value("record").toObject();
    serverInfor.name=server.value("name").toString();
    serverInfor.enable=server.value("enable").toBool();
    serverInfor.desc=server.value("desc").toString();
    serverInfor.username=server.value("username").toString();
    serverInfor.password=server.value("password").toString();
    serverInfor.address=server.value("address").toString();
}
