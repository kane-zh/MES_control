#include "baseprocess.h"

BaseProcess::BaseProcess(QObject *parent)
{
    loadConfig();
    this->start();
}
BaseProcess::~BaseProcess()
{
  runFlag=false;
  requestInterruption();
  quit();
  wait();
}
void BaseProcess::run()
{
    while(runFlag)
    {
        /*根据条件以此状态机，执行哪一步由状态机内部自行判断，但要求各状态机必须只执行一步并立即返回。不允许人为程序卡死*/
        QString currentState_str=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState));
        state currentState = (state)QMetaEnum::fromType<state>().keyToValue(currentState_str.toUtf8());
         /*运行或暂停状态下进入监控状态机*/
        if(currentState==state::IsRuning||currentState==state::IsPause||currentState==state::IsEnding){
          monitor();
        }
        /*初始化状态下进入初始化状态机*/
        if(currentState==state::IsInitializing){
          initAndReset();
        }
        /*运行状态下进入运行状态机*/
        if(currentState==state::IsRuning){
          startAndRun();
        }
       msleep(500);
     }
}
/*初始化流程(虚函数,需在子类中重构)*/
void BaseProcess::initAndReset()
 {
 }
/*运行流程(虚函数,需在子类中重构)*/
void BaseProcess::startAndRun()
{
}
/*监控流程(虚函数,需在子类中重构)*/
void BaseProcess::monitor()
{

}
//初始化启动
void BaseProcess::startInit()
{
    QString currentState_str=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState));
    state currentState = (state)QMetaEnum::fromType<state>().keyToValue(currentState_str.toUtf8());
    if (currentState == state::Unknown)
    {
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int0_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int1_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int2_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int3_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int4_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int5_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int6_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int7_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int8_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::int9_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run0_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run1_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run2_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run3_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run4_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run5_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run6_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run7_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run8_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::run9_step),"1");
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState),
                 QMetaEnum::fromType<state>().valueToKey(state::IsInitializing));
        emit sendMessage("初始化开始");
    }
}
//运行启动
void BaseProcess::startRun()
{
    QString currentState_str=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState));
    state currentState = (state)QMetaEnum::fromType<state>().keyToValue(currentState_str.toUtf8());
    if ((currentState == state::IsPause||currentState == state::IsEnding))
    {
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState),
                 QMetaEnum::fromType<state>().valueToKey(state::IsRuning));
        emit sendMessage("设备运行");
    }
}
//暂停流程
void BaseProcess::startPause()
{
    QString currentState_str=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState));
    state currentState = (state)QMetaEnum::fromType<state>().keyToValue(currentState_str.toUtf8());
    if ((currentState == state::IsRuning||currentState == state::IsEnding))
    {
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState),
                 QMetaEnum::fromType<state>().valueToKey(state::IsPause));
        emit sendMessage("设备暂停");
    }
}
//停止流程
void BaseProcess::startStop()
{
    QString currentState_str=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState));
    state currentState = (state)QMetaEnum::fromType<state>().keyToValue(currentState_str.toUtf8());
    if ((currentState == state::IsRuning||currentState == state::IsPause))
    {
        setValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState),
                 QMetaEnum::fromType<state>().valueToKey(state::IsEnding));
        emit sendMessage("设备开始收尾");
    }
}
//报警复位
void BaseProcess::startReset()
{
    QString currentState_str=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState));
    state currentState = (state)QMetaEnum::fromType<state>().keyToValue(currentState_str.toUtf8());
    if (currentState == state::IsAlarm)
    {
          setValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState),
                   QMetaEnum::fromType<state>().valueToKey(state::IsPause));
          emit sendMessage("设备重启");
    }
}
//报警启动
void BaseProcess::startAlarm()
{
    QString currentState_str=getValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState));
    state currentState = (state)QMetaEnum::fromType<state>().keyToValue(currentState_str.toUtf8());
    if ((currentState != state::IsRuning)&&
        (currentState != state::IsPause))
    {
          setValue(QMetaEnum::fromType<variable>().valueToKey(variable::currentState),
                   QMetaEnum::fromType<state>().valueToKey(state::IsAlarm));
          emit sendMessage("设备报警");
    }
}
bool BaseProcess::setValue(QString key, QString value)
{
    m_value.insert(key,value);
    return true;
}

QString BaseProcess::getValue(QString key)
{
    return m_value.value(key);
}

void BaseProcess::saveConfig()
{
    for(int i=0;m_value.keys().count();i++)
    {

    }
}

void BaseProcess::loadConfig()
{

}
