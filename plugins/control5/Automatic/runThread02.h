#ifndef RUNTHREAD02_H
#define RUNTHREAD02_H
#include <QThread>
#include <QObject>
#include <QDebug>
#include <metaData.h>
#include <QCoreApplication>
class RunThread02: public QThread
{
    Q_OBJECT
public:
    explicit RunThread02(QObject *parent = nullptr);
    ~RunThread02();
    void run();
signals:
    void sendMessage(QString);
    void SendMsgToPluginInterface(QString request);
public slots:
    void startInit();
    void startRun();
    void startRestart();
    void startAlarm();
    void startPause();
    void StartStop();
private:
    void initAndReset();
    void startAndRun();
    void init_0();
    void init_1();
    void run_0();
    void run_1();
private:
    bool runFlag=true;
};

#endif // RUNTHREAD02_H
