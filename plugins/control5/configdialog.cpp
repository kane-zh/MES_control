#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->availableGeometry();  //可用区域
    this->resize(deskRect.width()/3,deskRect.height()/1.2);
    connect(ui->save,SIGNAL(clicked()),this,SLOT(saveConfig()));
    connect(ui->read,SIGNAL(clicked()),this,SLOT(readTest()));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}
/*窗体显示事件*/
void ConfigDialog::showEvent(QShowEvent *)
{
    dataModel=new QStandardItemModel(this); //建立数据表显示model实例
    m_delegate=new ItemDelegate(this);           //建立model 代理实例
    ui->tableView->setModel(dataModel);
    ui->tableView->setItemDelegateForColumn(1,m_delegate);
    ui->tableView->setItemDelegateForColumn(2,m_delegate);
    /*以下为获取驱动插件的信息*/
    RequestMetaData_dialog request;
    request.type="getDrivesInfor";
    driveInfor="";
    emit SendMsgToPluginInterface(request);      //获取驱动信息
    while(driveInfor==""){
     QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }
    QJsonDocument document = QJsonDocument::fromJson(driveInfor.toUtf8());
    QJsonObject json = document.object();
    QStringList list;
    list.clear();
    for(QJsonObject::Iterator it=json.begin();it!=json.end();it++)
    {
      list.append(it.key());
    }
    QJsonObject json_all;
    for(int i = 0; i< list.size();++i)
    {
        request.type="getDataSetInfor";
        request.drive=list.at(i);
        dateSetInfor="";
        emit SendMsgToPluginInterface(request);
        while(dateSetInfor==""){
            QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
        }
        document = QJsonDocument::fromJson(dateSetInfor.toUtf8());
        QJsonArray array= document.array();
        json_all.insert(list.at(i),array);
    }
      m_delegate->setDriveInfor(list);//将驱动插件的信息传递到代理
      m_delegate->setDataSetInfor(json_all);
      createActions();
}
/*从容器管理器接收消息(回调)*/
void ConfigDialog::receiveMsgFromPluginInterface(ResponseMetaData_dialog response)
{
    if(response.type=="getDrivesInfor"){
        driveInfor=response.value;
    }
    if(response.type=="getDataSetInfor"){
        dateSetInfor=response.value;
    }
    if(response.type=="getValue"){
        getValueResult=response.value;
    }
}
void ConfigDialog::contextMenuEvent(QContextMenuEvent *e)
{
    menu->exec (QCursor::pos ());
}
void ConfigDialog::createActions()
{
    dataModel->clear();
    loadConfig();          //加载配置
    QStringList list;
    list<<tr("数据点名称")<<tr("驱动")<<tr("数据集")<<tr("数据集索引")<<tr("描述")<<tr("读取值");
    dataModel->setHorizontalHeaderLabels(list);
    menu=new QMenu(this);
    add_row=new QAction(tr("添加行"),this);
    del_row=new QAction(tr("删除行"),this);
    menu->addAction(add_row);
    menu->addAction(del_row);
    connect (add_row,SIGNAL(triggered(bool)),this,SLOT(AddRow()));
    connect (del_row,SIGNAL(triggered(bool)),this,SLOT(RemoveRow()));
}

void ConfigDialog::RemoveRow()
{
    dataModel->removeRow(ui->tableView->currentIndex().row());
}

void ConfigDialog::AddRow()
{
    dataModel->insertRow(ui->tableView->currentIndex().row()+1);
}
void ConfigDialog::readTest()
{
    for (int i = 0; i < dataModel->rowCount(); i++)
     {
             RequestMetaData_dialog request;
             request.type="getValue";
             request.drive=dataModel->data(dataModel->index(i,1)).toString();
             request.id=dataModel->data(dataModel->index(i,3)).toString();
             getValueResult="";
             emit SendMsgToPluginInterface(request);
             while(getValueResult==""){
                  QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
             }
             QJsonDocument document=QJsonDocument::fromJson(getValueResult.toUtf8());
             QJsonObject object=document.object();
              if(document.object().value("result").toString()=="err"){
              QMessageBox::warning(this,tr("提示"),tr("获取数据")+dataModel->data(dataModel->index(i,0)).toString()+"失败："
                                   +document.object().value("value").toString(),QMessageBox::Yes);
               }
             dataModel->setData(dataModel->index(i,5),document.object().value("value").toString());
       }
     QMessageBox::information(this,tr("提示"),tr("读数据完成"),QMessageBox::Ok);
}

/*保存信息*/
void ConfigDialog::saveConfig()
{
    QJsonArray dataArray;
    for(int index=0;index<dataModel->rowCount();index++){
        QJsonObject json;
        json.insert("name",dataModel->data(dataModel->index(index,0)).toString());
        json.insert("drive",dataModel->data(dataModel->index(index,1)).toString());
        json.insert("dataName",dataModel->data(dataModel->index(index,2)).toString());
        json.insert("dataId",dataModel->data(dataModel->index(index,3)).toString());
        json.insert("desc",dataModel->data(dataModel->index(index,4)).toString());
        dataArray.push_back(json);
    }
    QJsonDocument document;
    document.setArray(dataArray);
    QByteArray data=document.toJson();
    QDir path = QDir(qApp->applicationDirPath());
    QString fileName=path.path()+"/plugins/config/control5.ini";
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly|QIODevice::Text)) { //如果文件不存在则新建文件
        file.open( QIODevice::ReadWrite | QIODevice::Text );
    }
    file.write(data);
    file.close ();
    QMessageBox::information(this,tr("提示"),tr("保存数据成功"),QMessageBox::Ok);
}
/*加载信息*/
void ConfigDialog::loadConfig()
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
        QJsonObject value = array.at(i).toObject();
        QList<QStandardItem *>  list;
        list<<new QStandardItem(value["name"].toString())<<
              new QStandardItem(value["drive"].toString())<<
              new QStandardItem(value["dataName"].toString())<<
              new QStandardItem(value["dataId"].toString())<<
              new QStandardItem(value["desc"].toString());;
        dataModel->appendRow(list);
     }
}
