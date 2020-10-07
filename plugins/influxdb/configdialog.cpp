#include "configdialog.h"
#include "ui_configdialog.h"
#include <iostream>
#include "QDesktopWidget"
#include "configdialog.h"
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
    connect(ui->name2,SIGNAL(currentTextChanged(QString)),this,SLOT(showRules()));
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
     createActions();
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
        dataTableInfor[ui->index2->text().toInt()].getValueResult=response.value;
    }
}
void ConfigDialog::contextMenuEvent(QContextMenuEvent *e)
{
    menu->exec (QCursor::pos ());
}
void ConfigDialog::createActions()
{
    dataTableModel->clear();
    QStringList list;
    list<<tr("字段名")<<tr("驱动")<<tr("数据集")<<tr("数据集索引");
    dataTableModel->setHorizontalHeaderLabels(list);
    dataTableModel->insertRow(0);
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
    dataTableModel->removeRow(ui->tableView2->currentIndex().row());
}

void ConfigDialog::AddRow()
{
    dataTableModel->insertRow(ui->tableView2->currentIndex().row()+1);
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
       dataTableModel->clear();
       QStringList list;
       list<<tr("字段名")<<tr("驱动")<<tr("数据集")<<tr("数据集索引");
       dataTableModel->setHorizontalHeaderLabels(list);
       /*如果当前索引下配置的数据表为数据下拉框当前项 且 当前索引下的匹配规则存在*/
       if(dataTableInfor[ui->index2->value()].name==dataTable and dataTableInfor[ui->index2->value()].rules!=""){
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
              dataTableModel->insertRow(0);
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
    int index=ui->index1->value();
    if(dataBaseInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号数据库已有配置信息，是否清除??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    dataBaseInfor[index].name="";
    dataBaseInfor[index].enable=true;
    dataBaseInfor[index].desc="";
    dataBaseInfor[index].address="";
    dataBaseInfor[index].port="";
    dataBaseInfor[index].username="";
    dataBaseInfor[index].password="";
    ui->name1->setText("");
    ui->desc1->setText("");
    ui->address->setText("");
    ui->port->setText("");
    ui->username->setText("");
    ui->password->setText("");
    QMessageBox::information(this,tr("提示"),tr("清除数据库信息成功"),QMessageBox::Yes);
    saveConfig();
    showDataBase();
    fillDataBaseBox();
}

void ConfigDialog::clearDataTable()
{
    int index=ui->index2->value();
    if(dataBaseInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号数据表已有配置信息，是否清除??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    dataTableInfor[index].dataBase="";
    dataTableInfor[index].name="";
    dataTableInfor[index].enable=false;
    dataTableInfor[index].frequency="";
    dataTableInfor[index].desc="";
    dataTableInfor[index].rules="";
    ui->dataBase->setCurrentIndex(0);
    ui->name2->setCurrentText("");
    ui->enable2->setCheckState(Qt::Unchecked);
    ui->frequency->setValue(0);
    ui->desc2->setText("");
    dataTableModel->clear();
    QStringList list;
    list<<tr("字段名")<<tr("驱动")<<tr("数据集")<<tr("数据集索引");
    dataTableModel->setHorizontalHeaderLabels(list);
    dataTableModel->insertRow(0);
    QMessageBox::information(this,tr("提示"),tr("清除数据库信息成功"),QMessageBox::Yes);
    saveConfig();
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
    else{
        ui->name1->setText(dataBaseInfor[index].name);
        ui->enable1->setChecked(dataBaseInfor[index].enable);
        ui->desc1->setText(dataBaseInfor[index].desc);
       }
}
/*填充数据表表单*/
void ConfigDialog::fillDataTableForm()
{
    int index=ui->index2->value();
    ui->dataBase->setCurrentIndex(ui->dataBase->findData(dataTableInfor[index].dataBase));
    ui->name2->setCurrentText(dataTableInfor[index].name);
    ui->enable2->setChecked(dataTableInfor[index].enable);
    ui->frequency->setValue(dataTableInfor[index].frequency.toInt());
    ui->desc2->setText(dataTableInfor[index].desc);
    showRules();
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
    httpRequest  *myrequest=new httpRequest(this);
    QString result=myrequest->get(ui->address->text(),ui->port->text(),ui->username->text(),ui->password->text(),ui->name1->text(),"show databases");
    if(result=="err"){
      QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
      return;
    }
    QJsonDocument document=QJsonDocument::fromJson(result.toLocal8Bit().data());
    QJsonObject object=document.object().value("results")[0].toObject().value("series")[0].toObject();
    QJsonArray  values=object.value("values").toArray();
     for(int i=0;i<values.count();i++){
         if(ui->name1->text()==values.at(i)[0].toString())
         {
           QMessageBox::information(this,tr("提示"),tr("连接数据库成功!!!"),QMessageBox::Yes);
           return;
         }
     }
     QMessageBox::warning(this,tr("提示"),tr("连接服务器成功,但数据库不存在!!!"),QMessageBox::Yes);
}
void ConfigDialog::saveValueTest()
{
    httpRequest  *myrequest=new httpRequest(this);
    int serverIndex=ui->dataBase->currentData().toInt();
    QString  value1=ui->name2->currentText()+",index="+QString::number(ui->index2->value())+" ";
    QString  value2="";
    for (int i = 0; i < dataTableModel->rowCount(); i++)
     {
         if(dataTableModel->data(dataTableModel->index(i,2)).toString()!=""){
             RequestMetaData_dialog request;
             request.type="getValue";
             request.drive=dataTableModel->data(dataTableModel->index(i,1)).toString();
             request.index=dataTableModel->data(dataTableModel->index(i,3)).toString();
             emit SendMsgToContainerManage(request);
             while(dataTableInfor[ui->index2->text().toInt()].getValueResult==""){
                  QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
             }
             QJsonDocument document=QJsonDocument::fromJson(dataTableInfor[ui->index2->text().toInt()].getValueResult.toUtf8());
             dataTableInfor[ui->index2->text().toInt()].getValueResult="";
             QJsonObject object=document.object();
              if(document.object().value("result").toString()=="err"){
              QMessageBox::warning(this,tr("提示"),tr("获取数据")+dataTableModel->data(dataTableModel->index(i,0)).toString()+"失败："
                                   +document.object().value("value").toString(),QMessageBox::Yes);
             }
             else{
                value2.append(dataTableModel->data(dataTableModel->index(i,2)).toString()+"=");
                value2.append("\"");
                value2.append(document.object().value("value").toString());
                value2.append("\"");
                value2.append(",");
             }
           }
       }
        if(value2!=""){
             value2.remove(value2.length()-1,2);
             QString result=myrequest->post(ui->address->text(),ui->port->text(),ui->username->text(),ui->password->text(),ui->name1->text(),value1+value2);
             if(result=="err"){
               QMessageBox::warning(this,tr("提示"),tr("写数据失败!!!"),QMessageBox::Yes);
               return;
             }
             else{
               QMessageBox::warning(this,tr("提示"),tr("写数据成功!!!"),QMessageBox::Yes);
             }
        }
        else{
            QMessageBox::warning(this,tr("提示"),tr("未成功获取到有效可写数据!!!"),QMessageBox::Yes);
        }

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

