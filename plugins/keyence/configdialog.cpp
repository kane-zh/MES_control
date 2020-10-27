#include "configdialog.h"
#include "ui_configdialog.h"
#include <QTime>
#include "QDesktopWidget"
ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();  //可用区域
    this->resize(deskRect.width()/2,deskRect.height()/1.2);
    loadConfig();        //加载配置信息
    connect(ui->index1,SIGNAL(valueChanged(int)),this,SLOT(fillDataSourceForm()));
    connect(ui->index2,SIGNAL(valueChanged(int)),this,SLOT(fillDataSetForm()));
    connect(ui->clearDataSource,SIGNAL(clicked()),this,SLOT(clearDataSource()));
    connect(ui->saveDataSource,SIGNAL(clicked()),this,SLOT(setDataSource()));
    connect(ui->clearDataSet,SIGNAL(clicked()),this,SLOT(clearDataSet()));
    connect(ui->saveDataSet,SIGNAL(clicked()),this,SLOT(setDataSet()));
    connect(ui->getDataTest,SIGNAL(clicked()),this,SLOT(getValueTest()));
    connect(ui->setDataTest,SIGNAL(clicked()),this,SLOT(setValueTest()));
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
    fillDataSourceBox();

}
/*准备读数据信号*/
void ConfigDialog::onReadReady()
{
    auto reply = qobject_cast<QTcpSocket *>(sender());
    for (int i=0;i<MaxDataSet;i++) {
       if(reply==dataSetInfor[i].socket)
       {
        dataSetInfor[i].value=reply->readAll();
        dataSetInfor[i].socket->abort();
        dataSetInfor[i].socket=nullptr;
        reply->deleteLater();
        return;
       }
    }
}
/*通讯故障信号*/
void ConfigDialog::onError()
{
    auto reply = qobject_cast<QTcpSocket *>(sender());
        for (int i=0;i<MaxDataSet;i++) {
           if(reply==dataSetInfor[i].socket)
           {
            dataSetInfor[i].socket->abort();
            dataSetInfor[i].socket=nullptr;
            reply->deleteLater();
            return;
           }
    }
}

