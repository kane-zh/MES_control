#ifndef RUNTHREAD02_H
#define RUNTHREAD02_H
#include "baseprocess.h"
class RunThread02: public BaseProcess
{
    Q_OBJECT
public:
    explicit RunThread02(QObject *parent = nullptr);
    ~RunThread02();
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

#endif // RUNTHREAD02_H
