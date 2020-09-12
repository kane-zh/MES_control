#include "mainform.h"
#include "ui_mainform.h"
#include <QQuickWidget>
mainForm::mainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainForm)
{
    ui->setupUi(this);

}

mainForm::~mainForm()
{
    delete ui;
}

/*窗体显示事件*/
void mainForm::showEvent(QShowEvent *)
{
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView );
    ui->quickWidget->setSource(QUrl("qrc:/main.qml"));
    ui->quickWidget->rootObject();
}
void mainForm::receiveMsgFromManager(ResponseMetaData response)
{

}
