#include "mainform.h"
#include "ui_mainform.h"
#include <QThread>
mainForm::mainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainForm)
{
    ui->setupUi(this);
    loadConfig();
    m_request=new httpRequest(this);
    connect(ui->showForm,SIGNAL(clicked()),this,SLOT(showDebugForm()));
    connect(ui->test,SIGNAL(clicked()),this,SLOT(test()));
    connect(this,SIGNAL(errOccur(QString)),this,SLOT(showMsg(QString)));
    connect(ui->taskType,SIGNAL(currentTextChanged(QString)),this,SLOT(fillTaskBox()));
    connect(ui->task,SIGNAL(currentTextChanged(QString)),this,SLOT(showTaskItem()));
}

mainForm::~mainForm()
{
    recordInfor.runFlag=false;
    delete ui;

}

void mainForm::showEvent(QShowEvent *)
{
    taskModel=new QStandardItemModel(this);  //建立任务显示model实例
    ui->tableView->setModel(taskModel);     //为显示设置模型
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection);//设置选中单个
    fillTaskTypeBox();
}

/*从主程序框架接收消息*/
void mainForm::receiveMsgFromManager(ResponseMetaData response)
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
      emit sendMsgToDialog(data);
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
/*处理从对话框接收的信号*/
void mainForm::receiveMsgFromDialog(RequestMetaData_dialog request)
{
        RequestMetaData data;
        data.from="dialog";
        data.target="pluginManage";
        data.drive=request.drive;
        data.index=request.index;
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
        emit sendMsgToManager(data); //转发信息到插件管理器
}

