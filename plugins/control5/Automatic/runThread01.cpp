#include "runThread01.h"

RunThread01::RunThread01(QObject *parent):BaseProcess(parent)
{
    fileName="thread01";
}
RunThread01::~RunThread01()
{

}

void RunThread01::initAndReset()
{
   //获取控制模式,只用在入库模式下才调用此模块
   QString mode_str=GlobalVariable::instance()->getValue("Mode01");
    if(mode_str=="Enter"){
        run_0();
        run_1();
        run_2();
        run_3();
    }
}

void RunThread01::startAndRun()
{

}

void RunThread01::monitor()
{

}


void RunThread01::init_0()
{
    int step=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::int0_step)).toInt();
    switch (step)
    {
        case 1://
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int0_step),"100");
            break;
        default:
            break;
    }
}
void RunThread01::init_1()
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
/*机械手上料*/
void RunThread01::run_0()
{
    int step=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step)).toInt();
    switch (step)
    {
        case 1://判断检测机是否有料
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step),"100");
            break;
        case 10://判断机械手是否空闲
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step),"100");
            break;
        case 20://控制机械手抓料
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step),"100");
            break;
        case 30://等待抓料完成信号
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step),"100");
            break;
        case 40://判断空料盘,并判断放料位置
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step),"100");
            break;
        case 50://控制机械手放料
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step),"100");
            break;
        case 60://等待放料完成
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step),"100");
            break;
        case 70://判断是否进行模式转换,
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step),"100");
            break;
        default:
         break;
   }
}
/*空料盘出库*/
void RunThread01::run_1()
{
    int step=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::run1_step)).toInt();
    switch (step)
    {
        case 1://出库位有空位
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run1_step),"100");
            break;
        case 10://查询空料盘库存
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run1_step),"100");
            break;
        case 20://判断堆垛机空闲
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run1_step),"100");
            break;
        case 30://发送出库请求
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run1_step),"100");
            break;
        case 40://等待出库完成,并做出库申请单
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run1_step),"100");
            break;
        case 50://判断是否进行模式转换
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run1_step),"100");
            break;
        default:
         break;
    }
}
/*空料盘流转*/
void RunThread01::run_2()
{
    int step=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::run2_step)).toInt();
    switch (step)
    {
        case 1://工作位
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run2_step),"100");
            break;
        case 10://判断机械手是否空闲
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run2_step),"100");
            break;
        case 20://控制机械手抓料
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run2_step),"100");
            break;
        case 30://等待抓料完成信号
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run2_step),"100");
            break;
        case 40://判断空料盘,并判断放料位置
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run2_step),"100");
            break;
        case 50://控制机械手放料
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run2_step),"100");
            break;
        case 60://等待放料完成
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run2_step),"100");
            break;
        case 70://判断是否进行模式转换
            setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run2_step),"100");
        default:
            break;
    }
}

/*入库控制*/
void RunThread01::run_3()
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

