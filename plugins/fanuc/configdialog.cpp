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
    constructList();
    connect(ui->index1,SIGNAL(valueChanged(int)),this,SLOT(fillDataSourceForm()));
    connect(ui->index2,SIGNAL(valueChanged(int)),this,SLOT(fillDataSetForm()));
    connect(ui->clearDataSource,SIGNAL(clicked()),this,SLOT(clearDataSource()));
    connect(ui->saveDataSource,SIGNAL(clicked()),this,SLOT(setDataSource()));
    connect(ui->getDataTest,SIGNAL(clicked()),this,SLOT(getValueTest()));
    connect(ui->clearDataSet,SIGNAL(clicked()),this,SLOT(clearDataSet()));
    connect(ui->saveDataSet,SIGNAL(clicked()),this,SLOT(setDataSet()));
    connect(ui->connectTest,SIGNAL(clicked()),this,SLOT(connectTest()));
    connect(ui->group,SIGNAL(currentTextChanged(QString)),this,SLOT(fillFunctionBox()));
}
void ConfigDialog::constructList()
{    
    functionList.insert("控制轴主轴相关","读取主轴名称;读取受控主轴号;读取主轴进给率;读取主轴转速;读取主轴速度;读取主轴齿轮比;读取主轴负载;读取主轴负载表;读取主轴最大rpm率;读取主轴报警;读取轴名称;"
                                  "读取轴相对位置;读取轴绝对位置;读取轴机器位置;读取轴待走移动量;读取轴避让位置;读取轴位置信息;读取伺服负载表;读取轴伺服延时值;读取轴加减速延时值;"
                                  "读取点动或空运行进给率;读取轴的回路增益;读取轴的实际电流;读取轴的实际速度;读取轴状态信息;读取恒定表面速度;读取手轮中断的输入与输出值");
    functionList.insert("程序相关","读取程序号;读取程序名称");
    functionList.insert("文件数据相关","name3");
    functionList.insert("刀具寿命管理数据相关","读取刀具寿命管理元数据;读取最大刀具组数;读取刀具组数;读取刀具组号;读取使用的刀具组号;读取刀具组信息;读取刀具组寿命;读取刀具组升降计数;读取组内最大刀具数量;"
                                     "读取刀具数量;读取组内使用的刀具号;读取刀具数据;读取刀具长度;读取刀具补偿;读取刀具信息;读取刀具编号;读取组内所有数据");
    functionList.insert("操作历史数据相关","读取操作历史数据;读取报警历史记录数据;读取信号相关的操作历史;读取操作员历史记录数据");
    functionList.insert("其他","读取CNC系统信息;读取CNC系统硬件信息;读取CNC状态信息;读取操作员消息;读取模态数据;读取诊断数据;读取警报消息;读取警报状态;清除CNC警报;清除报警;获取详细错误信息;"
                         "读取CNC参数的信息;读取CNC参数的最大值最小值和总数;读取CNC配置信息;读取CNC设定数据的信息;读取CNC设置数据的最大最小和总数;读取CNC诊断数据的信息;读取CNC诊断数据的最大最小和总数;"
                         "读取DMG Netservice状态信息;读取G代码;读取程序重新启动信息;读取命令值;获得最大有效数字和小数位数;获取屏幕状态;更改屏幕模式;读取AD转换数据;读取块状态;;");
}



ConfigDialog::~ConfigDialog()
{  
     delete ui;
}