/*窗体显示*/
void mainForm::showDebugForm()
{
    ConfigDialog *m_config=new ConfigDialog(this);
    connect(m_config,SIGNAL(SendMsgToContainerManage(RequestMetaData_dialog)),this,SLOT(receiveMsgFromDialog(RequestMetaData_dialog)));
    connect(this,SIGNAL(sendMsgToDialog(ResponseMetaData_dialog)),m_config,SLOT(receiveMsgFromContainerManage(ResponseMetaData_dialog)));
    m_config->exec();
    loadConfig();
}
void mainForm::autoReport()
{
     QString result;
     int step=0;
     int errCount=0;
     QString startTime,stopTime,timeLong;
     QTime  time;
     while(recordInfor.runFlag){
         switch (step) {
         case 0: //获取设备状态,当设备状态变为运行中开始计时,并转入下一步
         {
            result=readFromDrive("17");
            if(result=="err"){  //如果获取失败连续超过3次,报警退出
                errCount++;
                if(errCount>3){
                  emit errOccur("读取设备状态累计3次失败,请检查原因！！！");
                  recordInfor.runFlag=false;
                  break;
                }
            }
            else{ //获取数值成功,则判断设备状态
              errCount=0;
              QJsonDocument document_result = QJsonDocument::fromJson(result.toUtf8());
              if(document_result.object().value("run")=="3")
                 {
                   startTime=QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz");
                   time.start();
                   step=1;
                 }
            }
           break;
         }
         case 1: //登录MES并获取当前任务情况,判断无误后,进入下一步
         {
            if(serverInfor.token==""){
                 result=m_request->login("http://124.70.176.250:8000","admin","admin123");
                 if(result=="err"){
                    emit errOccur("连接服务器失败!!!");
                    recordInfor.runFlag=false;
                    break;
                 }
                 serverInfor.token=result;
            }
             result=m_request->get("http://124.70.176.250:8000/plan/productTaskItemCreate/1/",serverInfor.token);
             if(result=="err"){
                emit errOccur("读取计划失败!!!");
                serverInfor.token="";
                recordInfor.runFlag=false;
                break;
             }
             QJsonDocument document_result = QJsonDocument::fromJson(result.toUtf8());
             if(document_result.object().value("state")!="加工中"){
                 emit errOccur("当前选择的生产任务非【加工中】状态,请检查设置是否正确!!!");
                 recordInfor.runFlag=false;
                 break;
             }
             else{
                 step=2;
             }
             break;
         }
         case 2: //获取设备状态,当设备状态变为暂停中结束计时,并转入下一步
         {
            result=readFromDrive("17");
            if(result=="err"){  //如果获取失败连续超过3次,报警退出
                errCount++;
                if(errCount>3){
                  emit errOccur("读取设备状态累计3次失败,请检查原因！！！");
                  recordInfor.runFlag=false;
                  break;
                }
            }
            else{ //获取数值成功,则判断设备状态
              errCount=0;
              QJsonDocument document_result = QJsonDocument::fromJson(result.toUtf8());
              if(document_result.object().value("run")=="0")
                 {
                   stopTime=QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz");
                   timeLong=QString::number(time.elapsed(),10);
                   step=3;
                 }
            }
           break;
         }
         case 3: //获取任务子项信息,并更新任务子项数据
         {
             result=m_request->get("http://124.70.176.250:8000/plan/productTaskItemCreate/1/",serverInfor.token);
             if(result=="err"){
                emit errOccur("读取计划失败!!!");
                serverInfor.token="";
                recordInfor.runFlag=false;
             }
             QJsonDocument document_result = QJsonDocument::fromJson(result.toUtf8());
             QJsonObject obj=document_result.object();
             if(document_result.object().value("state")!="加工中"){
                 emit errOccur("当前选择的生产任务非【加工中】状态,请检查设置是否正确!!!");
                 recordInfor.runFlag=false;
                 break;
             }
             else{
                 step=4;
             }
             break;
         }
         case 4: //自动报工
         {
             result=m_request->get("http://124.70.176.250:8000/plan/productTaskItemCreate/1/",serverInfor.token);
             if(result=="err"){
                emit errOccur("读取计划失败!!!");
                serverInfor.token="";
                recordInfor.runFlag=false;
                break;
             }
             //发送数据
            QJsonObject json;
            json.insert("type",ui->taskType->currentData().toString());
            json.insert("product_id","1");
            json.insert("task_id","1");
            json.insert("batch","name");
            json.insert("dataTime",QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss.zzz"));
            json.insert("attribute1",startTime);
            json.insert("attribute2",stopTime);
            json.insert("attribute3",timeLong);
             QJsonDocument document;
             document.setObject(json);
             QByteArray qByteHttpData = document.toJson(QJsonDocument::Compact);
             result=m_request->post("http://124.70.176.250:8000/production/productData/",serverInfor.token,qByteHttpData);
             if(result=="err"){
                emit errOccur("写入产品生产记录数据失败!!!");
                serverInfor.token="";
                recordInfor.runFlag=false;
                break;
             }
             recordInfor.runFlag=false;
             break;
         }
         }
         if(recordInfor.runFlag==true){
           QThread::msleep(100);
         }

    }
}

void mainForm::test()
{
    if(recordInfor.runFlag==false){
      recordInfor.runFlag=true;
      QtConcurrent::run(this,&mainForm::autoReport);
    }
}

void mainForm::showMsg(QString message)
{
    QMessageBox::warning(this,tr("提示"),message,QMessageBox::Yes);
}

void mainForm::fillTaskTypeBox()
{
    if(serverInfor.address==""){
        return;
    }
    QString result;
    ui->taskType->clear();
    if(serverInfor.token==""){
         result=m_request->login(serverInfor.address,serverInfor.username,serverInfor.password);
         if(result=="err"){
            QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
            recordInfor.runFlag=false;
            return;
         }
         serverInfor.token=result;
    }
     result=m_request->get(serverInfor.address+"/plan/productTaskType/",serverInfor.token);
     if(result=="err"){
        QMessageBox::warning(this,tr("提示"),tr("读取任务类型失败!!!"),QMessageBox::Yes);
        serverInfor.token="";
     }
     QJsonDocument document_result = QJsonDocument::fromJson(result.toUtf8());
     QJsonArray result_array=document_result.object().value("results").toArray();
     for(int i=0;i<result_array.size();i++) {
         QString text=result_array.at(i).toObject().value("name").toString()+"("+result_array.at(i).toObject().value("code").toString()+")";
         QString data=QString::number(result_array.at(i).toObject().value("id").toInt());
         ui->taskType->addItem(text,data);
      }

}

void mainForm::fillTaskBox()
{
    if(ui->taskType->currentText()==""){
        return;
    }

    QString result;
    ui->task->clear();
    if(serverInfor.token==""){
         result=m_request->login(serverInfor.address,serverInfor.username,serverInfor.password);
         if(result=="err"){
            QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
            recordInfor.runFlag=false;
            return;
         }
         serverInfor.token=result;
    }
     result=m_request->get(serverInfor.address+"/plan/productTaskCreate/?page_size=99999&ordering=-id&state=使用中&type="+ui->taskType->currentData().toString(),serverInfor.token);
     if(result=="err"){
        QMessageBox::warning(this,tr("提示"),tr("读取任务失败!!!"),QMessageBox::Yes);
        serverInfor.token="";
     }
     QJsonDocument document_result = QJsonDocument::fromJson(result.toUtf8());
     QJsonArray result_array=document_result.object().value("results").toArray();
     for(int i=0;i<result_array.size();i++) {
         QString text=result_array.at(i).toObject().value("name").toString()+"("+result_array.at(i).toObject().value("code").toString()+")";
         QString data=QString::number(result_array.at(i).toObject().value("id").toInt());
         ui->task->addItem(text,data);
      }

}

void mainForm::showTaskItem()
{
    taskModel->clear();
    QStringList list;
    list<<tr("产品类型")<<tr("产品")<<tr("批次")<<tr("状态")<<tr("数量")<<tr("完成数")
            <<tr("工位1")<<tr("工位1完成数")<<tr("工位2")<<tr("工位2完成数")<<tr("工位3")<<tr("工位3完成数")
            <<tr("工位4")<<tr("工位4完成数")<<tr("工位5")<<tr("工位5完成数")<<tr("工位6")<<tr("工位6完成数");
    taskModel->setHorizontalHeaderLabels(list);

    QString result;
    if(serverInfor.token==""){
         result=m_request->login(serverInfor.address,serverInfor.username,serverInfor.password);
         if(result=="err"){
            QMessageBox::warning(this,tr("提示"),tr("连接服务器失败!!!"),QMessageBox::Yes);
            recordInfor.runFlag=false;
            return;
         }
         serverInfor.token=result;
    }
     result=m_request->get(serverInfor.address+"/plan/productTaskCreate/"+ui->task->currentData().toString()+"/",serverInfor.token);
     if(result=="err"){
        QMessageBox::warning(this,tr("提示"),tr("读取任务失败!!!"),QMessageBox::Yes);
        serverInfor.token="";
     }
     QJsonDocument document_result = QJsonDocument::fromJson(result.toUtf8());
     QJsonArray result_array=document_result.object().value("child").toArray();
     for(int i=0;i<result_array.size();i++) {
         QString text=result_array.at(i).toObject().value("name").toString()+"("+result_array.at(i).toObject().value("code").toString()+")";
         QString data=QString::number(result_array.at(i).toObject().value("id").toInt());
         QList<QStandardItem *>  list;
         list<<new QStandardItem(result_array.at(i).toObject().value("salesOrderItem").toObject().value("productType_name").toString()+"("
                                 +result_array.at(i).toObject().value("salesOrderItem").toObject().value("productType_code").toString()+")")<<
               new QStandardItem(result_array.at(i).toObject().value("salesOrderItem").toObject().value("product_name").toString()+"("
                                 +result_array.at(i).toObject().value("salesOrderItem").toObject().value("product_code").toString()+")")<<
                new QStandardItem(result_array.at(i).toObject().value("salesOrderItem").toObject().value("batch").toString())<<
                new QStandardItem(result_array.at(i).toObject().value("state").toString())<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("sum").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("completed").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute1").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute2").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute3").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute4").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute5").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute6").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute7").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute8").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute9").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute10").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute11").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("attribute12").toInt()))<<
                new QStandardItem(QString::number(result_array.at(i).toObject().value("id").toInt()))<<
               new QStandardItem(result_array.at(i).toObject().value("salesOrderItem").toObject().value("product_id").toString());

         taskModel->appendRow(list);
      }


}

