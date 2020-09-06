#include "configdialog.h"
#include "ui_configdialog.h"

#ifdef Q_OS_WIN
    extern "C" __declspec(dllimport) char * UpdateDLL(char *);
    extern "C" __declspec(dllimport) char * GetPkFloorList();
    extern "C" __declspec(dllimport) char * GetLineNameList();
    extern "C" __declspec(dllimport) char * GetProjCode();
#endif

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();  //可用区域
    this->resize(deskRect.width()/2,deskRect.height()/1.2);
    loadConfig();          //加载配置
    connect(ui->save1,SIGNAL(clicked()),this,SLOT(setSerialConfig()));
    connect(ui->save2,SIGNAL(clicked()),this,SLOT(setMesConfig()));
    connect(ui->connectTest,SIGNAL(clicked()),this,SLOT(connectTest()));
    connect(ui->projectcode,SIGNAL(currentIndexChanged(int)),this,SLOT(fillMesInfo()));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

/*窗体显示事件*/
void ConfigDialog::showEvent(QShowEvent *)
{
    fillInfo();          //填充串口下拉框信息
    fillSerialForm();    //填充串口表单信息(配置信息回填到表单)
    fillMesForm();       //填充mes表单(配置信息回填到表单)

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
}

void ConfigDialog::fillMesInfo()
{
    #ifdef Q_OS_WIN
    char* result;
    QStringList list;//QString字符串分割函数
    result=UpdateDLL(ui->projectcode->currentText().toUtf8().data());
//    if(result!="Success"){
//        int ok=QMessageBox::warning(this,tr("提示"),tr("执行更新MES接口程序失败,是否退出??"),QMessageBox::Yes,QMessageBox::No);
//        if(ok==QMessageBox::No)
//        {
//          return;
//        }
//    }
    result=GetPkFloorList();
    QString qStr1(result);
    list=qStr1.split(";");
     ui->floorcode->clear();
    for(int i=0;i<list.count();i++){
        ui->floorcode->addItem(list.at(i));
    }
    result=GetLineNameList();
    QString qStr2(result);
    list=qStr2.split(";");
    ui->linename->clear();
    for(int i=0;i<list.count();i++){
        ui->linename->addItem(list.at(i));
    }
    result=GetProjCode();
    QString qStr3(result);
    list=qStr3.split(";");
    ui->producttype->clear();
    for(int i=0;i<list.count();i++){
        ui->producttype->addItem(list.at(i));
    }
    #endif
}
/*设置串口配置信息*/
void ConfigDialog::setSerialConfig()
{
    if(serialinfor.name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前串口已有配置信息，是否更新??"),QMessageBox::Yes,QMessageBox::No);
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
    serialinfor.name=ui->name1->text();
    serialinfor.com=ui->com->currentText();
    serialinfor.baudRate= static_cast<QSerialPort::BaudRate>(ui->baudRate->currentData().toInt());
    serialinfor.stringbaudRate=ui->baudRate->currentText();
    serialinfor.parity= static_cast<QSerialPort::Parity>(ui->parity->currentData().toInt());
    serialinfor.stringparity=ui->parity->currentText();
    serialinfor.dataBits= static_cast<QSerialPort::DataBits>(ui->dataBits->currentData().toInt());
    serialinfor.stringdataBits=ui->dataBits->currentText();
    serialinfor.stopBits= static_cast<QSerialPort::StopBits>(ui->stopBits->currentData().toInt());
    serialinfor.stringstopBits=ui->stopBits->currentText();
    QMessageBox::information(this,tr("提示"),tr("设置串口成功"),QMessageBox::Yes);
    saveConfig();       //保存配置
}
/*设置mes配置信息*/
void ConfigDialog::setMesConfig()
{
    if(mesinfor.name!=""){
        int ok=QMessageBox::warning(this,tr("提示"),tr("当前mes已有配置信息，是否更新??"),QMessageBox::Yes,QMessageBox::No);
        if(ok==QMessageBox::No)
        {
          return;
        }
    }
    if(ui->name2->text()==""){
      QMessageBox::warning(this,tr("提示"),tr("名称不能为空!!!"),QMessageBox::Yes);
      return;
    }
    if(ui->floorcode->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("楼层不能为空!!!"),QMessageBox::Yes);
      return;
    }
    if(ui->linename->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("线体不能为空!!!"),QMessageBox::Yes);
      return;
    }
    if(ui->producttype->currentText()==""){
      QMessageBox::warning(this,tr("提示"),tr("产品类型不能为空!!!"),QMessageBox::Yes);
      return;
    }
    mesinfor.name=ui->name2->text();
    mesinfor.projectcode=ui->projectcode->currentText();
    mesinfor.floorcode=ui->floorcode->currentText();
    mesinfor.linename=ui->linename->currentText();
    mesinfor.producttype=ui->producttype->currentText();
    QMessageBox::information(this,tr("提示"),tr("设置mes信息成功"),QMessageBox::Yes);
    saveConfig();       //保存配置
}
/*填充数据库表单*/
void ConfigDialog::fillSerialForm()
{
    ui->name1->setText(serialinfor.name);
    ui->baudRate->setCurrentText(serialinfor.stringbaudRate);
    ui->parity->setCurrentText(serialinfor.stringparity);
    ui->dataBits->setCurrentText(serialinfor.stringdataBits);
    ui->stopBits->setCurrentText(serialinfor.stringstopBits);
}
/*填充数据表表单*/
void ConfigDialog::fillMesForm()
{
    ui->name2->setText(mesinfor.name);
    ui->projectcode->setCurrentText(mesinfor.projectcode);
    ui->floorcode->setCurrentText(mesinfor.floorcode);
    ui->linename->setCurrentText(mesinfor.linename);
    ui->producttype->setCurrentText(mesinfor.producttype);
}
/*连接串口测试*/
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
        serial->close();
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
/*保存信息*/
void ConfigDialog::saveConfig()
{
    QJsonObject serialjson;
    serialjson.insert("name",serialinfor.name);
    serialjson.insert("com",serialinfor.com);
    serialjson.insert("baudRate",serialinfor.baudRate);
    serialjson.insert("stringbaudRate",serialinfor.stringbaudRate);
    serialjson.insert("parity",serialinfor.parity);
    serialjson.insert("stringparity",serialinfor.stringparity);
    serialjson.insert("dataBits",serialinfor.dataBits);
    serialjson.insert("stringdataBits",serialinfor.stringdataBits);
    serialjson.insert("stopBits",serialinfor.stopBits);
    serialjson.insert("stringstopBits",serialinfor.stringstopBits);
    QJsonObject mesjson;
    mesjson.insert("name",mesinfor.name);
    mesjson.insert("projectcode",mesinfor.projectcode);
    mesjson.insert("floorcode",mesinfor.floorcode);
    mesjson.insert("linename",mesinfor.linename);
    mesjson.insert("producttype",mesinfor.producttype);

    QJsonObject object;
    object.insert("serial",serialjson);
    object.insert("mes",mesjson);
    QJsonDocument document;
    document.setObject(object);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/control1.ini";
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
