#include "mainform.h"
#include "ui_mainform.h"

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

void mainForm::receiveMsgFromManager(ResponseMetaData response)
{

}
