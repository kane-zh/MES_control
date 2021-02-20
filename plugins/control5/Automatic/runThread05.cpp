#include "runThread05.h"

RunThread05::RunThread05(QObject *parent):BaseProcess(parent)
{
   fileName="thread05";
}
RunThread05::~RunThread05()
{

}
void RunThread05::initAndReset()
{
   //获取控制模式,只用在入库模式下才调用此模块
   QString mode_str=GlobalVariable::instance()->getValue("Mode05");
    if(mode_str=="Enter"){
     run_0();
     run_1();
    }
}

void RunThread05::startAndRun()
{

}

void RunThread05::monitor()
{

}


void RunThread05::init_0()
{
    int step=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::int0_step)).toInt();
    switch (step)
    {
        case 1:
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int0_step),"100");
            break;
        default:
            break;
    }
}
void RunThread05::init_1()
{
    int step=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::int1_step)).toInt();
    switch (step)
    {
        case 1: //
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int1_step),"100");
            break;
         default:
            break;
    }
}
void RunThread05::run_0()
{
    int step=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step)).toInt();
    switch (step)
    {
        case 1://
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step),"100");
            break;
        default:
            break;
   }
}
void RunThread05::run_1()
{
    int step=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::run1_step)).toInt();
    switch (step)
    {
        case 1:
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run1_step),"100");
            break;
        default:
            break;
    }
}

