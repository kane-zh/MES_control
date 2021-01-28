#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCborError>
#include <QDir>
#include <QMenu>
#include <QtConcurrent>
#include "ItemDelegate.h"
#include "metaData.h"
#include "QDesktopWidget"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = nullptr);
    ~ConfigDialog();
    void showEvent(QShowEvent *) ;
    void contextMenuEvent(QContextMenuEvent* e);
signals:
    void SendMsgToPluginInterface(RequestMetaData_dialog request);
public slots:
    void receiveMsgFromPluginInterface(ResponseMetaData_dialog response); //从ContainerManage接收消息
    void autosave();
private slots:
    void readTest();      //读数据测试
    void createActions();//创建右键菜单
    void RemoveRow();//移除选中行
    void AddRow();   //在选中行下添加行
    void saveConfig();                           //保存配置信息
    void loadConfig();                           //加载配置信息
private:
    QStandardItemModel    *dataModel;
    ItemDelegate *m_delegate;
    QMenu                 *menu;
    QAction               *add_row;
    QAction               *del_row;
    QString  driveInfor="";
    QString  dateSetInfor="";
    QString  getValueResult="";
    Ui::ConfigDialog *ui;
};

#endif // CONFIGDIALOG_H
