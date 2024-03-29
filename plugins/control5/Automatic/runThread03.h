#ifndef RUNTHREAD03_H
#define RUNTHREAD03_H
#include "baseprocess.h"
class RunThread03: public BaseProcess
{
    Q_OBJECT
public:
    explicit RunThread03(QObject *parent = nullptr);
    ~RunThread03();
protected:
    virtual void initAndReset();
    virtual void startAndRun();
    virtual void monitor();
private:
    void init_0();
    void init_1();
    void run_0();
    void run_1();
};

#endif // RUNTHREAD03_H
