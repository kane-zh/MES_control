#include "mainform.h"
#include "ui_mainform.h"
#include <QQuickWidget>
mainForm::mainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainForm)
{
    ui->setupUi(this);
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView );
    ui->quickWidget->setSource(QUrl("qrc:/PlanetsMain.qml"));
    ui->quickWidget->rootObject();
}

mainForm::~mainForm()
{
    delete ui;
}

void mainForm::receiveMsgFromPluginManager(ResponseMetaData response)
{

}