/*显示数据源内容*/
void ConfigDialog::showDataSource()
{
    dataSourceModel->clear();
    QStringList list;
    list<<tr("索引")<<tr("名称")<<tr("使能")<<tr("说明")<<tr("主机")<<tr("端口号");
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
                 new QStandardItem(dataSourceInfor[index].port);
            dataSourceModel->appendRow(list);
         }
    }
}
/*显示数据集内容*/
void ConfigDialog::showDataSet()
{
    dataSetModel->clear();
    QStringList list;
    list<<tr("索引")<<tr("名称")<<tr("数据源名称")<<tr("数据源索引")<<tr("使能")<<tr("写使能")<<tr("说明")
         <<tr("软元件类型")<<("软元件编号")<<("超时时间");
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
                    new QStandardItem(dataSetInfor[index].type)<<
                    new QStandardItem(dataSetInfor[index].number)<<
                    new QStandardItem(dataSetInfor[index].timeout);
            dataSetModel->appendRow(list);
        }
    }
}
/*设置数据源*/
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
      QMessageBox::warning(this,tr("提示"),tr("主机名能为空!!!"),QMessageBox::Yes);
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
    QMessageBox::information(this,tr("提示"),tr("设置数据源成功"),QMessageBox::Yes);
    saveConfig();  //保存配置信息
    showDataSource();  //显示数据源信息(更新显示)
    fillDataSourceBox();//填充数据源下拉框
}
/*设置数据集*/
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
    if(ui->number->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("软元编号不能为空!!!"),QMessageBox::Yes);
      return;
    }
    dataSetInfor[index].name=ui->name2->text();
    dataSetInfor[index].sourceName=ui->dataSource->currentText();
    dataSetInfor[index].sourceIndex=ui->dataSource->currentData().toString();
    dataSetInfor[index].enable=ui->enable2->isChecked();
    dataSetInfor[index].writeEnable=ui->writeEnable->isChecked();
    dataSetInfor[index].desc=ui->desc2->text();
    dataSetInfor[index].type=ui->type->currentText();
    dataSetInfor[index].number=QString("%1").arg(ui->number->text().toInt(), 5, 10, QLatin1Char('0'));
    dataSetInfor[index].timeout=QString::number(ui->timeout->value());
    QMessageBox::information(this,tr("提示"),tr("设置数据集成功"),QMessageBox::Yes);
    saveConfig();  //保存配置信息
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
    ui->name1->setText("");
    ui->desc1->setText("");
    ui->host->setText("");
    ui->port->setText("");
    QMessageBox::information(this,tr("提示"),tr("清除数据源信息成功"),QMessageBox::Yes);
    saveConfig();  //保存配置信息
    showDataSource();  //显示数据源信息(更新显示)
    fillDataSourceBox();//填充数据源下拉框;
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
    dataSetInfor[index].type="";
    dataSetInfor[index].number="";
    dataSetInfor[index].timeout="";
    ui->name2->setText("");
    ui->desc2->setText("");
    ui->timeout->setValue(0);
    QMessageBox::information(this,tr("提示"),tr("清除数据集信息成功"),QMessageBox::Yes);
    saveConfig();  //保存配置信息
    showDataSet(); //显示数据集信息(更新显示)
}
/*填充数据源表单*/
void ConfigDialog::fillDataSourceForm()
{
    int index=ui->index1->value();
    if(dataSourceInfor[index].name!=""){      //如果已经有配置信息，则填充回表单
        ui->name1->setText(dataSourceInfor[index].name);
        ui->enable1->setChecked(dataSourceInfor[index].enable);
        ui->desc1->setText(dataSourceInfor[index].desc);
        ui->host->setText(dataSourceInfor[index].host);
        ui->port->setText(dataSourceInfor[index].port);
       }
    else{
        ui->name1->setText(dataSourceInfor[index].name);
        ui->enable1->setChecked(dataSourceInfor[index].enable);
        ui->desc1->setText(dataSourceInfor[index].desc);
    }
}
/*填充数据集表单*/
void ConfigDialog::fillDataSetForm()
{
    int index=ui->index2->value();
    if(dataSetInfor[index].name!=""){
    ui->name2->setText(dataSetInfor[index].name);
    ui->dataSource->setCurrentIndex(ui->dataSource->findData(dataSetInfor[index].sourceIndex.toInt()));
    ui->enable2->setChecked(dataSetInfor[index].enable);
    ui->writeEnable->setChecked(dataSetInfor[index].writeEnable);
    ui->desc2->setText(dataSetInfor[index].desc);
    ui->type->setCurrentText(dataSetInfor[index].type);
    ui->number->setText(dataSetInfor[index].number);
    ui->timeout->setValue(dataSetInfor[index].timeout.toInt());
    }
    else{
    ui->name2->setText(dataSetInfor[index].name);
    ui->dataSource->setCurrentIndex(ui->dataSource->findData(dataSetInfor[index].sourceIndex.toInt()));
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
          ui->dataSource->addItem(dataSourceInfor[index].name,index);
      }
    }
}
/*连接到服务器测试*/
void ConfigDialog::connectTest()
{
    if(ui->host->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("服务器地址不能为空!!!"),QMessageBox::Yes);
      return;
      }
    if(ui->port->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("端口号不能为空!!!"),QMessageBox::Yes);
      return;
      }
    QTcpSocket *socket= new QTcpSocket();
    //取消已有的连接
    socket->abort();
    //连接服务器
    socket->connectToHost(ui->host->text(),ui->port->text().toInt());
    //等待连接成功
    if(!socket->waitForConnected(3000))
    {
        QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
    }
    else{
        QMessageBox::information(this,tr("提示"),tr("连接服务器成功!!!"),QMessageBox::Yes);
    }
    socket->abort();
    delete  socket;
}
/*获取值测试*/
void ConfigDialog::getValueTest(){
    if(ui->dataSource->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("数据源不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->number->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("软元件编号不能为空!!!"),QMessageBox::Yes);
      return;
       }
    int  dataSource=ui->dataSource->currentData().toInt();
    int  dataSet=ui->index2->value();
     /*判断数据源连接状态(如果已经连接)*/
    if(dataSetInfor[dataSet].socket!=nullptr){
        if(dataSetInfor[dataSet].socket->state()!=QAbstractSocket::ConnectedState ){
            dataSetInfor[dataSet].socket->abort();
            dataSetInfor[dataSet].socket=nullptr;
        }
    }
    /*判断数据源是否连接*/
    if(dataSetInfor[dataSet].socket==nullptr){
        /*连接指定的服务器*/
        QTcpSocket  *socket = new QTcpSocket(this);
        if (socket){
           socket->connectToHost(dataSourceInfor[dataSetInfor[dataSet].sourceIndex.toInt()].host,
                   dataSourceInfor[dataSetInfor[dataSet].sourceIndex.toInt()].port.toInt());
           //等待连接成功
           if(!socket->waitForConnected(3000))
           {
               socket->abort();
               delete  socket;
               QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
               return;
           }
            else{
                 dataSetInfor[dataSet].socket=socket;
                 connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
                 connect(socket, SIGNAL(disconnected()), this, SLOT(onError()));
                 connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError()));
            }
        }
        else{
         QMessageBox::warning(this,tr("提示"),tr("建立数据集的服务器对象失败!!"),QMessageBox::Yes);
         return ;
        }
    }
    QString RD = "RD "+ui->type->currentText()+ui->number->text()+"\r";
    dataSetInfor[dataSet].value="";
    dataSetInfor[dataSet].socket->write(RD.toUtf8().data());
    QTime dieTime = QTime::currentTime().addMSecs(ui->timeout->value());
    while (dataSetInfor[dataSet].value==""&&QTime::currentTime() < dieTime) {
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    if(dataSetInfor[dataSet].value==""&&QTime::currentTime() >=dieTime){
       QMessageBox::warning(this,tr("提示"),tr("读取节点值超时"),QMessageBox::Yes);
       return;
    }
     QMessageBox::warning(this,tr("提示"),tr("读取节点值成功，值为：")+dataSetInfor[dataSet].value,QMessageBox::Yes);

}
/*设置数值测试*/
void ConfigDialog::setValueTest()
{
    if(ui->dataSource->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("数据源不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->number->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("软元件编号不能为空!!!"),QMessageBox::Yes);
      return;
       }
    int  dataSource=ui->dataSource->currentData().toInt();
    int  dataSet=ui->index2->value();
     /*判断数据源连接状态(如果已经连接)*/
    if(dataSetInfor[dataSet].socket!=nullptr){
        if(dataSetInfor[dataSet].socket->state()!=QAbstractSocket::ConnectedState){
            dataSetInfor[dataSet].socket->abort();
            dataSetInfor[dataSet].socket=nullptr;
        }
    }
    /*判断数据源是否连接*/
    if(dataSetInfor[dataSet].socket==nullptr){
        /*连接指定的服务器*/
        QTcpSocket  *socket = new QTcpSocket(this);
        if (socket){
           socket->connectToHost(dataSourceInfor[dataSetInfor[dataSet].sourceIndex.toInt()].host,
                   dataSourceInfor[dataSetInfor[dataSet].sourceIndex.toInt()].port.toInt());
           //等待连接成功
           if(!socket->waitForConnected(3000))
           {
               socket->abort();
               delete  socket;
               QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
               return;
           }
            else{
                 dataSetInfor[dataSet].socket=socket;
                 connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
                 connect(socket, SIGNAL(disconnected()), this, SLOT(onError()));
                 connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError()));
            }
        }
        else{
         QMessageBox::warning(this,tr("提示"),tr("建立数据集的服务器对象失败!!"),QMessageBox::Yes);
         return ;
        }
    }
    QString WR = "WR "+ui->type->currentText()+ui->number->text()+" "+ui->value->text()+"\r";
    dataSetInfor[dataSet].value="";
    dataSetInfor[dataSet].socket->write(WR.toUtf8().data());
    QTime dieTime = QTime::currentTime().addMSecs(ui->timeout->value());
    while (dataSetInfor[dataSet].value==""&&QTime::currentTime() < dieTime) {
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    if(dataSetInfor[dataSet].value==""&&QTime::currentTime() >=dieTime){
       QMessageBox::warning(this,tr("提示"),tr("设置节点值超时"),QMessageBox::Yes);
       return;
    }
    QMessageBox::warning(this,tr("提示"),tr("设置节点值成功，结果为：")+dataSetInfor[dataSet].value,QMessageBox::Yes);
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
            dataSourceArray.push_back(json);
    }
    QJsonArray dataSetArray;
    for(int index=0;index<MaxDataSet;index++){
            QJsonObject json;
            json.insert("name",dataSetInfor[index].name);
            json.insert("sourceName",dataSetInfor[index].sourceName);
            json.insert("sourceIndex",dataSetInfor[index].sourceIndex);
            json.insert("enable",dataSetInfor[index].enable);
            json.insert("writeEnable",dataSetInfor[index].writeEnable);
            json.insert("desc",dataSetInfor[index].desc);
            json.insert("type",dataSetInfor[index].type);
            json.insert("number",dataSetInfor[index].number);
            json.insert("timeout",dataSetInfor[index].timeout);
            dataSetArray.push_back(json);
    }
    QJsonObject object;
    object.insert("dataSource",dataSourceArray);
    object.insert("dataSet",dataSetArray);
    QJsonDocument document;
    document.setObject(object);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/keyence.ini";
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
    QString fileName=path.path()+"/plugins/config/keyence.ini";
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
        dataSetInfor[index].type=json.value("type").toString();
        dataSetInfor[index].number=json.value("number").toString();
        dataSetInfor[index].timeout=json.value("timeout").toString();
   }
}
