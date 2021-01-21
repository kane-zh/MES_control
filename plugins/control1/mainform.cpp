#include "mainform.h"
#include "ui_mainform.h"
#include <QTime>
#include <QThread>
#include <QCoreApplication>
extern "C" __declspec(dllimport) bool BarCodeFxToOcv(char* traycode);
extern "C" __declspec(dllimport) bool CheckTrayCodeState(char* traycode, char* linename, char* floor,char * productcode);
extern "C" __declspec(dllimport) char* GetTrayBarcode(char* traycode);
extern "C" __declspec(dllimport) bool TrayBarCodeUntying(char* traycode);
mainForm::mainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainForm)
{
    ui->setupUi(this);
    loadConfig();        //加载配置信息
    startSerial();
    connect(ui->showDebugForm,SIGNAL(clicked()),this,SLOT(showDebugForm()));
}

mainForm::~mainForm()
{
    delete ui;
}

void mainForm::showEvent(QShowEvent *)
{

}

void mainForm::receiveMsgFromPluginManager(ResponseMetaData response)
{
    switch(response.type){
    case getDrivesInfor:
        driveInfor=response.value;
        break;
    case getDataSetInfor:
        dateSetInfor=response.value;
        break;
    case getValue:
        getValueResult=response.value;
        break;
    case setValue:
        getValueResult=response.value;
        break;
    }
}

/*窗体显示*/
void mainForm::showDebugForm()
{
  if(serialinfor.serialDevice!=nullptr){
    if(serialinfor.serialDevice->isOpen()){
      serialinfor.serialDevice->close();
      delete  serialinfor.serialDevice;
      serialinfor.serialDevice==nullptr;
     }
  }
  ConfigDialog *m_config=new ConfigDialog(this);
  m_config->exec();
  delete  m_config;
  loadConfig();
  startSerial();
}

