#include "configdialog.h"
#include "ui_configdialog.h"
#include <iostream>
#include "QDesktopWidget"
#include "InfluxDBFactory.h"
#include "Point.h"
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
    connect(ui->clearDataBase,SIGNAL(clicked()),this,SLOT(clearDataBase()));
    connect(ui->saveDataBase,SIGNAL(clicked()),this,SLOT(setDataBase()));
    connect(ui->getDataTest,SIGNAL(clicked()),this,SLOT(saveValueTest()));
    connect(ui->clearDataTable,SIGNAL(clicked()),this,SLOT(clearDataTable()));
    connect(ui->saveDataTable,SIGNAL(clicked()),this,SLOT(setDataTable()));
    connect(ui->index1,SIGNAL(valueChanged(int)),this,SLOT(fillDataBaseForm()));
    connect(ui->index2,SIGNAL(valueChanged(int)),this,SLOT(fillDataTableForm()));
    connect(ui->dataBase,SIGNAL(currentIndexChanged(int)),this,SLOT(fillDataTableBox()));
    connect(ui->name2,SIGNAL(currentIndexChanged(int)),this,SLOT(showRules()));
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
    dataBaseModel=new QStandardItemModel(this);  //建立数据库显示model实例
    dataTableModel=new QStandardItemModel(this);     //建立数据表显示model实例
    ui->tableView1->setModel(dataBaseModel);
    ui->tableView2->setModel(dataTableModel);
    ui->tableView2->setItemDelegateForColumn(1,m_delegate);
    ui->tableView2->setItemDelegateForColumn(2,m_delegate);
    ui->index1->setMaximum(MaxDataBase-1);
    ui->index2->setMaximum(MaxDataTable-1);
     showDataBase();        //显示数据库信息
     fillDataBaseForm();    //填充数据库表单
     fillDataBaseBox();     //填充数据库下拉框
     fillDataTableBox();    //填充数据表下拉框
     fillDataTableForm();   //填充数据表表单
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

void ConfigDialog::receiveMsgFromContainerManage(ResponseMetaData_dialog response)
{
    if(response.type=="getDrivesInfor"){
        driveInfor=response.value;
    }
    if(response.type=="getDataSetInfor"){
        dateSetInfor=response.value;
    }
}

