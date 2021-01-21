#pragma once

#include <QWidget>
#include "../../mainApp/pluginManage/debugcontrolinterface.h"
namespace Ui {
class mainForm;
}

class mainForm : public QWidget,public DebugControlInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID InterfacePlugin_iid FILE "programmer.json")
    Q_INTERFACES(DebugControlInterface)
public:
    explicit mainForm(QWidget *parent = nullptr);
    ~mainForm();
signals:
    virtual void sendMsgToPluginManager(RequestMetaData request) Q_DECL_OVERRIDE;
public:
    virtual void receiveMsgFromPluginManager(ResponseMetaData response) Q_DECL_OVERRIDE;

private:
    Ui::mainForm *ui;
};

