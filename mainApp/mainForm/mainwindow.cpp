#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDesktopWidget"
#include "QMessageBox"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
//    setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::WindowTitleHint);
    PluginManage::getInstance().loadAllPlugins();
    addPluginActions();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
}

/*根据加载的插件动态添加菜单action*/
void MainWindow::addPluginActions()
{
    foreach (QFileInfo info,PluginManage::getInstance().m_loaders.keys())
    {
      QAction *action=new QAction();
      action->setText(PluginManage::getInstance().m_title.value(info.absoluteFilePath()).toString());
      action->setObjectName(info.absoluteFilePath());//将插件字典键值保存到action的ObjectName,方便向槽传递数值
      connect(action,SIGNAL(triggered(bool)),this,SLOT(showPluginForm()));
      if(PluginManage::getInstance().m_types.value(info.absoluteFilePath())=="dataSource"){
        ui->dataSource->addAction(action);
      }
      if(PluginManage::getInstance().m_types.value(info.absoluteFilePath())=="container"){
        ui->container->addAction(action);
      }
      if(PluginManage::getInstance().m_types.value(info.absoluteFilePath())=="autoControl" ||
         PluginManage::getInstance().m_types.value(info.absoluteFilePath())=="debugControl"){
        ui->control->addAction(action);
      }
    }
}

/*根据动态菜单信号，显示相应插件窗体*/
void MainWindow::showPluginForm()
{
    QList<QObject*> list=ui->centralWidget->children(); //获取桌面所有子窗口，并销毁。确保桌面只有唯一子窗口
     if(list.count()>1){
         for(int i=1;i<list.count();i++){
             list.at(i)->deleteLater();
         }
      }
      QAction  *action= qobject_cast<QAction *>(sender());//获取信号发出者,并获取其ObjectName还原键值
      QPluginLoader *loader = PluginManage::getInstance().m_loaders.value(action->objectName()); //通过键值获取保存的插件loader
      auto obj = loader->instance();
      QString type=PluginManage::getInstance().m_types.value(action->objectName());
      if(type=="dataSource")
      {
          DriveInterface *plugin = qobject_cast<DriveInterface *>(obj);
          plugin->showForm(ui->centralWidget);
      }
      else if(type=="container"){
            ContainerInterface *plugin = qobject_cast<ContainerInterface *>(obj);
            plugin->showForm(ui->centralWidget);
      }
      else if(type=="autoControl"){
            AutoControlInterface *plugin = qobject_cast<AutoControlInterface *>(obj);
            plugin->showForm(ui->centralWidget);
      }
      else if(type=="debugControl"){
               QWidget *widget = qobject_cast<QWidget*>(obj);
               widget->setParent(ui->centralWidget);
               ui->gridLayout->addWidget(widget);
               widget->show();
      }
}

