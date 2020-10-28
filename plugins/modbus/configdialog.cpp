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
    loadConfig();    //加载配置
    connect(ui->connectTest,SIGNAL(clicked()),this,SLOT(connectTest()));
    connect(ui->clearDataSource,SIGNAL(clicked()),this,SLOT(clearDataSource()));
    connect(ui->saveDataSource,SIGNAL(clicked()),this,SLOT(setDataSource()));
    connect(ui->getDataTest,SIGNAL(clicked()),this,SLOT(getValueTest()));
    connect(ui->clearDataSet,SIGNAL(clicked()),this,SLOT(clearDataSet()));
    connect(ui->saveDataSet,SIGNAL(clicked()),this,SLOT(setDataSet()));
    connect(ui->index1,SIGNAL(valueChanged(int)),this,SLOT(fillDataSourceForm()));
    connect(ui->index2,SIGNAL(valueChanged(int)),this,SLOT(fillDataSetForm()));
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
    fillInfo();          //填充下拉框
    showDataSource();    //显示数据源信息
    showDataSet();       //显示数据集信息
    fillDataSourceForm();//填充数据源表单
    fillDataSetForm();   //填充数据集表单
    fillDataSourceBox();

}
/*准备接收数据槽*/
void ConfigDialog::onReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit result = reply->result();
        for (int i=0;i<MaxDataSet;i++) {
           if(reply==dataSetInfor[i].readData.reply)
           {
               for (int j = 0;j<result.valueCount();j++) {
                   dataSetInfor[i].readData.value.append(QString::number(result.value(j)));
               }
              reply->deleteLater();
              return;
           }
        }
    }
    else{
        for (int i=0;i<MaxDataSet;i++) {
           if(reply==dataSetInfor[i].readData.reply)
           {

               dataSetInfor[i].readData.value="err";
              reply->deleteLater();
              return;
           }
        }
    }
}