void mainForm::startSerial()
{
    QSerialPort *serial=new QSerialPort(this);
    serial->setPortName (serialinfor.com);
    serial->setBaudRate (static_cast<QSerialPort::BaudRate>(serialinfor.baudRate));
    serial->setParity   (static_cast<QSerialPort::Parity>(serialinfor.parity));
    serial->setDataBits (static_cast<QSerialPort::DataBits>(serialinfor.dataBits));
    serial->setStopBits (static_cast<QSerialPort::StopBits>(serialinfor.stopBits));
    if(serial->isOpen ())   //如果端口已经被打开
     {
        qDebug()<<"端口已经被打开";
        QMessageBox::warning(this,tr("提示"),tr("端口已经被打开!!!"),QMessageBox::Yes);
        serialinfor.serialDevice=serial;
        connect(serial, SIGNAL(finished()), this, SLOT(onReadReady()));
        connect(serial,SIGNAL(errorOccurred(QModbusDevice::Error)),this,SLOT(handleSerialError(QModbusDevice::Error )));
        return;
     }
     serial->open (QIODevice::ReadWrite);
     if(serial->isOpen ())
     {
        qDebug()<<"端口打开成功";
        QMessageBox::information(this,tr("提示"),tr("端口打开成功!!!"),QMessageBox::Yes);
        serialinfor.serialDevice=serial;
        connect(serial, SIGNAL(readyRead()), this, SLOT(onReadReady()));
        connect(serial,SIGNAL(errorOccurred(QModbusDevice::Error)),this,SLOT(handleSerialError(QModbusDevice::Error )));
        delete  serial;
        return;
     }
     else
     {
         QMessageBox::information(this,tr("提示"),tr("端口打开失败!!!"),QMessageBox::Yes);
         qDebug()<<"端口打开失败";
         delete  serial;
         return;
     }
}
/*数据读取槽*/
void mainForm::onReadReady()
{
    QJsonDocument document;
    QJsonObject json;
    auto reply = qobject_cast<QSerialPort *>(sender());
    if (reply->error() != QSerialPort::NoError) {
        QMessageBox::information(this,tr("提示"),tr("串口读取数据失败!!!"),QMessageBox::Yes);
        qDebug()<<"串口读取数据失败";
        return;
    }
    QString serialValue=reply->readAll();
    serialValue.remove(serialValue.length()-2,4);
    ui->serialValue->setText(serialValue);
    QString result;
    result=readFromDrive("0");
    if(result=="err")
    {
       QMessageBox::warning(this,tr("提示"),tr("读取plc数据失败(0)!!!"),QMessageBox::Yes);
       qDebug()<<"读取plc数据失败(0)";
       return;
    }
    if(result!="00001")
    {
       QMessageBox::information(this,tr("提示"),tr("扫码枪误触发!!!"),QMessageBox::Yes);
       qDebug()<<"扫码枪误触发";
       return;
    }
    /*自动转序*/
    bool ret=BarCodeFxToOcv(serialValue.toUtf8().data());
    if(ret==false){
       result=writeToDrive("0","00005");
       if(result=="err")
       {
        QMessageBox::warning(this,tr("提示"),tr("写plc数据失败(0)!!!"),QMessageBox::Yes);
        qDebug()<<"写plc数据失败(0)";
        return;
       }
               QMessageBox::information(this,tr("提示"),tr("串口读取数据失败!!!"),QMessageBox::Yes);
       qDebug()<<"转序失败";
       return;
    }
    result=writeToDrive("0","00004");
    if(result=="err")
    {
        QMessageBox::warning(this,tr("提示"),tr("写plc数据失败(1)!!!"),QMessageBox::Yes);
        qDebug()<<"写plc数据失败(1)";
        return;
    }
    /*确认托盘状态*/
    ui->linename->setText(mesinfor.linename);
    ui->floorcode->setText(mesinfor.floorcode);
    ui->producttype->setText(mesinfor.producttype);
    ret=CheckTrayCodeState(serialValue.toUtf8().data(),mesinfor.linename.toUtf8().data(),
                           mesinfor.floorcode.toUtf8().data(),mesinfor.producttype.toUtf8().data());
    if(ret==false){
        result=writeToDrive("0","00007");
        if(result=="err")
        {
            QMessageBox::warning(this,tr("提示"),tr("写plc数据失败(7)!!!"),QMessageBox::Yes);
            qDebug()<<"写plc数据失败(7)";
            return;
        }
        QMessageBox::information(this,tr("提示"),tr("当前托盘不能在此线生产!!!"),QMessageBox::Yes);
        qDebug()<<"当前托盘不能在此线生产";
       return;
    }
    result=writeToDrive("0","00006");
    if(result=="err")
    {
        QMessageBox::warning(this,tr("提示"),tr("写plc数据失败(6)!!!"),QMessageBox::Yes);
        qDebug()<<"写plc数据失败(6)";
        return;
       return;
    }
    /**/
    char* data;
    QStringList list;//QString字符串分割函数
    data=GetTrayBarcode(serialValue.toUtf8().data());
    QString qStr1(data);
    list=qStr1.split(";");
    char number[259]={0};
    for(int i=1;i<list.count();i++){
      number[list.at(i-1).toInt()]=1;
    }
    for(int i=1;i<259;i++){
        result=writeToDrive(QString::number(i),QString::number(number[i]));
        if(result=="err")
        {
            QMessageBox::warning(this,tr("提示"),tr("写plc数据失败(111)!!!"),QMessageBox::Yes);
            qDebug()<<"写plc数据失败(111)";
        }
    }
    /*解绑托盘*/
     ret=TrayBarCodeUntying(serialValue.toUtf8().data());
    if(ret==false){
        result=writeToDrive("0","00011");
        if(result=="err")
        {
            QMessageBox::warning(this,tr("提示"),tr("写plc数据失败(11)!!!"),QMessageBox::Yes);
            qDebug()<<"写plc数据失败(11)";
            return;
        }
       QMessageBox::warning(this,tr("提示"),tr("解绑托盘失败!!!"),QMessageBox::Yes);
       qDebug()<<"解绑托盘失败";
       return;
    }
    result=writeToDrive("0","00010");
    if(result=="err")
    {
        QMessageBox::warning(this,tr("提示"),tr("写plc数据失败(10)!!!"),QMessageBox::Yes);
        qDebug()<<"写plc数据失败(10)";
        return;
    }


}
QString mainForm::readFromDrive(QString id){
    RequestMetaData data;
    data.type=getValue;
    data.from="1";
    data.target="manage";
    data.drive="KEYENCE";
    data.id=id;
    getValueResult="";
    emit sendMsgToPluginManager(data);//发送信号到插件管理器
    while(getValueResult==""){
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    QJsonDocument document=QJsonDocument::fromJson(getValueResult.toLocal8Bit().data());
    QJsonObject object=document.object();
    if(object.value("result").toString()=="err"){
        return "err";
    }
    return object.value("value").toString();
}
QString mainForm::writeToDrive(QString id,QString value){
    RequestMetaData data;
    data.type=setValue;
    data.from="1";
    data.target="manage";
    data.drive="KEYENCE";
    data.id=id;
    data.value=value;
    getValueResult="";
    emit sendMsgToPluginManager(data);//发送信号到插件管理器
    while(getValueResult==""){
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    QJsonDocument document=QJsonDocument::fromJson(getValueResult.toLocal8Bit().data());
    QJsonObject object=document.object();
    if(object.value("result").toString()=="err"){
        return "err";
    }
    return "ok";
}
void mainForm::handleSerialError(QSerialPort::SerialPortError err)
{

}

/*加载信息*/
void mainForm::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/control1.ini";
    QFile file(fileName);
   if (!file.open(QFile::ReadOnly)) {   //如果文件不存在则新建文件
       file.open( QIODevice::ReadWrite | QIODevice::Text );
      }
    QByteArray data=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(data);
    QJsonObject object=doc.object();
    QJsonObject serial=object.value("serial").toObject();
    QJsonObject mes=object.value("mes").toObject();
    serialinfor.name=serial.value("name").toString();
    serialinfor.com=serial.value("com").toString();
    serialinfor.baudRate=static_cast<QSerialPort::BaudRate>(serial.value("baudRate").toInt());
    serialinfor.stringbaudRate=serial.value("stringbaudRate").toString();
    serialinfor.parity=static_cast<QSerialPort::Parity>(serial.value("parity").toInt());
    serialinfor.stringparity=serial.value("stringparity").toString();
    serialinfor.dataBits=static_cast<QSerialPort::DataBits>(serial.value("dataBits").toInt());
    serialinfor.stringdataBits=serial.value("stringdataBits").toString();
    serialinfor.stopBits=static_cast<QSerialPort::StopBits>(serial.value("stopBits").toInt());
    serialinfor.stringstopBits=serial.value("stringstopBits").toString();
    mesinfor.name=mes.value("name").toString();
    mesinfor.projectcode=mes.value("projectcode").toString();
    mesinfor.floorcode=mes.value("floorcode").toString();
    mesinfor.linename=mes.value("linename").toString();
    mesinfor.producttype=mes.value("producttype").toString();
}

