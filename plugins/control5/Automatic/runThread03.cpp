#include "runThread03.h"

RunThread03::RunThread03(QObject *parent):QThread(parent)
{

}
RunThread03::~RunThread03()
{
    runFlag=false;
    requestInterruption();
    quit();
    wait();
}
void RunThread03::run()
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
void RunThread03::startInit()
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
void RunThread03::startRun()
{
//    if ((currentState == BaseAutoProcess.state.IsPause))
//    {
//        currentState = BaseAutoProcess.state.IsRuning;
//        msgInvoker("设备运行", true);
//    }
}
//重启启动
void RunThread03::startRestart()
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
void RunThread03::startAlarm()
{
//    if ((currentState != BaseAutoProcess.state.Unknown)&&
//        (currentState != BaseAutoProcess.state.IsScraming))
//    {
//        currentState = BaseAutoProcess.state.IsAlarm;
//        msgInvoker("设备报警", true);
//    }
}
//暂停流程
void RunThread03::startPause()
{
//    if ((currentState == BaseAutoProcess.state.IsRuning))
//    {
//        _axesControl.PauseAxis(0, 1);
//        currentState = BaseAutoProcess.state.IsPause;
//        msgInvoker("设备暂停", true);
//    }
}
void RunThread03::initAndReset()
 {
     init_0();
     init_1();
 }

void RunThread03::startAndRun()
{
     run_0();
     run_1();

}

void RunThread03::init_0()
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
void RunThread03::init_1()
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
void RunThread03::run_0()
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
void RunThread03::run_1()
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
