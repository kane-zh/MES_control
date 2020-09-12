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
    connect(ui->group,SIGNAL(currentIndexChanged(int)),this,SLOT(fillFunctionBox()));
}
void ConfigDialog::constructList()
{
    functionList.insert("控制轴主轴相关","读取主轴进给率;读取主轴转速;读取恒定表面速度;读取轴负载;读取伺服负载表;读取轴负载表;读取轴最大rpm率;读取轴转速;读取轴速度;"
                        "读取轴齿轮比;读取轴相对位置;读取轴绝对位置;读取轴机器位置;读取轴剩余移动量;读取轴避让位置;读取轴伺服延时值;读取轴加减速延时值;读取显示的点动或空运行进给率;"
                        "读取轴刀具位置信息;读取手动重叠运动值;显示手轮中断的输入与输出值;读取用于5轴加工的手动进给的机床轴移动量;读取轴加工汇总数据;读取轴所有动态数据;读取轴所有数据");
    functionList.insert("程序相关","name2");
    functionList.insert("NC文件数据相关","name3");
    functionList.insert("刀具寿命管理数据相关","name4");
    functionList.insert("刀具管理数据相关","name5");
    functionList.insert("操作历史数据相关","name6");
    functionList.insert("3D干涉检查","name7");
    functionList.insert("故障诊断","name8");
    functionList.insert("其他","name9");
    functionList.insert("图形指令数据","name10");
    functionList.insert("伺服学习数据","name11");
    functionList.insert("NC显示功能","name12");
    functionList.insert("远程诊断功能","name13");
    functionList.insert("双重检查安全功能","name14");
    functionList.insert("连续位置数据输出功能","name15");
    functionList.insert("程序重新启动","name16");
    functionList.insert("辅助状态","name17");
    functionList.insert("安全IO信号历史","name18");
    functionList.insert("机器状态监控器_记录器","name19");
    functionList.insert("环保模式","name20");
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
        <<tr("主机")<<tr("端口号")<<tr("超时时间");
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
                 new QStandardItem(dataSourceInfor[index].timeout);
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
                    new QStandardItem(dataSetInfor[index].sourceIndex)<<
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
    dataSetInfor[index].sourceIndex=ui->dataSource->currentData().toString();
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
    dataSetInfor[index].sourceIndex="";
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
    ui->group->setCurrentText(dataSetInfor[index].group);
    ui->function->setCurrentText(dataSetInfor[index].function);
    ui->parameter1->setText(dataSetInfor[index].parameter1);
    ui->parameter2->setText(dataSetInfor[index].parameter2);
    ui->parameter3->setText(dataSetInfor[index].parameter3);
    ui->parameter4->setText(dataSetInfor[index].parameter4);
   }
    else{   //如果已经有配置信息，则填充回表单
    ui->name2->setText(dataSetInfor[index].name);
    ui->dataSource->setCurrentIndex(ui->dataSource->findData(dataSetInfor[index].sourceIndex.toInt()));
    ui->enable2->setChecked(dataSetInfor[index].enable);
    ui->writeEnable->setCheckable(dataSetInfor[index].writeEnable);
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
          ui->dataSource->addItem(dataSourceInfor[index].name,index);
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
    }
    if(ui->group->currentText()=="NC文件数据相关"){
    }
    if(ui->group->currentText()=="刀具寿命管理数据相关"){
    }
    if(ui->group->currentText()=="刀具管理数据相关"){
    }
    if(ui->group->currentText()=="操作历史数据相关"){
    }
    if(ui->group->currentText()=="3D干涉检查"){
    }
    if(ui->group->currentText()=="故障诊断"){
    }
    if(ui->group->currentText()=="其他"){
    }
    if(ui->group->currentText()=="图形指令数据"){
    }
    if(ui->group->currentText()=="伺服学习数据"){
    }
    if(ui->group->currentText()=="NC显示功能"){
    }
    if(ui->group->currentText()=="远程诊断功能"){
    }
    if(ui->group->currentText()=="双重检查安全功能"){
    }
    if(ui->group->currentText()=="连续位置数据输出功能"){
    }
    if(ui->group->currentText()=="程序重新启动"){
    }
    if(ui->group->currentText()=="辅助状态"){
    }
    if(ui->group->currentText()=="安全IO信号历史"){
    }
    if(ui->group->currentText()=="机器状态监控器_记录器"){
    }
    if(ui->group->currentText()=="环保模式"){
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
        dataSetInfor[index].group=json.value("group").toString();
        dataSetInfor[index].function=json.value("function").toString();
        dataSetInfor[index].parameter1=json.value("parameter1").toString();
        dataSetInfor[index].parameter2=json.value("parameter2").toString();
        dataSetInfor[index].parameter3=json.value("parameter3").toString();
        dataSetInfor[index].parameter4=json.value("parameter4").toString();
   }
}

