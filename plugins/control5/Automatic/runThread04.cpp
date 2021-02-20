#include "runThread04.h"

RunThread04::RunThread04(QObject *parent):BaseProcess(parent)
{
   fileName="thread04";
}
RunThread04::~RunThread04()
{

}
void RunThread04::initAndReset()
{
   //获取控制模式,只用在入库模式下才调用此模块
   QString mode_str=GlobalVariable::instance()->getValue("Mode04");
    if(mode_str=="Enter"){
     run_0();
     run_1();
    }
}

void RunThread04::startAndRun()
{

}

void RunThread04::monitor()
{

}


void RunThread04::init_0()
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
void RunThread04::init_1()
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
void RunThread04::run_0()
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
void RunThread04::run_1()
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

