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
    list<<tr("索引")<<tr("名称")<<tr("数据源名称")<<tr("数据源索引")<<tr("使能")<<tr("写使能")
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
                  new QStandardItem(dataSetInfor[index].sourceIndex)<<
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
    dataSetInfor[index].sourceIndex=ui->dataSource->currentData().toString();
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
    dataSetInfor[index].sourceIndex="";
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
    ui->dataSource->setCurrentIndex(ui->dataSource->findData(dataSetInfor[index].sourceIndex.toInt()));
    ui->enable2->setChecked(dataSetInfor[index].enable);
    ui->writeEnable->setCheckable(dataSetInfor[index].writeEnable);
    ui->desc2->setText(dataSetInfor[index].desc);
    ui->namespaceIndex->setText(dataSetInfor[index].uaNode.namespaceIndex);
    ui->identifierType->setCurrentIndex(ui->identifierType->findText(dataSetInfor[index].uaNode.identifierType));
    ui->identifier->setText(dataSetInfor[index].uaNode.identifier);
    ui->dataType->setCurrentIndex(ui->dataType->findText(dataSetInfor[index].uaNode.dataType));
   }
    else{   //如果已经有配置信息，则填充回表单
    ui->name2->setText(dataSetInfor[index].name);
    ui->dataSource->setCurrentIndex(ui->dataSource->findData(dataSetInfor[index].sourceIndex.toInt()));
    ui->enable2->setChecked(dataSetInfor[index].enable);
    ui->writeEnable->setCheckable(dataSetInfor[index].writeEnable);
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
    ushort Flibhndl = 0;
    short ret = cnc_allclibhndl3(ui->address->text().toUtf8().data(), 8193, 10, &Flibhndl);
    if (ret != EW_OK)
    {
        QMessageBox::warning(this,tr("提示"),tr("连接服务器失败"),QMessageBox::Yes);
        return;
    }

//    #region cnc_machine

//    Focas1.ODBAXIS odbaxis = new Focas1.ODBAXIS();
//    for (short i = 0; i < 3; i++)
//    {
//        ret = Focas1.cnc_machine(Flibhndl, (short)(i + 1), 8, odbaxis);
//        Console.WriteLine(odbaxis.data[0]*Math.Pow(10,-4));
//    }
//    #endregion
//    Console.Write("read values");
//    Console.Read();
//}
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
        json.insert("sourceIndex",dataSetInfor[index].sourceIndex);
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
        dataSetInfor[index].sourceIndex=json.value("sourceIndex").toString();
        dataSetInfor[index].enable=json.value("enable").toBool();
        dataSetInfor[index].writeEnable=json.value("writeEnable").toBool();
        dataSetInfor[index].desc=json.value("desc").toString();
        dataSetInfor[index].uaNode.namespaceIndex=json.value("uaNode_namespaceIndex").toString();
        dataSetInfor[index].uaNode.identifierType=json.value("uaNode_identifierType").toString();
        dataSetInfor[index].uaNode.identifier=json.value("uaNode_identifier").toString();
        dataSetInfor[index].uaNode.dataType=json.value("uaNode_dataType").toString();
   }
}
