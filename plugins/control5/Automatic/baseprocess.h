#ifndef BASEPROCESS_H
#define BASEPROCESS_H

#include <QObject>
#include <QMetaEnum>
#include <QThread>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include "metaData.h"
#include "globalvariable.h"
class BaseProcess :public QThread
{
    Q_OBJECT
public:
    explicit BaseProcess(QObject *parent = nullptr);
    ~BaseProcess();
    enum  state
    {
        Unknown,          //未知状态(设备打开时\初始化过程中出现报警\急停按钮松开时)
        IsInitializing,   //初始化中(1.出现手动初始化动作时--设备处于暂停中\未知状态\报警状态 )
        IsPause,          //暂停中(1.初始化完成 2.设备处于运行中--出现手动停止时3.设备处于报警状态--出现报警清除时)
        IsRuning,         //运行中(出现手动运行时  设备处于暂停中)
        IsAlarm,          //报警中(设备运行中\暂停中  出现报警)
        IsEnding,         //结束中(当前流程结束进入收尾模式)
    };
     Q_ENUM(state)
    enum  variable
    {
        currentState,    //当前模块状态
        int0_step,       //初始化0状态步
        int1_step,       //初始化1状态步
        int2_step,       //初始化2状态步
        int3_step,       //初始化3状态步
        int4_step,       //初始化4状态步
        int5_step,       //初始化5状态步
        int6_step,       //初始化6状态步
        int7_step,       //初始化7状态步
        int8_step,       //初始化8状态步
        int9_step,       //初始化9状态步
        run0_step,       //运行0状态步
        run1_step,       //运行1状态步
        run2_step,       //运行2状态步
        run3_step,       //运行3状态步
        run4_step,       //运行4状态步
        run5_step,       //运行5状态步
        run6_step,       //运行6状态步
        run7_step,       //运行7状态步
        run8_step,       //运行8状态步
        run9_step,       //运行9状态步
    };
     Q_ENUM(variable)
     void run();
signals:
    void sendMessage(QString);
    void SendMsgToPluginInterface(QString request);
public slots:
    void startInit();
    void startRun();
    void startReset();
    void startAlarm();
    void startPause();
    void startStop();
protected:
    virtual void initAndReset();
    virtual void startAndRun();
    virtual void monitor();
    bool setValue(QString key,QString value);
    QString getValue(QString key);
    QString fileName="";
private:
    void saveConfig();                           //保存配置信息
    void loadConfig();                           //加载配置信息
    bool runFlag=true;
    QHash<QString,QString> m_value;
};

#endif // BASEPROCESS_H
