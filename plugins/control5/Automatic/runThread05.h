#ifndef RUNTHREAD05_H
#define RUNTHREAD05_H
#include <QThread>
#include <QObject>
#include <QDebug>
#include <metaData.h>
#include <QCoreApplication>
class RunThread05: public QThread
{
    Q_OBJECT
public:
    explicit RunThread05(QObject *parent = nullptr);
    ~RunThread05();
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

#endif // RUNTHREAD01_H
