#ifndef BASEPROCESS_H
#define BASEPROCESS_H
#include <QObject>
#include <QMetaEnum>

class BaseProcess: public QObject
{
    Q_OBJECT
public:
    BaseProcess();
    enum  state
    {
        Unknown,          //未知状态(设备打开时\初始化过程中出现报警\急停按钮松开时)
        IsInitializing,   //初始化中(1.出现手动初始化动作时--设备处于暂停中\未知状态\报警状态 )
        IsPause,          //暂停中(1.初始化完成 2.设备处于运行中--出现手动停止时3.设备处于报警状态--出现报警清除时)
        IsRuning,         //运行中(出现手动运行时  设备处于暂停中)
        IsAlarm,          //报警中(设备运行中\暂停中  出现报警)
        IsScraming,       //急停中（出现手动急停信号按下)
    };
     Q_ENUM(state)
};

#endif // BASEPROCESS_H
