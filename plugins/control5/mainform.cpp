#include "mainform.h"
#include "ui_mainform.h"
#include <QThread>
mainForm::mainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainForm)
{
    ui->setupUi(this);
    loadConfig();
    connect(ui->showForm,SIGNAL(clicked()),this,SLOT(showDebugForm()));
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
       QtConcurrent::run(this,&mainForm::autoSave);
    }
    ui->label->setText(GlobalVariable::instance()->getValue("1"));
}
void mainForm::autoSave()
{
 for (int i = 0; i <1000; i++)
  {
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

