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
    loadConfig();           //加载配置信息
    connect(ui->index1,SIGNAL(valueChanged(int)),this,SLOT(fillDataSourceForm()));
    connect(ui->index2,SIGNAL(valueChanged(int)),this,SLOT(fillDataSetForm()));
    connect(ui->clearDataSource,SIGNAL(clicked()),this,SLOT(clearDataSource()));
    connect(ui->saveDataSource,SIGNAL(clicked()),this,SLOT(setDataSource()));
    connect(ui->getDataTest,SIGNAL(clicked()),this,SLOT(getValueTest()));
    connect(ui->clearDataSet,SIGNAL(clicked()),this,SLOT(clearDataSet()));
    connect(ui->saveDataSet,SIGNAL(clicked()),this,SLOT(setDataSet()));
    connect(ui->connectTest,SIGNAL(clicked()),this,SLOT(connectTest()));

}

ConfigDialog::~ConfigDialog()
{  
    for(int index=0;index<MaxDataSource;index++  ){
       if(dataSourceInfor[index].uaInfor.client!=nullptr){
         UA_Client_delete(dataSourceInfor[index].uaInfor.client);
         }
    }
     delete ui;
}

/*窗体显示事件*/
void ConfigDialog::showEvent(QShowEvent *)
{
    dataSourceModel=new QStandardItemModel();  //建立数据源显示model实例
    dataSetModel=new QStandardItemModel();     //建立数据集显示model实例
    ui->tableView1->setModel(dataSourceModel);
    ui->tableView2->setModel(dataSetModel);
    ui->index1->setMaximum(MaxDataSource-1);
    ui->index2->setMaximum(MaxDataSet-1);
    showDataSource();    //显示数据源信息
    showDataSet();       //显示数据集信息
    fillDataSourceForm();//填充数据源表单
    fillDataSetForm();   //填充数据集表单
    fillDataSourceBox(); //填充数据源下拉框
}
/*数据解码，将OPCUA数据格式转换成字符串*/
QString ConfigDialog::dataDecoding(UA_Variant value)
{
     QString   result="";
     if(strcmp(value.type->typeName, "Boolean")==0){
         UA_Boolean   raw_date = *(UA_Boolean *) value.data;
         if(raw_date==false){
             result="False";
         }
         else{
             result="True";
         }
     }
     else if(strcmp(value.type->typeName, "Int16")==0){
         UA_Int16  raw_date = *(UA_Int16 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "UInt16")==0){
         UA_UInt16  raw_date = *(UA_UInt16 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "Int32")==0){
         UA_Int32  raw_date = *(UA_Int32 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "UInt32")==0){
         UA_UInt32  raw_date = *(UA_UInt32 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "Int64")==0){
         UA_Int64  raw_date = *(UA_Int64 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "UInt64")==0){
         UA_UInt64  raw_date = *(UA_UInt64 *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "Float")==0){
         UA_Float   raw_date = *(UA_Float *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "Double")==0){
         UA_Double   raw_date = *(UA_Double *) value.data;
         result=QString::number(raw_date);
     }
     else if(strcmp(value.type->typeName, "String")==0){
         UA_String   raw_date = *(UA_String *) value.data;
         UA_Byte *p;
         char data[raw_date.length+1];
         p=raw_date.data;
         for(int i=0;i<raw_date.length;i++)
         {
             data[i]=*p;
             p++;
         }
         data[raw_date.length]='\0';
         result=QString::fromLocal8Bit(data);
     }
     else if(strcmp(value.type->typeName, "DateTime")==0){
             UA_DateTime raw_date = *(UA_DateTime *) value.data;
             UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
             result.append(QString::number(dts.year));
             result.append("-");
             result.append(QString::number(dts.month));
             result.append("-");
             result.append(QString::number(dts.day));
             result.append("T");
             result.append(QString::number(dts.hour));
             result.append("-");
             result.append(QString::number(dts.min));
             result.append("-");
             result.append(QString::number(dts.sec));
             result.append("-");
             result.append(QString::number(dts.milliSec));
             result.append("-");
             result.append(QString::number(dts.microSec));
     }
     else{
          result="null";
         }
     return  result;
}
/*数据编码，将字符串转换成OPCUA数据格式*/
QString ConfigDialog::dataEncoding(QString value, QString DataType, UA_Variant *result)
{
    UA_Variant  newValue;
      UA_Variant_init(&newValue);
      if(strcmp(DataType.toLatin1(), "Boolean")==0){
            UA_Boolean  data;
           if(strcmp(value.toLatin1(), "False")==0){
              data=false;
          }else{
             data=true;
          }
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_BOOLEAN]);
      }
      else  if(strcmp(DataType.toLatin1(), "SByte")==0){
        UA_SByte data=value.toShort();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_SBYTE]);
      }
       else if(strcmp(DataType.toLatin1(), "Int16")==0){
        UA_Int16 data=value.toInt();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_INT16]);
      }
       else if(strcmp(DataType.toLatin1(), "UInt16")==0){
        UA_UInt16 data=value.toUInt();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_UINT16]);
      }
      else if(strcmp(DataType.toLatin1(), "Int32")==0){
        UA_Int32 data=value.toLong();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_INT32]);
      }
     else if(strcmp(DataType.toLatin1(), "UInt32")==0){
        UA_UInt32 data=value.toULong();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_UINT32]);
      }
     else if(strcmp(DataType.toLatin1(), "Int64")==0){
        UA_Int64 data=value.toLongLong();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_INT64]);
      }
     else if(strcmp(DataType.toLatin1(), "UInt64")==0){
        UA_UInt64 data=value.toULongLong();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_UINT64]);
      }
      else if(strcmp(DataType.toLatin1(), "Float")==0){
        UA_Float data=value.toFloat();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_FLOAT]);
      }
      else if(strcmp(DataType.toLatin1(), "Double")==0){
        UA_Double data=value.toDouble();
        UA_Variant_setScalar(&newValue, &data, &UA_TYPES[UA_TYPES_DOUBLE]);
      }
      else if(strcmp(DataType.toLatin1(), "String")==0){
//          UA_String  data=value.toStdString();
//          UA_Variant_setScalar(&newValue, &value, &UA_TYPES[UA_TYPES_STRING]);
      }
      else if(strcmp(DataType.toLatin1(), "DateTime")==0){
      }
      else{

      }
       UA_Variant_copy(&newValue,result);
}
/*显示数据源信息*/
void ConfigDialog::showDataSource()
{
    dataSourceModel->clear();
    QStringList list;
    list<<tr("索引")<<tr("名称")<<tr("使能") <<tr("说明")
        <<tr("用户名")<<tr("密码")<<tr("地址");
    dataSourceModel->setHorizontalHeaderLabels(list);
    for(int index=0;index<MaxDataSource;index++  ){
        if(dataSourceInfor[index].name!=""){
           QList<QStandardItem *>  list;
           QString enable="True";
           if(dataSourceInfor[index].enable==false){
              enable="False";
           }
           list<<new QStandardItem(QString::number(index))<<
                 new QStandardItem(dataSourceInfor[index].name)<<
                 new QStandardItem(enable)<<
                 new QStandardItem(dataSourceInfor[index].desc)<<
                 new QStandardItem(dataSourceInfor[index].uaInfor.username)<<
                 new QStandardItem(dataSourceInfor[index].uaInfor.password)<<
                 new QStandardItem(dataSourceInfor[index].uaInfor.address);
            dataSourceModel->appendRow(list);
        }
    }
}
/*显示数据集信息*/
void ConfigDialog::showDataSet()
{
    dataSetModel->clear();
    QStringList list;
    list<<tr("索引")<<tr("名称")<<tr("数据源名称")<<tr("数据源标识")<<tr("使能")<<tr("写使能")
        <<tr("说明")<<tr("命名空间")<<tr("节点类型")<<tr("标识")<<tr("数据类型");
    dataSetModel->setHorizontalHeaderLabels(list);
    for(int index=0;index<MaxDataSet;index++){
        if(dataSetInfor[index].name!=""){
            QList<QStandardItem *>  list;
            QString enable="True";
            if(dataSetInfor[index].enable==false){
               enable="False";
            }
            QString writeEnable="False";
            if(dataSetInfor[index].writeEnable==true){
               writeEnable="True";
            }
            list<<new QStandardItem(QString::number(index))<<
                  new QStandardItem(dataSetInfor[index].name)<<
                  new QStandardItem(dataSetInfor[index].sourceName)<<
                  new QStandardItem(dataSetInfor[index].sourceId)<<
                  new QStandardItem(enable)<<
                  new QStandardItem(writeEnable)<<
                  new QStandardItem(dataSetInfor[index].desc)<<
                  new QStandardItem(dataSetInfor[index].uaNode.namespaceIndex)<<
                  new QStandardItem(dataSetInfor[index].uaNode.identifierType)<<
                  new QStandardItem(dataSetInfor[index].uaNode.identifier)<<
                  new QStandardItem(dataSetInfor[index].uaNode.dataType);
            dataSetModel->appendRow(list);
        }
    }
}
/*设置数据源信息*/
void ConfigDialog::setDataSource()
{
    int index=ui->index1->value();
    if(dataSourceInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号数据源已有配置信息，是否更新??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    if(ui->name1->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("名称不能为空!!!"),QMessageBox::Yes);
      return;
    }
    if(ui->address->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("地址不能为空!!!"),QMessageBox::Yes);
      return;
    }
    dataSourceInfor[index].name=ui->name1->text();
    dataSourceInfor[index].enable=ui->enable1->isChecked();
    dataSourceInfor[index].desc=ui->desc1->text();
    dataSourceInfor[index].uaInfor.username=ui->username->text();
    dataSourceInfor[index].uaInfor.password=ui->password->text();
    dataSourceInfor[index].uaInfor.address=ui->address->text();
    QMessageBox::information(this,tr("提示"),tr("设置数据源成功"),QMessageBox::Yes);
    saveConfig();      //保存配置信息
    showDataSource();  //显示数据源信息(更新显示)
    fillDataSourceBox();//填充数据源下拉框
}
/*设置数据集信息*/
void ConfigDialog::setDataSet()
{
    int index=ui->index2->value();
    if(dataSetInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号数据集已有配置信息，是否更新??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    if(ui->name2->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("名称不能为空!!!"),QMessageBox::Yes);
      return;
    }
    if(ui->dataSource->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("数据源不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->namespaceIndex->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("节点命名空间不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->identifier->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("节点标识不能为空!!!"),QMessageBox::Yes);
      return;
       }
    dataSetInfor[index].name=ui->name2->text();
    dataSetInfor[index].sourceName=ui->dataSource->currentText();
    dataSetInfor[index].sourceId=ui->dataSource->currentData().toString();
    dataSetInfor[index].enable=ui->enable2->isChecked();
    dataSetInfor[index].writeEnable=ui->writeEnable->isChecked();
    dataSetInfor[index].desc=ui->desc2->text();
    dataSetInfor[index].uaNode.namespaceIndex=ui->namespaceIndex->text();
    dataSetInfor[index].uaNode.identifierType=ui->identifierType->currentText();
    dataSetInfor[index].uaNode.identifier=ui->identifier->text();
    dataSetInfor[index].uaNode.dataType=ui->dataType->currentText();
    QMessageBox::information(this,tr("提示"),tr("设置数据集成功"),QMessageBox::Yes);
    saveConfig();      //保存配置信息
    showDataSet(); //显示数据集信息(更新显示)
}
/*清除数据源内容*/
void ConfigDialog::clearDataSource()
{
    int index=ui->index1->value();
    if(dataSourceInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号数据源已有配置信息，是否清除??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    dataSourceInfor[index].name="";
    dataSourceInfor[index].enable=true;
    dataSourceInfor[index].desc="";
    dataSourceInfor[index].uaInfor.username="";
    dataSourceInfor[index].uaInfor.password="";
    dataSourceInfor[index].uaInfor.address="";
    ui->name1->setText("");
    ui->desc1->setText("");
    ui->username->setText("");
    ui->password->setText("");
    ui->address->setText("");
    QMessageBox::information(this,tr("提示"),tr("清除数据源信息成功"),QMessageBox::Yes);
    saveConfig();      //保存配置信息
    showDataSource();  //显示数据源信息(更新显示)
    fillDataSourceBox();//填充数据源下拉框
}
/*清除数据集内容*/
void ConfigDialog::clearDataSet()
{
    int index=ui->index2->value();
    if(dataSetInfor[index].name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前编号数据集已有配置信息，是否清除??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    dataSetInfor[index].name="";
    dataSetInfor[index].sourceName="";
    dataSetInfor[index].sourceId="";
    dataSetInfor[index].enable=true;
    dataSetInfor[index].writeEnable=false;
    dataSetInfor[index].desc="";
    dataSetInfor[index].uaNode.namespaceIndex="";
    dataSetInfor[index].uaNode.identifierType="";
    dataSetInfor[index].uaNode.identifier="";
    dataSetInfor[index].uaNode.dataType="";
    ui->name2->setText("");
    ui->desc2->setText("");
    ui->namespaceIndex->setText("");
    ui->identifier->setText("");
    QMessageBox::information(this,tr("提示"),tr("清除数据集信息成功"),QMessageBox::Yes);
    saveConfig(); //保存配置信息
    showDataSet(); //显示数据集信息(更新显示)
}
/*填充数据源表单(槽)*/
void ConfigDialog::fillDataSourceForm()
{
    int index=ui->index1->value();
    if(dataSourceInfor[index].name!=""){      //如果已经有配置信息，则填充回表单
        ui->name1->setText(dataSourceInfor[index].name);
        ui->enable1->setChecked(dataSourceInfor[index].enable);
        ui->desc1->setText(dataSourceInfor[index].desc);
        ui->username->setText(dataSourceInfor[index].uaInfor.username);
        ui->password->setText(dataSourceInfor[index].uaInfor.password);
        ui->address->setText(dataSourceInfor[index].uaInfor.address);
       }
    else{
        ui->name1->setText(dataSourceInfor[index].name);
        ui->enable1->setChecked(dataSourceInfor[index].enable);
        ui->desc1->setText(dataSourceInfor[index].desc);
       }
}
/*填充数据集表单(槽)*/
void ConfigDialog::fillDataSetForm()
{
    int index=ui->index2->value();
    if(dataSetInfor[index].name!=""){   //如果已经有配置信息，则填充回表单
    ui->name2->setText(dataSetInfor[index].name);
    ui->dataSource->setCurrentIndex(ui->dataSource->findData(dataSetInfor[index].sourceId.toInt()));
    ui->enable2->setChecked(dataSetInfor[index].enable);
    ui->writeEnable->setChecked(dataSetInfor[index].writeEnable);
    ui->desc2->setText(dataSetInfor[index].desc);
    ui->namespaceIndex->setText(dataSetInfor[index].uaNode.namespaceIndex);
    ui->identifierType->setCurrentIndex(ui->identifierType->findText(dataSetInfor[index].uaNode.identifierType));
    ui->identifier->setText(dataSetInfor[index].uaNode.identifier);
    ui->dataType->setCurrentIndex(ui->dataType->findText(dataSetInfor[index].uaNode.dataType));
   }
    else{   //如果已经有配置信息，则填充回表单
    ui->name2->setText(dataSetInfor[index].name);
    ui->dataSource->setCurrentIndex(ui->dataSource->findData(dataSetInfor[index].sourceId.toInt()));
    ui->enable2->setChecked(dataSetInfor[index].enable);
    ui->writeEnable->setChecked(dataSetInfor[index].writeEnable);
    ui->desc2->setText(dataSetInfor[index].desc);
    ui->identifierType->setCurrentIndex(ui->identifierType->findText(dataSetInfor[index].uaNode.identifierType));
    ui->dataType->setCurrentIndex(ui->dataType->findText(dataSetInfor[index].uaNode.dataType));
   }
}
/*填充数据源下拉框*/
void ConfigDialog::fillDataSourceBox()
{
    ui->dataSource->clear();
    for(int index=0;index<MaxDataSource;index++)
    {
      if(dataSourceInfor[index].name!="")
      {
          ui->dataSource->addItem(dataSourceInfor[index].name,index);
      }
    }
}
/*连接到服务器测试*/
void ConfigDialog::connectTest()
{
    if(ui->address->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("请输入服务器地址"),QMessageBox::Yes);
      return;
    }
    /*连接指定的服务器*/
    UA_Client *client =UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    UA_StatusCode retval=UA_Client_connect(client,ui->address->text().toLatin1());
    UA_Client_delete(client);
    if(retval != UA_STATUSCODE_GOOD) {
     QMessageBox::warning(this,tr("提示"),tr("服务器连接失败"),QMessageBox::Yes);
    }
    else {
     QMessageBox::information(this,tr("提示"),tr("服务器连接成功"),QMessageBox::Yes);
    }
}
/*获取数值测试*/
void ConfigDialog::getValueTest()
{
    if(ui->dataSource->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("数据源不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->namespaceIndex->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("节点命名空间不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->identifier->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("节点标识不能为空!!!"),QMessageBox::Yes);
      return;
       }
    UA_Client  *client=UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    QString address=dataSourceInfor[ui->dataSource->currentData().toInt()].uaInfor.address;
    UA_StatusCode  retval=UA_Client_connect(client,address.toLatin1());
    if (retval!=UA_STATUSCODE_GOOD) {
        UA_Client_delete (client);
        QMessageBox::warning(this,tr("提示"),tr("服务器连接失败"),QMessageBox::Yes);
        return;
    }
    UA_Variant value;
    UA_Variant_init(&value);
    QString identifierType=ui->identifierType->currentText();
    QString namespaceIndex=ui->namespaceIndex->text();
    QString identifier=ui->identifier->text();
    const UA_NodeId nodeId = UA_NODEID_STRING(namespaceIndex.toInt(),identifier.toLocal8Bit().data());
    retval = UA_Client_readValueAttribute(client, nodeId, &value);
    /*根据数据集节点类型，构建不同的节点信息*/
    if(identifierType=="String"){
       const UA_NodeId nodeId = UA_NODEID_STRING(namespaceIndex.toInt(),identifier.toLocal8Bit().data());
       retval = UA_Client_readValueAttribute(client, nodeId, &value);
     }
    else if(identifierType=="Numeric"){
       const UA_NodeId nodeId =UA_NODEID_NUMERIC(namespaceIndex.toInt(),identifier.toLong());
       retval = UA_Client_readValueAttribute(client, nodeId, &value);
     }
    else if(identifierType=="ByteString"){
       const UA_NodeId nodeId =UA_NODEID_BYTESTRING(namespaceIndex.toInt(),identifier.toLocal8Bit().data());
       retval = UA_Client_readValueAttribute(client, nodeId, &value);
     }
    else if(identifierType=="String_Alloc"){
       const UA_NodeId nodeId =UA_NODEID_STRING_ALLOC(namespaceIndex.toInt(),identifier.toLocal8Bit().data());
       retval = UA_Client_readValueAttribute(client, nodeId, &value);
     }
    else{
      QMessageBox::warning(this,tr("提示"),tr("不支持的节点类型"),QMessageBox::Yes);
      return;
     }
    if(retval != UA_STATUSCODE_GOOD) {
       UA_Client_delete(client);
       QMessageBox::warning(this,tr("提示"),tr("读取节点数值失败"),QMessageBox::Yes);
       return;
    }
    QString result = dataDecoding(value);
    UA_Variant_clear(&value);
    UA_Client_delete(client);
    QMessageBox::information(this,tr("提示"),tr("读取节点数值成功,值为:")+result,QMessageBox::Yes);
}
/*保存信息*/
void ConfigDialog::saveConfig()
{
    QJsonArray dataSourceArray;
    for(int index=0;index<MaxDataSource;index++){
            QJsonObject json;
            json.insert("name",dataSourceInfor[index].name);
            json.insert("enable",dataSourceInfor[index].enable);
            json.insert("desc",dataSourceInfor[index].desc);
            json.insert("uaInfor_username",dataSourceInfor[index].uaInfor.username);
            json.insert("uaInfor_password",dataSourceInfor[index].uaInfor.password);
            json.insert("uaInfor_address",dataSourceInfor[index].uaInfor.address);
            dataSourceArray.push_back(json);
    }
    QJsonArray dataSetArray;
    for(int index=0;index<MaxDataSet;index++){
        QJsonObject json;
        json.insert("name",dataSetInfor[index].name);
        json.insert("sourceName",dataSetInfor[index].sourceName);
        json.insert("sourceId",dataSetInfor[index].sourceId);
        json.insert("enable",dataSetInfor[index].enable);
        json.insert("writeEnable",QString::number(dataSetInfor[index].writeEnable));
        json.insert("desc",dataSetInfor[index].desc);
        json.insert("uaNode_namespaceIndex",dataSetInfor[index].uaNode.namespaceIndex);
        json.insert("uaNode_identifierType",dataSetInfor[index].uaNode.identifierType);
        json.insert("uaNode_identifier",dataSetInfor[index].uaNode.identifier);
        json.insert("uaNode_dataType",dataSetInfor[index].uaNode.dataType);
        dataSetArray.push_back(json);
    }
    QJsonObject object;
    object.insert("dataSource",dataSourceArray);
    object.insert("dataSet",dataSetArray);
    QJsonDocument document;
    document.setObject(object);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/opcua.ini";
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
    QString fileName=path.path()+"/plugins/config/opcua.ini";
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
        dataSourceInfor[index].uaInfor.username=json.value("uaInfor_username").toString();
        dataSourceInfor[index].uaInfor.password=json.value("uaInfor_password").toString();
        dataSourceInfor[index].uaInfor.address=json.value("uaInfor_address").toString();
   }
   QJsonValue dataSet=object.value("dataSet");
   QJsonArray dataSetArray=dataSet.toArray();
   for(int index=0;index<MaxDataSet;index++){
        QJsonObject json=dataSetArray.at(index).toObject();
        dataSetInfor[index].name=json.value("name").toString();
        dataSetInfor[index].sourceName=json.value("sourceName").toString();
        dataSetInfor[index].sourceId=json.value("sourceId").toString();
        dataSetInfor[index].enable=json.value("enable").toBool();
        dataSetInfor[index].writeEnable=json.value("writeEnable").toBool();
        dataSetInfor[index].desc=json.value("desc").toString();
        dataSetInfor[index].uaNode.namespaceIndex=json.value("uaNode_namespaceIndex").toString();
        dataSetInfor[index].uaNode.identifierType=json.value("uaNode_identifierType").toString();
        dataSetInfor[index].uaNode.identifier=json.value("uaNode_identifier").toString();
        dataSetInfor[index].uaNode.dataType=json.value("uaNode_dataType").toString();
   }
}