/*填充下拉框内容*/
void ConfigDialog::fillInfo()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    QStringList  infoList;
    infoList << info.portName()
             << info.description()
             << info.manufacturer()
             << info.serialNumber()
             << info.systemLocation()
             << QString::number(info.vendorIdentifier(), 16);
        ui->com->addItem (infoList.first(), infoList);//填充下拉框
        }

    ui->baudRate->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
    ui->baudRate->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    ui->baudRate->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    ui->baudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRate->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->baudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    ui->parity->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    ui->parity->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    ui->parity->addItem(QStringLiteral("Odd"),  QSerialPort::OddParity);
    ui->parity->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
    ui->parity->addItem(QStringLiteral("Space"),QSerialPort::SpaceParity);

    ui->dataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBits->setCurrentIndex(3);

    ui->stopBits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBits->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->code_read->addItem(QStringLiteral("01"), QModbusDataUnit::Coils);
    ui->code_read->addItem(QStringLiteral("02"), QModbusDataUnit::DiscreteInputs);
    ui->code_read->addItem(QStringLiteral("03"), QModbusDataUnit::HoldingRegisters);
    ui->code_read->addItem(QStringLiteral("04"), QModbusDataUnit::InputRegisters);

    ui->code_write->addItem(QStringLiteral("0f"), QModbusDataUnit::Coils);
    ui->code_write->addItem(QStringLiteral("10"), QModbusDataUnit::HoldingRegisters);
}
/*显示数据源内容*/
void ConfigDialog::showDataSource()
{
    dataSourceModel->clear();
    QStringList list;
    list<<tr("索引")<<tr("名称")<<tr("使能")<<tr("说明")<<tr("端口号")<<tr("波特率")<<tr("校验方式")
        <<tr("数据位")<<tr("停止位")<<tr("请求时间")<<tr("重复次数");
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
                    new QStandardItem(dataSourceInfor[index].com)<<
                    new QStandardItem(dataSourceInfor[index].stringbaudRate)<<
                    new QStandardItem(dataSourceInfor[index].stringparity)<<
                    new QStandardItem(dataSourceInfor[index].stringdataBits)<<
                    new QStandardItem(dataSourceInfor[index].stringstopBits)<<
                    new QStandardItem(dataSourceInfor[index].responseTime)<<
                    new QStandardItem(dataSourceInfor[index].numberOfRetries);
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
        <<tr("读主机地址")<<tr("读功能码")<<tr("读起始地址")<<tr("读长度")
        <<tr("写主机地址")<<tr("写功能码")<<tr("写起始地址")<<tr("写长度");
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
                  new QStandardItem(dataSetInfor[index].readData.serverAddress)<<
                  new QStandardItem(dataSetInfor[index].readData.stringCode)<<
                  new QStandardItem(dataSetInfor[index].readData.startAddress)<<
                  new QStandardItem(dataSetInfor[index].readData.length)<<
                  new QStandardItem(dataSetInfor[index].writeData.serverAddress)<<
                  new QStandardItem(dataSetInfor[index].writeData.stringCode)<<
                  new QStandardItem(dataSetInfor[index].writeData.startAddress)<<
                  new QStandardItem(dataSetInfor[index].writeData.length);
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
    if(ui->com->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("端口号不能为空!!!"),QMessageBox::Yes);
      return;
    }
    dataSourceInfor[index].name=ui->name1->text();
    dataSourceInfor[index].enable=ui->enable1->isChecked();
    dataSourceInfor[index].desc=ui->desc1->text();
    dataSourceInfor[index].com=ui->com->currentText();
    dataSourceInfor[index].baudRate= static_cast<QSerialPort::BaudRate>(ui->baudRate->currentData().toInt());
    dataSourceInfor[index].stringbaudRate=ui->baudRate->currentText();
    dataSourceInfor[index].parity= static_cast<QSerialPort::Parity>(ui->parity->currentData().toInt());
    dataSourceInfor[index].stringparity=ui->parity->currentText();
    dataSourceInfor[index].dataBits= static_cast<QSerialPort::DataBits>(ui->dataBits->currentData().toInt());
    dataSourceInfor[index].stringdataBits=ui->dataBits->currentText();
    dataSourceInfor[index].stopBits= static_cast<QSerialPort::StopBits>(ui->stopBits->currentData().toInt());
    dataSourceInfor[index].stringstopBits=ui->stopBits->currentText();
    dataSourceInfor[index].responseTime=QString::number(ui->responseTime->value());
    dataSourceInfor[index].numberOfRetries=QString::number(ui->numberOfRetries->value());
    QMessageBox::information(this,tr("提示"),tr("设置数据源成功"),QMessageBox::Yes);
    saveConfig();       //保存配置
    showDataSource();   //显示数据源信息(更新显示)
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
    dataSetInfor[index].name=ui->name2->text();
    dataSetInfor[index].sourceName=ui->dataSource->currentText();
    dataSetInfor[index].sourceIndex=ui->dataSource->currentData().toString();
    dataSetInfor[index].enable=ui->enable2->isChecked();
    dataSetInfor[index].writeEnable=ui->writeEnable->isChecked();
    dataSetInfor[index].desc=ui->desc2->text();
    dataSetInfor[index].readData.serverAddress=ui->server_read->text();
    dataSetInfor[index].readData.code=static_cast<QModbusDataUnit::RegisterType>(ui->code_read->currentData().toInt());
    dataSetInfor[index].readData.stringCode=ui->code_read->currentText();
    dataSetInfor[index].readData.startAddress=ui->startAddr_read->text();
    dataSetInfor[index].readData.length=ui->length_read->text();
    dataSetInfor[index].writeData.serverAddress=ui->server_write->text();
    dataSetInfor[index].writeData.code=static_cast<QModbusDataUnit::RegisterType>(ui->code_write->currentData().toInt());
    dataSetInfor[index].writeData.stringCode=ui->code_write->currentText();
    dataSetInfor[index].writeData.startAddress=ui->startAddr_write->text();
    dataSetInfor[index].writeData.length=ui->length_write->text();
    QMessageBox::information(this,tr("提示"),tr("设置数据集成功"),QMessageBox::Yes);
    saveConfig();       //保存配置
    showDataSet();      //显示数据集信息(更新显示)
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
    dataSourceInfor[index].baudRate=QSerialPort::UnknownBaud;
    dataSourceInfor[index].stringbaudRate="";
    dataSourceInfor[index].parity=QSerialPort::UnknownParity;
    dataSourceInfor[index].stringparity="";
    dataSourceInfor[index].dataBits= QSerialPort::UnknownDataBits;
    dataSourceInfor[index].stringdataBits="";
    dataSourceInfor[index].stopBits= QSerialPort::UnknownStopBits;
    dataSourceInfor[index].stringstopBits="";
    dataSourceInfor[index].responseTime="";
    dataSourceInfor[index].numberOfRetries="";
    ui->name1->setText("");
    ui->desc1->setText("");
    ui->responseTime->setValue(0);
    ui->numberOfRetries->setValue(0);
    QMessageBox::information(this,tr("提示"),tr("清除数据源信息成功"),QMessageBox::Yes);
    saveConfig();       //保存配置
    showDataSource();   //显示数据源信息(更新显示)
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
    dataSetInfor[index].readData.serverAddress="";
    dataSetInfor[index].readData.code=QModbusDataUnit::QModbusDataUnit::Invalid;
    dataSetInfor[index].readData.stringCode="";
    dataSetInfor[index].readData.startAddress="";
    dataSetInfor[index].readData.length="";
    dataSetInfor[index].writeData.serverAddress="";
    dataSetInfor[index].writeData.code=QModbusDataUnit::QModbusDataUnit::Invalid;
    dataSetInfor[index].writeData.stringCode="";
    dataSetInfor[index].writeData.startAddress="";
    dataSetInfor[index].writeData.length="";
    ui->name2->setText("");
    ui->desc2->setText("");
    ui->server_read->setText("");
    ui->startAddr_read->setText("");
    ui->length_read->setText("");
    ui->server_write->setText("");
    ui->startAddr_write->setText("");
    ui->length_write->setText("");
    QMessageBox::information(this,tr("提示"),tr("清除数据集信息成功"),QMessageBox::Yes);
    saveConfig();       //保存配置
    showDataSet();      //显示数据集信息(更新显示)
}
/*填充数据源表单*/
void ConfigDialog::fillDataSourceForm()
{
    int index=ui->index1->value();
    if(dataSourceInfor[index].name!=""){      //如果已经有配置信息，则填充回表单
        ui->name1->setText(dataSourceInfor[index].name);
        ui->enable1->setChecked(dataSourceInfor[index].enable);
        ui->desc1->setText(dataSourceInfor[index].desc);
        ui->baudRate->setCurrentText(dataSourceInfor[index].stringbaudRate);
        ui->parity->setCurrentText(dataSourceInfor[index].stringparity);
        ui->dataBits->setCurrentText(dataSourceInfor[index].stringdataBits);
        ui->stopBits->setCurrentText(dataSourceInfor[index].stringstopBits);
        ui->responseTime->setValue(dataSourceInfor[index].responseTime.toInt());
        ui->numberOfRetries->setValue(dataSourceInfor[index].numberOfRetries.toInt());
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
    if(dataSetInfor[index].name!=""){      //如果已经有配置信息，则填充回表单
    ui->name2->setText(dataSetInfor[index].name);
    ui->dataSource->setCurrentIndex(ui->dataSource->findData(dataSetInfor[index].sourceIndex.toInt()));
    ui->enable2->setChecked(dataSetInfor[index].enable);
    ui->writeEnable->setChecked(dataSetInfor[index].writeEnable);
    ui->desc2->setText(dataSetInfor[index].desc);
    ui->server_read->setText(dataSetInfor[index].readData.serverAddress);
    ui->code_read->setCurrentText(dataSetInfor[index].readData.stringCode);
    ui->startAddr_read->setText(dataSetInfor[index].readData.startAddress);
    ui->length_read->setText(dataSetInfor[index].readData.length);
    ui->server_write->setText(dataSetInfor[index].writeData.serverAddress);
    ui->code_write->setCurrentText(dataSetInfor[index].writeData.stringCode);
    ui->startAddr_write->setText(dataSetInfor[index].writeData.startAddress);
    ui->length_write->setText(dataSetInfor[index].writeData.length);
    }
    else{
    ui->name2->setText(dataSetInfor[index].name);
    ui->dataSource->setCurrentIndex(ui->dataSource->findData(dataSetInfor[index].sourceIndex.toInt()));
    ui->enable2->setChecked(dataSetInfor[index].enable);
    ui->writeEnable->setChecked(dataSetInfor[index].writeEnable);
    ui->desc2->setText(dataSetInfor[index].desc);
    ui->server_read->setText(dataSetInfor[index].readData.serverAddress);
    ui->server_write->setText(dataSetInfor[index].writeData.serverAddress);
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
    if(ui->com->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("端口号不能为空!!!"),QMessageBox::Yes);
      return;
    }
    QSerialPort *serial=new QSerialPort(this);
    serial->setPortName (ui->com->currentText());
    serial->setBaudRate (static_cast<QSerialPort::BaudRate>(ui->baudRate->currentData().toInt()));
    serial->setParity   (static_cast<QSerialPort::Parity>(ui->parity->currentData().toInt()));
    serial->setDataBits (static_cast<QSerialPort::DataBits>(ui->dataBits->currentData().toInt()));
    serial->setStopBits (static_cast<QSerialPort::StopBits>(ui->stopBits->currentData().toInt()));
    if(serial->isOpen ())   //如果端口已经被打开
     {
        QMessageBox::information(this,tr("提示"),tr("端口已经被打开"),QMessageBox::Yes);
        delete serial;
        return;
     }
     serial->open (QIODevice::ReadWrite);
     if(serial->isOpen ())
     {
         QMessageBox::information(this,tr("提示"),tr("端口打开成功"),QMessageBox::Yes);
         serial->close();
         delete serial;
         return;
     }
     else
     {
         QMessageBox::warning(this,tr("提示"),tr("端口打开失败"),QMessageBox::Yes);
         delete serial;
         return;
     }
}
/*获取值测试*/
void ConfigDialog::getValueTest(){
    if(ui->dataSource->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("数据源不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->server_read->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("主机地址不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->startAddr_read->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("起始地址不能为空!!!"),QMessageBox::Yes);
      return;
       }
    if(ui->length_read->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("数据长度不能为空!!!"),QMessageBox::Yes);
      return;
       }
    int  dataSource=ui->dataSource->currentData().toInt();
     /*判断数据源连接状态(如果已经连接)*/
    if(dataSourceInfor[dataSource].modbusDevice!=nullptr){
        if(dataSourceInfor[dataSource].modbusDevice->state()==QModbusDevice::UnconnectedState ||
           dataSourceInfor[dataSource].modbusDevice->state()==QModbusDevice::ClosingState ){
            dataSourceInfor[dataSource].modbusDevice->disconnect();
            dataSourceInfor[dataSource].modbusDevice=nullptr;
        }
    }
    /*判断数据源是否连接*/
    if(dataSourceInfor[dataSource].modbusDevice==nullptr){
        /*连接指定的服务器*/
        QModbusClient  *modbusDevice = new QModbusRtuSerialMaster(this);
        if (modbusDevice){
            modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,dataSourceInfor[dataSource].com);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,dataSourceInfor[dataSource].parity);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,dataSourceInfor[dataSource].baudRate);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,dataSourceInfor[dataSource].dataBits);
            modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,dataSourceInfor[dataSource].stopBits);
            modbusDevice->setTimeout(dataSourceInfor[dataSource].responseTime.toInt());
            modbusDevice->setNumberOfRetries(dataSourceInfor[dataSource].numberOfRetries.toInt());
            modbusDevice->connectDevice();
            if(modbusDevice->state()==QModbusDevice::UnconnectedState ||
               modbusDevice->state()==QModbusDevice::ClosingState ){
                modbusDevice->disconnectDevice();
                QMessageBox::warning(this,tr("提示"),tr("连接数据集的服务器失败!!"),QMessageBox::Yes);
                delete  modbusDevice;
                return ;
            }
            else{
                 dataSourceInfor[dataSource].modbusDevice=modbusDevice;
            }
        }
        else{
         QMessageBox::warning(this,tr("提示"),tr("建立数据集的服务器对象失败!!"),QMessageBox::Yes);
         return ;
        }
    }
    /*构建moubus读数据块*/
    int dataSet=ui->index2->value();
    dataSetInfor[dataSet].readData.value="";
    QModbusDataUnit requestData=QModbusDataUnit(static_cast<QModbusDataUnit::RegisterType>(ui->code_read->currentData().toInt()),
                     ui->startAddr_read->text().toInt(),ui->length_read->text().toUInt());
    QModbusReply *reply = dataSourceInfor[dataSource].modbusDevice->sendReadRequest(requestData,
                         ui->server_read->text().toInt());
    if (reply){
        if (!reply->isFinished()){
            dataSetInfor[dataSet].readData.reply=reply;
            connect(reply, SIGNAL(finished()), this, SLOT(onReadReady()));
        }
        else{
            QMessageBox::warning(this,tr("提示"),tr("发送请求失败!!"),QMessageBox::Yes);
            delete reply;
            return ;
        }
    }
    else {
        QMessageBox::warning(this,tr("提示"),tr("发送请求失败!!"),QMessageBox::Yes);
        return ;
    }
    //添加事件循环机制，返回后再运行后面的
     QEventLoop eventLoop;
     connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
     eventLoop.exec();       //block until finish
     while (dataSetInfor[dataSet].readData.value=="") {
          QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
     }
     delete  reply;
    if(dataSetInfor[dataSet].readData.value=="err"){
        QMessageBox::warning(this,tr("提示"),tr("读取节点值失败，无返回值"),QMessageBox::Yes);
    }
    else{
        QMessageBox::warning(this,tr("提示"),tr("读取节点值成功，值为：")+dataSetInfor[dataSet].readData.value,QMessageBox::Yes);
    }
}
/*设置数值测试*/
void ConfigDialog::setValueTest()
{

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
            json.insert("com",dataSourceInfor[index].com);
            json.insert("baudRate",dataSourceInfor[index].baudRate);
            json.insert("stringbaudRate",dataSourceInfor[index].stringbaudRate);
            json.insert("parity",dataSourceInfor[index].parity);
            json.insert("stringparity",dataSourceInfor[index].stringparity);
            json.insert("dataBits",dataSourceInfor[index].dataBits);
            json.insert("stringdataBits",dataSourceInfor[index].stringdataBits);
            json.insert("stopBits",dataSourceInfor[index].stopBits);
            json.insert("stringstopBits",dataSourceInfor[index].stringstopBits);
            json.insert("responseTime",dataSourceInfor[index].responseTime);
            json.insert("numberOfRetries",dataSourceInfor[index].numberOfRetries);
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
            json.insert("readData_code",dataSetInfor[index].readData.code);
            json.insert("readData_stringCode",dataSetInfor[index].readData.stringCode);
            json.insert("readData_serverAddress",dataSetInfor[index].readData.serverAddress);
            json.insert("readData_startAddress",dataSetInfor[index].readData.startAddress);
            json.insert("readData_length",dataSetInfor[index].readData.length);
            json.insert("writeData_code",dataSetInfor[index].writeData.code);
            json.insert("writeData_stringCode",dataSetInfor[index].writeData.stringCode);
            json.insert("writeData_serverAddress",dataSetInfor[index].writeData.serverAddress);
            json.insert("writeData_startAddress",dataSetInfor[index].writeData.startAddress);
            json.insert("writeData_length",dataSetInfor[index].writeData.length);
            dataSetArray.push_back(json);
    }
    QJsonObject object;
    object.insert("dataSource",dataSourceArray);
    object.insert("dataSet",dataSetArray);
    QJsonDocument document;
    document.setObject(object);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/modbusRTU.ini";
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
    QString fileName=path.path()+"/plugins/config/modbusRTU.ini";
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
        dataSourceInfor[index].com=json.value("com").toString();
        dataSourceInfor[index].baudRate=static_cast<QSerialPort::BaudRate>(json.value("baudRate").toInt());
        dataSourceInfor[index].stringbaudRate=json.value("stringbaudRate").toString();
        dataSourceInfor[index].parity=static_cast<QSerialPort::Parity>(json.value("parity").toInt());
        dataSourceInfor[index].stringparity=json.value("stringparity").toString();
        dataSourceInfor[index].dataBits=static_cast<QSerialPort::DataBits>(json.value("dataBits").toInt());
        dataSourceInfor[index].stringdataBits=json.value("stringdataBits").toString();
        dataSourceInfor[index].stopBits=static_cast<QSerialPort::StopBits>(json.value("stopBits").toInt());
        dataSourceInfor[index].stringstopBits=json.value("stringstopBits").toString();
        dataSourceInfor[index].responseTime=json.value("responseTime").toString();
        dataSourceInfor[index].numberOfRetries=json.value("numberOfRetries").toString();
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
        dataSetInfor[index].readData.code=static_cast<QModbusDataUnit::RegisterType>(json.value("readData_code").toInt());
        dataSetInfor[index].readData.stringCode=json.value("readData_stringCode").toString();
        dataSetInfor[index].readData.serverAddress=json.value("readData_serverAddress").toString();
        dataSetInfor[index].readData.startAddress=json.value("readData_startAddress").toString();
        dataSetInfor[index].readData.length=json.value("readData_length").toString();
        dataSetInfor[index].writeData.code=static_cast<QModbusDataUnit::RegisterType>(json.value("writeData_code").toInt());
        dataSetInfor[index].writeData.stringCode=json.value("writeData_stringCode").toString();
        dataSetInfor[index].writeData.serverAddress=json.value("writeData_serverAddress").toString();
        dataSetInfor[index].writeData.startAddress=json.value("writeData_startAddress").toString();
        dataSetInfor[index].writeData.length=json.value("writeData_length").toString();

   }
}