QString mainForm::readFromDrive(QString index){
    RequestMetaData data;
    data.type=getValue;
    data.from="1";
    data.target="manage";
    data.drive="FNAUC";
    data.index=index;
    emit sendMsgToManager(data);
    while(getValueResult==""){
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    QJsonDocument document=QJsonDocument::fromJson(getValueResult.toLocal8Bit().data());
    getValueResult="";
    if(document.object().value("result").toString()!="ok"){
        return "err";
    }
    else{
        if(document.object().value("value").isObject()){
            QJsonDocument docment3;
            docment3.setObject(document.object().value("value").toObject());
            QString json_str(docment3.toJson(QJsonDocument::Compact));
            return json_str;
        }
        else{
            return  document.object().value("result").toString();
        }
    }
}
QString mainForm::writeToDrive(QString index,QString value){
    RequestMetaData data;
    data.type=setValue;
    data.from="1";
    data.target="manage";
    data.drive="FANUC";
    data.index=index;
    data.value=value;
    emit sendMsgToManager(data);
    while(getValueResult==""){
         QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    QJsonDocument document=QJsonDocument::fromJson(getValueResult.toLocal8Bit().data());
    QJsonObject object=document.object();
    getValueResult="";
    if(object.value("result")=="err"){
        return "err";
    }
    return "ok";
}

/*加载信息*/
void mainForm::loadConfig()
{
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/control3.ini";
    QFile file(fileName);
   if (!file.open(QFile::ReadOnly)) {   //如果文件不存在则新建文件
       file.open( QIODevice::ReadWrite | QIODevice::Text );
      }
    QByteArray data=file.readAll();
    file.close();
    QJsonDocument doc=QJsonDocument::fromJson(data);
    QJsonObject server=doc.object().value("server").toObject();
    QJsonObject record=doc.object().value("record").toObject();
    serverInfor.name=server.value("name").toString();
    serverInfor.enable=server.value("enable").toBool();
    serverInfor.desc=server.value("desc").toString();
    serverInfor.username=server.value("username").toString();
    serverInfor.password=server.value("password").toString();
    serverInfor.address=server.value("address").toString();

}

