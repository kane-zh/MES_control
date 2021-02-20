#include "mainform.h"
#include "ui_mainform.h"
#include <QThread>
mainForm::mainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainForm)
{
    ui->setupUi(this);
    loadConfig();
    connect(ui->dataPointConfig,SIGNAL(clicked()),this,SLOT(showDebugForm()));
    connect(ui->init_all,SIGNAL(clicked()),this,SLOT(Init()));//总初始化
    connect(ui->run_all,SIGNAL(clicked()),this,SLOT(Init()));//总运行
    connect(ui->pause_all,SIGNAL(clicked()),this,SLOT(Init()));//总暂停
    connect(ui->reset_all,SIGNAL(clicked()),this,SLOT(Init()));//总报警复位
}

mainForm::~mainForm()
{
    delete ui;
}

void mainForm::showEvent(QShowEvent *)
{
    m_time=new QTimer(this);
    connect(m_time,SIGNAL(timeout()),this,SLOT(timeOut()));
    m_time->start(200);
    StartAllThread();
}
/*从插件管理器接收消息(回调)*/
void mainForm::receiveMsgFromPluginManager(ResponseMetaData response)
{
    /*判断消息是否发送给对话框界面的*/
    if(response.target=="dialog"){
      ResponseMetaData_dialog data;
      if(response.type==getDrivesInfor){
         data.type="getDrivesInfor";
      }
      if(response.type==getDataSetInfor){
         data.type="getDataSetInfor";
      }
      if(response.type==setValue){
         data.type="setValue";
      }
      if(response.type==getValue){
         data.type="getValue";
      }
      data.value=response.value;
      if(m_config!=nullptr){
      m_config->receiveMsgFromPluginInterface(data);
      }
      return;
    }
     if(response.target=="thread"){
      return;
     }
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
/*处理对话框发送的信号*/
void mainForm::dealSignalOfDialog(RequestMetaData_dialog request)
{
    RequestMetaData data;
    data.from="dialog";
    data.target="pluginManage";
    data.drive=request.drive;
    data.id=request.id;
    data.value=request.value;
    if(request.type=="getDrivesInfor"){
       data.type=getDrivesInfor;
    }
    if(request.type=="getDataSetInfor"){
       data.type=getDataSetInfor;
    }
    if(request.type=="setValue"){
       data.type=setValue;
    }
    if(request.type=="getValue"){
       data.type=getValue;
    }
    emit sendMsgToPluginManager(data); //发送信号到插件管理器
}
/*处理线程发送的信号*/
void mainForm::dealSignalOfThread(QString request)
{
    RequestMetaData data;
    data.from="thread";
    data.target="pluginManage";
    data.drive="OPCUA";
    data.id="1";
    data.value="";
    data.type=getValue;
    emit sendMsgToPluginManager(data); //发送信号到插件管理器
}
/*窗体显示*/
void mainForm::showDebugForm()
{
    m_config=new ConfigDialog(this);
    connect(m_config,SIGNAL(SendMsgToPluginInterface(RequestMetaData_dialog)),this,SLOT(dealSignalOfDialog(RequestMetaData_dialog)));
    m_config->exec();
    delete m_config;
    m_config=nullptr;
    loadConfig();
}
/*时间定时器超时(槽)*/
void mainForm::timeOut()
{
    if(getValueEnable==true){
       getValueEnable=false;
//       QtConcurrent::run(this,&mainForm::autoSave);
    }
}
void mainForm::autoSave()
{
 for (int i = 0; i <200; i++)
  {
     if(!this->isVisible()){
      return;
     }
     if(m_datapoint[i].dataId!=""){
         RequestMetaData data;
         data.type=getValue;
         data.from=m_datapoint[i].name;
         data.target="manage";
         data.drive=m_datapoint[i].drive;
         data.id=m_datapoint[i].dataId;
         getValueResult="";
         emit sendMsgToPluginManager(data);
         while(getValueResult==""){
              QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
         }
         QJsonDocument document=QJsonDocument::fromJson(getValueResult.toLocal8Bit().data());
          if(document.object().value("result").toString()=="err"){
            m_datapoint[i].errCount++;
         }
         else{
           GlobalVariable::instance()->setValue(m_datapoint[i].name,document.object().value("value").toString());
         }
       }
   }
 getValueEnable=true;
}
//启动所有子任务，并绑定信号槽
void mainForm::StartAllThread()
{
    if(pThread01==nullptr)
    {
        pThread01=new RunThread01(this);
        connect(pThread01,SIGNAL(SendMsgToPluginInterface(QString)),this,SLOT(dealSignalOfThread(QString)));
        connect(ui->init_1,SIGNAL(clicked()),pThread01,SLOT(startInit()));//初始化1
        connect(ui->run_1,SIGNAL(clicked()),pThread01,SLOT(startRun()));//运行1
        connect(ui->pause_1,SIGNAL(clicked()),pThread01,SLOT(startPause()));//暂停1
        connect(ui->init_1,SIGNAL(clicked()),pThread01,SLOT(startReset()));//重启1
    }
    if(pThread02==nullptr)
    {
        pThread02=new RunThread02(this);
        connect(pThread02,SIGNAL(SendMsgToPluginInterface(QString)),this,SLOT(dealSignalOfThread(QString)));
        connect(ui->init_2,SIGNAL(clicked()),pThread02,SLOT(startInit()));//初始化2
        connect(ui->run_2,SIGNAL(clicked()),pThread02,SLOT(startRun()));//运行2
        connect(ui->pause_2,SIGNAL(clicked()),pThread02,SLOT(startPause()));//暂停2
        connect(ui->init_2,SIGNAL(clicked()),pThread02,SLOT(startReset()));//重启2
    }
    if(pThread03==nullptr)
    {
        pThread03=new RunThread03(this);
        connect(pThread03,SIGNAL(SendMsgToPluginInterface(QString)),this,SLOT(dealSignalOfThread(QString)));
        connect(ui->init_3,SIGNAL(clicked()),pThread03,SLOT(startInit()));//初始化3
        connect(ui->run_3,SIGNAL(clicked()),pThread03,SLOT(startRun()));//运行3
        connect(ui->pause_3,SIGNAL(clicked()),pThread03,SLOT(startPause()));//暂停3
        connect(ui->init_3,SIGNAL(clicked()),pThread03,SLOT(startReset()));//重启3
    }
    if(pThread04==nullptr)
    {
        pThread04=new RunThread04(this);
        connect(pThread04,SIGNAL(SendMsgToPluginInterface(QString)),this,SLOT(dealSignalOfThread(QString)));
        connect(ui->init_4,SIGNAL(clicked()),pThread04,SLOT(startInit()));//初始化4
        connect(ui->run_4,SIGNAL(clicked()),pThread04,SLOT(startRun()));//运行4
        connect(ui->pause_4,SIGNAL(clicked()),pThread04,SLOT(startPause()));//暂停4
        connect(ui->init_4,SIGNAL(clicked()),pThread04,SLOT(startReset()));//重启4
    }
    if(pThread05==nullptr)
    {
        pThread05=new RunThread05(this);
        connect(pThread05,SIGNAL(SendMsgToPluginInterface(QString)),this,SLOT(dealSignalOfThread(QString)));
        connect(ui->init_5,SIGNAL(clicked()),pThread05,SLOT(startInit()));//初始化5
        connect(ui->run_5,SIGNAL(clicked()),pThread05,SLOT(startRun()));//运行5
        connect(ui->pause_5,SIGNAL(clicked()),pThread05,SLOT(startPause()));//暂停5
        connect(ui->init_5,SIGNAL(clicked()),pThread05,SLOT(startReset()));//重启5
    }
    if(pThread06==nullptr)
    {
        pThread06=new RunThread06(this);
        connect(pThread06,SIGNAL(SendMsgToPluginInterface(QString)),this,SLOT(dealSignalOfThread(QString)));
        connect(ui->init_6,SIGNAL(clicked()),pThread06,SLOT(startInit()));//初始化6
        connect(ui->run_6,SIGNAL(clicked()),pThread06,SLOT(startRun()));//运行6
        connect(ui->pause_6,SIGNAL(clicked()),pThread06,SLOT(startPause()));//暂停6
        connect(ui->init_6,SIGNAL(clicked()),pThread06,SLOT(startReset()));//重启6
    }
}
//总初始化流程
void mainForm::Init()
{
    QString ALLState_str=GlobalVariable::instance()->getValue("ALLState");
    BaseProcess::state ALLState = (BaseProcess::state)QMetaEnum::fromType<BaseProcess::state>().keyToValue(ALLState_str.toUtf8());
    if ((ALLState == BaseProcess::state::Unknown) ||
        (ALLState == BaseProcess::state::IsPause) ||
        (ALLState == BaseProcess::state::IsAlarm))
    {
        pThread01->startInit();
        pThread02->startInit();
        pThread03->startInit();
        pThread04->startInit();
        pThread05->startInit();
        ALLState = BaseProcess::state::IsInitializing;
    }
}
//总运行流程
void mainForm::Run()
{
    QString ALLState_str=GlobalVariable::instance()->getValue("ALLState");
    BaseProcess::state ALLState = (BaseProcess::state)QMetaEnum::fromType<BaseProcess::state>().keyToValue(ALLState_str.toUtf8());
    if ((ALLState == BaseProcess::state::IsPause))
    {
        pThread01->startRun();
        pThread02->startRun();
        pThread03->startRun();
        pThread04->startRun();
        pThread05->startRun();
        ALLState = BaseProcess::state::IsRuning;
    }
}
//总暂停流程
void mainForm::Pause()
{
    QString ALLState_str=GlobalVariable::instance()->getValue("ALLState");
    BaseProcess::state ALLState = (BaseProcess::state)QMetaEnum::fromType<BaseProcess::state>().keyToValue(ALLState_str.toUtf8());
    if (ALLState == BaseProcess::state::IsRuning)
    {
        pThread01->startPause();
        pThread02->startPause();
        pThread03->startPause();
        pThread04->startPause();
        pThread05->startPause();
        ALLState = BaseProcess::state::IsPause;
    }
}
//总报警复位流程
void mainForm::Reset()
{
    QString ALLState_str=GlobalVariable::instance()->getValue("ALLState");
    BaseProcess::state ALLState = (BaseProcess::state)QMetaEnum::fromType<BaseProcess::state>().keyToValue(ALLState_str.toUtf8());
    if ((ALLState == BaseProcess::state::IsAlarm))
    {
        pThread01->startReset();
        pThread02->startReset();
        pThread03->startReset();
        pThread04->startReset();
        pThread05->startReset();
        ALLState = BaseProcess::state::IsPause;
    }
}
/*加载信息*/
void mainForm::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/control5.ini";
    QFile file(fileName);
   if (!file.open(QFile::ReadOnly)) {   //如果文件不存在则新建文件
       file.open( QIODevice::ReadWrite | QIODevice::Text );
      }
   QByteArray data=file.readAll();
   file.close();
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonArray array= document.array();
    for (int i = 0; i < array.count(); i++)
     {
        QJsonObject json = array.at(i).toObject();
        m_datapoint[i].name=json.value("name").toString();
        m_datapoint[i].drive=json.value("drive").toString();
        m_datapoint[i].dataName=json.value("dataName").toBool();
        m_datapoint[i].dataId=json.value("dataId").toString();
     }
}