/*显示数据库信息*/
void ConfigDialog::showDataBase()
{
    dataBaseModel->clear();
    QStringList list;
    list<<tr("名称")<<tr("使能")<<tr("说明")<<tr("用户名")
        <<tr("密码")<<tr("地址")<<tr("端口号");
    dataBaseModel->setHorizontalHeaderLabels(list);
    for(int index=0;index<MaxDataBase;index++  ){
       QList<QStandardItem *>  list;
       QString enable="True";
       if(dataBaseInfor[index].enable==false){
          enable="False";
       }
       list<<new QStandardItem(dataBaseInfor[index].name)<<
             new QStandardItem(enable)<<
             new QStandardItem(dataBaseInfor[index].desc)<<
             new QStandardItem(dataBaseInfor[index].username)<<
             new QStandardItem(dataBaseInfor[index].password)<<
             new QStandardItem(dataBaseInfor[index].address)<<
             new QStandardItem(dataBaseInfor[index].port);
        dataBaseModel->appendRow(list);
    }
}
/*显示匹配规则*/
void ConfigDialog::showRules()
{
       QString dataTable=ui->name2->currentText();
       /*如果当前索引下配置的数据表为数据下拉框当前项 且 当前索引下的匹配规则存在*/
       if(dataTableInfor[ui->index2->value()].name==dataTable and dataTableInfor[ui->index2->value()].rules!=""){
           dataTableModel->clear();
           QStringList list;
           list<<tr("字段名")<<tr("驱动")<<tr("数据集")<<tr("数据集索引");
           dataTableModel->setHorizontalHeaderLabels(list);
           QJsonDocument document = QJsonDocument::fromJson(dataTableInfor[ui->index2->value()].rules.toUtf8());
           QJsonArray array= document.array();
           for (int i = 0; i < array.count(); i++)
            {
               QJsonObject value = array.at(i).toObject();
               QList<QStandardItem *>  list;
               list<<new QStandardItem(value["field"].toString())<<
                     new QStandardItem(value["drive"].toString())<<
                     new QStandardItem(value["dataName"].toString())<<
                     new QStandardItem(value["dataIndex"].toString());
               dataTableModel->appendRow(list);
            }
       }
       else{
           /*连接服务器，获取指定信息*/
           int database=ui->dataBase->currentData().toInt();
           QSqlDatabase db=QSqlDatabase::database(dataBaseInfor[database].name);
           if(!db.isValid()){
               db.close();
               QSqlDatabase::removeDatabase(dataBaseInfor[database].name);
               db= QSqlDatabase::addDatabase("QMYSQL",dataBaseInfor[database].name);
               db.setHostName(dataBaseInfor[database].address);
               db.setPort(dataBaseInfor[database].port.toInt());
               db.setDatabaseName(dataBaseInfor[database].name);
               db.setUserName(dataBaseInfor[database].username);
               db.setPassword(dataBaseInfor[database].password);
               if(!db.open())
               {
                  db.close();
                  QSqlDatabase::removeDatabase(dataBaseInfor[database].name);
                  QMessageBox::warning(this,tr("提示"),tr("连接数据库失败!!!"),QMessageBox::Yes);
                  return;
               }
           }
           dataTableModel->clear();
           QStringList list;
           list<<tr("字段名")<<tr("驱动")<<tr("数据集")<<tr("数据集索引");
           dataTableModel->setHorizontalHeaderLabels(list);
           QString cmd = "describe "+ui->name2->currentText()+" ;";
           QSqlQuery query(cmd,db);
           while (query.next()) {
               QList<QStandardItem *>  list;
               list<<new QStandardItem(query.value(0).toString())<<
                     new QStandardItem("")<<
                     new QStandardItem("")<<
                     new QStandardItem("");
               dataTableModel->appendRow(list);
           }
       }
}
/*设置数据库信息*/
void ConfigDialog::setDataBase()
{
    int index=ui->index1->value();
    if(dataBaseInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号数据库已有配置信息，是否更新??"),QMessageBox::Yes,QMessageBox::No);
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
    dataBaseInfor[index].name=ui->name1->text();
    dataBaseInfor[index].enable=ui->enable1->isChecked();
    dataBaseInfor[index].desc=ui->desc1->text();
    dataBaseInfor[index].username=ui->username->text();
    dataBaseInfor[index].password=ui->password->text();
    dataBaseInfor[index].address=ui->address->text();
    dataBaseInfor[index].port=ui->port->text();
    QMessageBox::information(this,tr("提示"),tr("设置数据库成功"),QMessageBox::Yes);
    saveConfig();  //保存配置信息
    showDataBase();
    fillDataBaseBox();
}
/*设置数据表信息*/
void ConfigDialog::setDataTable()
{
    int index=ui->index2->value();
    if(dataTableInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号数据表已有配置信息，是否更新??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    QJsonArray array;
    for(int index=0;index<dataTableModel->rowCount();index++){
            QJsonObject json;
            json.insert("field",dataTableModel->data(dataTableModel->index(index,0)).toString());
            json.insert("drive",dataTableModel->data(dataTableModel->index(index,1)).toString());
            json.insert("dataName",dataTableModel->data(dataTableModel->index(index,2)).toString());
            json.insert("dataIndex",dataTableModel->data(dataTableModel->index(index,3)).toString());
            array.push_back(json);
    }
    QJsonDocument document;
    document.setArray(array);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    dataTableInfor[index].dataBase=ui->dataBase->currentData().toString();
    dataTableInfor[index].name=ui->name2->currentText();
    dataTableInfor[index].enable=ui->enable2->isChecked();
    dataTableInfor[index].frequency=QString::number(ui->frequency->value(),10);
    dataTableInfor[index].desc=ui->desc2->text();
    dataTableInfor[index].rules=json_str;
    QMessageBox::information(this,tr("提示"),tr("设置数据表成功"),QMessageBox::Yes);
    saveConfig();  //保存配置信息
}

void ConfigDialog::clearDataBase()
{

}

void ConfigDialog::clearDataTable()
{

}
/*填充数据库表单*/
void ConfigDialog::fillDataBaseForm()
{
    int index=ui->index1->value();
    if(dataBaseInfor[index].name!=""){      //如果已经有配置信息，则填充回表单
        ui->name1->setText(dataBaseInfor[index].name);
        ui->enable1->setChecked(dataBaseInfor[index].enable);
        ui->desc1->setText(dataBaseInfor[index].desc);
        ui->username->setText(dataBaseInfor[index].username);
        ui->password->setText(dataBaseInfor[index].password);
        ui->address->setText(dataBaseInfor[index].address);
        ui->port->setText(dataBaseInfor[index].port);
       }
}
/*填充数据表表单*/
void ConfigDialog::fillDataTableForm()
{
    int index=ui->index2->value();
    if(dataTableInfor[index].name!=""){      //如果已经有配置信息，则填充回表单
        ui->dataBase->setCurrentIndex(ui->dataBase->findData(dataTableInfor[index].dataBase));
        ui->name2->setCurrentText(dataTableInfor[index].name);
        ui->enable2->setChecked(dataTableInfor[index].enable);
        ui->frequency->setValue(dataTableInfor[index].frequency.toInt());
        ui->desc2->setText(dataTableInfor[index].desc);
        showRules();
       }
}
/*填充数据库下拉框*/
void ConfigDialog::fillDataBaseBox()
{
    ui->dataBase->clear();
    for(int index=0;index<MaxDataBase;index++)
    {
      if(dataBaseInfor[index].name!="")
      {
          ui->dataBase->addItem(dataBaseInfor[index].name,index);
      }
    }
}
/*填充数据表下拉框*/
void ConfigDialog::fillDataTableBox()
{
    ui->name2->clear();
    /*连接服务器，获取指定信息*/
    int index=ui->dataBase->currentData().toInt();
    QSqlDatabase db=QSqlDatabase::database(dataBaseInfor[index].name);
    if(!db.isValid()){
        db.close();
        QSqlDatabase::removeDatabase(dataBaseInfor[index].name);
        db= QSqlDatabase::addDatabase("QMYSQL",dataBaseInfor[index].name);
        db.setHostName(dataBaseInfor[index].address);
        db.setPort(dataBaseInfor[index].port.toInt());
        db.setDatabaseName(dataBaseInfor[index].name);
        db.setUserName(dataBaseInfor[index].username);
        db.setPassword(dataBaseInfor[index].password);
        if(!db.open())
        {
           db.close();
           QSqlDatabase::removeDatabase(dataBaseInfor[index].name);
           QMessageBox::warning(this,tr("提示"),tr("连接数据库失败!!!"),QMessageBox::Yes);
           return;
        }
    }
    QString cmd = "show tables;";
    QSqlQuery query(cmd,db);
    while (query.next()) {
        ui->name2->addItem(query.value(0).toString());
    }
}
/*连接数据库测试*/
void ConfigDialog::connectTest()
{
    if(ui->name1->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("名称不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->address->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("地址不能为空!!!"),QMessageBox::Yes);
      return;
       }
    QString url="http://"+ui->address->text()+":"+ui->port->text()+"/?db=";
    try {
         auto influxdb = influxdb::InfluxDBFactory::Get(url.toStdString());
         try {
             auto points = influxdb->query("show databases");
             for(int i=0;i<points.size();i++){
                 if(",name="+ui->name1->text()==points.at(i).getTags().data())
                 {
                   QMessageBox::information(this,tr("提示"),tr("连接数据库成功!!!"),QMessageBox::Yes);
                   return;
                 }
             }
             QMessageBox::warning(this,tr("提示"),tr("连接服务器成功,但数据库不存在!!!"),QMessageBox::Yes);
         } catch (...) {
              QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
         }
    } catch (...) {
         QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
    }

//    influxdb->write(influxdb::Point{ "cpu" }
//        .addTag("host", "localhost")
//        .addTag("region", "china")
//        .addField("north_latitude", 12)
//        .addField("east_longitude", 34)
//    );
//    influxdb->write(influxdb::Point{ "cpu" }
//        .addTag("host", "localhost")
//        .addTag("region", "china")
//        .addField("north_latitude", 67)
//        .addField("east_longitude", 89)
//        .addField("north_latitude1", 1011)
//        .addField("east_longitude1", 1213)
//    );
//    auto points = influxdb->query("SELECT * FROM cpu");
//    qDebug()<< "name : "<<points.back().getFields().data();
//    }
}
void ConfigDialog::saveValueTest()
{
    QSqlDatabase db=QSqlDatabase::database(ui->dataBase->currentText());
//    model=new QSqlTableModel(this,db);
//    model->setTable(ui->name2->currentText());
//    model->select();
//    int rowNum=model->rowCount();
//    model->insertRow(rowNum);
//    QJsonDocument document = QJsonDocument::fromJson(dataTableInfor[ui->index2->value()].rules.toUtf8());
//    QJsonArray array= document.array();
//    for (int i = 1; i < array.count(); i++)
//     {
//        QJsonObject value = array.at(i).toObject();
//        if(value["dataIndex"].toString()!=""){
//            RequestMetaData data;
//            data.type=getValue;
//            data.drive=value["drive"].toString();
//            data.index=value["dataIndex"].toString();
//            emit sendMsgToManager(data);
//            while(getValueResult==""){
//            }
////            model->setData(model->index(rowNum,i),getValueResult);
//            getValueResult="";
//         }
//     }
//        if(SaveDataToDB()==false)
//        {
//         model->revertAll();
//        }
//        else{
//         delete  model;
//        }
 }
/*保存信息*/
void ConfigDialog::saveConfig()
{
    QJsonArray dataBaseArray;
    for(int index=0;index<MaxDataBase;index++){
            QJsonObject json;
            json.insert("name",dataBaseInfor[index].name);
            json.insert("enable",dataBaseInfor[index].enable);
            json.insert("desc",dataBaseInfor[index].desc);
            json.insert("username",dataBaseInfor[index].username);
            json.insert("desc",dataBaseInfor[index].desc);
            json.insert("password",dataBaseInfor[index].password);
            json.insert("address",dataBaseInfor[index].address);
            json.insert("port",dataBaseInfor[index].port);
            dataBaseArray.push_back(json);
    }
    QJsonArray dataTableArray;
    for(int index=0;index<MaxDataTable;index++){
            QJsonObject json;
            json.insert("name",dataTableInfor[index].name);
            json.insert("dataBase",dataTableInfor[index].dataBase);
            json.insert("enable",dataTableInfor[index].enable);
            json.insert("frequency",dataTableInfor[index].frequency);
            json.insert("desc",dataTableInfor[index].desc);
            json.insert("rules",dataTableInfor[index].rules);
            dataTableArray.push_back(json);
    }
    QJsonObject object;
    object.insert("dataBase",dataBaseArray);
    object.insert("dataTable",dataTableArray);
    QJsonDocument document;
    document.setObject(object);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/influxdb.ini";
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