/*窗体显示事件*/
void ConfigDialog::showEvent(QShowEvent *)
{
    dataSourceModel=new QStandardItemModel(this);  //建立数据源显示model实例
    dataSetModel=new QStandardItemModel(this);     //建立数据集显示model实例
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
        <<tr("主机")<<tr("端口号")<<tr("超时时间")<<tr("重复次数");
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
                 new QStandardItem(dataSourceInfor[index].host)<<
                 new QStandardItem(dataSourceInfor[index].port)<<
                 new QStandardItem(dataSourceInfor[index].timeout)<<
                 new QStandardItem(dataSourceInfor[index].repeat);
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
        <<tr("说明")<<tr("功能组")<<tr("功能")<<tr("参数1")<<tr("参数2")<<tr("参数3")<<tr("参数4");
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
                    new QStandardItem(dataSetInfor[index].group)<<
                    new QStandardItem(dataSetInfor[index].function)<<
                    new QStandardItem(dataSetInfor[index].parameter1)<<
                    new QStandardItem(dataSetInfor[index].parameter2)<<
                    new QStandardItem(dataSetInfor[index].parameter3)<<
                    new QStandardItem(dataSetInfor[index].parameter4);

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
    if(ui->host->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("主机不能为空!!!"),QMessageBox::Yes);
      return;
    }
    if(ui->port->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("端口号不能为空!!!"),QMessageBox::Yes);
      return;
    }
    dataSourceInfor[index].name=ui->name1->text();
    dataSourceInfor[index].enable=ui->enable1->isChecked();
    dataSourceInfor[index].desc=ui->desc1->text();
    dataSourceInfor[index].host=ui->host->text();
    dataSourceInfor[index].port=ui->port->text();
    dataSourceInfor[index].timeout=QString::number(ui->timeout->value());
    dataSourceInfor[index].repeat=QString::number(ui->repeat->value());
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
    dataSetInfor[index].name=ui->name2->text();
    dataSetInfor[index].sourceName=ui->dataSource->currentText();
    dataSetInfor[index].sourceId=ui->dataSource->currentData().toString();
    dataSetInfor[index].enable=ui->enable2->isChecked();
    dataSetInfor[index].writeEnable=ui->writeEnable->isChecked();
    dataSetInfor[index].desc=ui->desc2->text();
    dataSetInfor[index].group=ui->group->currentText();
    dataSetInfor[index].function=ui->function->currentText();
    dataSetInfor[index].parameter1=ui->parameter1->text();
    dataSetInfor[index].parameter2=ui->parameter2->text();
    dataSetInfor[index].parameter3=ui->parameter3->text();
    dataSetInfor[index].parameter4=ui->parameter4->text();
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
    dataSourceInfor[index].host="";
    dataSourceInfor[index].port="";
    dataSourceInfor[index].timeout="";
    dataSourceInfor[index].repeat="";
    ui->name1->setText("");
    ui->desc1->setText("");
    ui->host->setText("");
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
    dataSetInfor[index].group="";
    dataSetInfor[index].function="";
    dataSetInfor[index].parameter1="";
    dataSetInfor[index].parameter2="";
    dataSetInfor[index].parameter3="";
    dataSetInfor[index].parameter4="";
    ui->name2->setText("");
    ui->desc2->setText("");
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
        ui->host->setText(dataSourceInfor[index].host);
        ui->port->setText(dataSourceInfor[index].port);
        ui->timeout->setValue(dataSourceInfor[index].timeout.toInt());
        ui->repeat->setValue(dataSourceInfor[index].repeat.toInt());
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
    ui->group->setCurrentText(dataSetInfor[index].group);
    fillFunctionBox();
    ui->function->setCurrentText(dataSetInfor[index].function);
    ui->parameter1->setText(dataSetInfor[index].parameter1);
    ui->parameter2->setText(dataSetInfor[index].parameter2);
    ui->parameter3->setText(dataSetInfor[index].parameter3);
    ui->parameter4->setText(dataSetInfor[index].parameter4);
   }
    else{   //如果已经有配置信息，则填充回表单
    ui->name2->setText(dataSetInfor[index].name);
    ui->dataSource->setCurrentIndex(ui->dataSource->findData(dataSetInfor[index].sourceId.toInt()));
    ui->enable2->setChecked(dataSetInfor[index].enable);
    ui->writeEnable->setChecked(dataSetInfor[index].writeEnable);
    ui->desc2->setText(dataSetInfor[index].desc);
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
          ui->dataSource->addItem(dataSourceInfor[index].name,id);
      }
    }
}
/*填充功能下拉框*/
void ConfigDialog::fillFunctionBox()
{
    QStringList list;
    QString qStr=functionList.value(ui->group->currentText());
    list=qStr.split(";");
    ui->function->clear();
    for(int i=0;i<list.count();i++){
        ui->function->addItem(list.at(i));
    }
}
/*连接到服务器测试*/
void ConfigDialog::connectTest()
{
    ushort Flibhndl = 0;
    short ret = cnc_allclibhndl3(ui->host->text().toUtf8().data(), ui->port->text().toUInt(), ui->timeout->value(), &Flibhndl);
    cnc_freelibhndl(Flibhndl);
    if (ret != EW_OK)
    {
        QMessageBox::warning(this,tr("提示"),tr("连接机器失败"),QMessageBox::Yes);
    }
    else {
        QMessageBox::information(this,tr("提示"),tr("连接机器成功"),QMessageBox::Yes);
    }
}
/*获取数值测试*/
void ConfigDialog::getValueTest()
{
    if(ui->dataSource->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("数据源不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->function->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("功能选择不能为空!!!"),QMessageBox::Yes);
      return;
       }
    ushort Flibhndl = 0;
    short ret = cnc_allclibhndl3(ui->host->text().toUtf8().data(),ui->port->text().toUInt(), ui->timeout->value(), &Flibhndl);
//    if (ret != EW_OK)
//    {
//        QMessageBox::warning(this,tr("提示"),tr("连接机器失败"),QMessageBox::Yes);
//        return;
//    }
    QString result;
     QJsonDocument document;
     QJsonObject object;
    DATASET dataSetInfor;
    dataSetInfor.flibhndl=QString::number(Flibhndl);
    dataSetInfor.function=ui->function->currentText();
    dataSetInfor.parameter1=ui->parameter1->text();
    dataSetInfor.parameter2=ui->parameter2->text();
    dataSetInfor.parameter3=ui->parameter3->text();
    dataSetInfor.parameter4=ui->parameter4->text();
    if(ui->group->currentText()=="控制轴主轴相关"){
        axisInfor m_axiInfor=axisInfor();
        result=m_axiInfor.getValue(dataSetInfor);
        QMessageBox::information(this,tr("提示"),tr("读取结果为")+result,QMessageBox::Yes);
        return;
    }
    if(ui->group->currentText()=="程序相关"){
        programInfor m_programInfor=programInfor();
        result=m_programInfor.getValue(dataSetInfor);
        QMessageBox::information(this,tr("提示"),tr("读取结果为")+result,QMessageBox::Yes);
        return;
    }
    if(ui->group->currentText()=="NC文件数据相关"){
    }
    if(ui->group->currentText()=="刀具寿命管理数据相关"){
        toolLifeManagement m_toolLifeManagement=toolLifeManagement();
        result=m_toolLifeManagement.getValue(dataSetInfor);
        QMessageBox::information(this,tr("提示"),tr("读取结果为")+result,QMessageBox::Yes);
        return;
    }
    if(ui->group->currentText()=="操作历史数据相关"){
        operationHistroyData m_operationHistroyData=operationHistroyData();
        result=m_operationHistroyData.getValue(dataSetInfor);
        QMessageBox::information(this,tr("提示"),tr("读取结果为")+result,QMessageBox::Yes);
        return;
    }
    if(ui->group->currentText()=="其他"){
        otherInfor m_otherInfor=otherInfor();
        result=m_otherInfor.getValue(dataSetInfor);
        QMessageBox::information(this,tr("提示"),tr("读取结果为")+result,QMessageBox::Yes);
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
            json.insert("host",dataSourceInfor[index].host);
            json.insert("port",dataSourceInfor[index].port);
            json.insert("timeout",dataSourceInfor[index].timeout);
            json.insert("repeat",dataSourceInfor[index].repeat);
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
        json.insert("group",dataSetInfor[index].group);
        json.insert("function",dataSetInfor[index].function);
        json.insert("parameter1",dataSetInfor[index].parameter1);
        json.insert("parameter2",dataSetInfor[index].parameter2);
        json.insert("parameter3",dataSetInfor[index].parameter3);
        json.insert("parameter4",dataSetInfor[index].parameter4);
        dataSetArray.push_back(json);
    }
    QJsonObject object;
    object.insert("dataSource",dataSourceArray);
    object.insert("dataSet",dataSetArray);
    QJsonDocument document;
    document.setObject(object);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/fanuc.ini";
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
    QString fileName=path.path()+"/plugins/config/fanuc.ini";
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
        dataSourceInfor[index].timeout=json.value("timeout").toString();
        dataSourceInfor[index].repeat=json.value("repeat").toString();
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
        dataSetInfor[index].group=json.value("group").toString();
        dataSetInfor[index].function=json.value("function").toString();
        dataSetInfor[index].parameter1=json.value("parameter1").toString();
        dataSetInfor[index].parameter2=json.value("parameter2").toString();
        dataSetInfor[index].parameter3=json.value("parameter3").toString();
        dataSetInfor[index].parameter4=json.value("parameter4").toString();
   }
}


