#include "runThread05.h"

RunThread05::RunThread05(QObject *parent):QThread(parent)
{

}
RunThread05::~RunThread05()
{
    runFlag=false;
    requestInterruption();
    quit();
    wait();
}
void RunThread05::run()
{
    while(runFlag)
    {
    /*依次调用各部分功能程序，执行哪一步由各功能程序自行判断，但要求各功能程序必须只执行一步并立即
    返回。不允许功能程序卡死*/
      initAndReset();
      startAndRun();
      msleep(50);
    }
}

//初始化启动
void RunThread05::startInit()
{
//    if ((currentState == BaseAutoProcess.state.Unknown)||
//        (currentState == BaseAutoProcess.state.IsPause)||
//        (currentState == BaseAutoProcess.state.IsAlarm)
//        )
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            homeStep[i] = 1;
//        }
//        for (int i = 0; i < 10; i++)
//        {
//            iniStep[i] = 1;
//        }
//        for (int i = 0; i < 10; i++)
//        {
//            runStep[i] = 1;
//        }
//        currentState = BaseAutoProcess.state.IsInitializing;
//        msgInvoker("初始化开始", true);
//    }
}
//运行启动
void RunThread05::startRun()
{
//    if ((currentState == BaseAutoProcess.state.IsPause))
//    {
//        currentState = BaseAutoProcess.state.IsRuning;
//        msgInvoker("设备运行", true);
//    }
}
//重启启动
void RunThread05::startRestart()
{
//    if (currentState == BaseAutoProcess.state.IsScraming)
//    {
//        for (int i = 0; i < 16; i++)
//        {
//            homeStep[i] = 1;
//        }
//        for (int i = 0; i < 5; i++)
//        {
//            iniStep[i] = 1;
//        }
//        for (int i = 0; i < 5; i++)
//        {
//            runStep[i] = 1;
//        }
//        currentState = BaseAutoProcess.state.Unknown;
//       // msgInvoker("设备上电", true);
//    }
}
//报警启动
void RunThread05::startAlarm()
{
//    if ((currentState != BaseAutoProcess.state.Unknown)&&
//        (currentState != BaseAutoProcess.state.IsScraming))
//    {
//        currentState = BaseAutoProcess.state.IsAlarm;
//        msgInvoker("设备报警", true);
//    }
}
//暂停流程
void RunThread05::startPause()
{
//    if ((currentState == BaseAutoProcess.state.IsRuning))
//    {
//        _axesControl.PauseAxis(0, 1);
//        currentState = BaseAutoProcess.state.IsPause;
//        msgInvoker("设备暂停", true);
//    }
}
void RunThread05::initAndReset()
 {
     init_0();
     init_1();
 }

void RunThread05::startAndRun()
{
     run_0();
     run_1();

}

void RunThread05::init_0()
{
    int step=0;
    switch (step)
    {
        case 1:
            step = 1000;
            break;
        default:
            break;
    }
}
void RunThread05::init_1()
{
    int step=0;
    switch (step)
    {
        case 1: //
            step = 1000;
            break;
         default:
            break;
    }
}
void RunThread05::run_0()
{
    int step=0;
    switch (step)
    {
        case 1://
            break;
        default:
            break;
   }
}
void RunThread05::run_1()
{
    int step=0;
    switch (step)
    {
        case 1:

            break;
        default:
            break;
    }
}
