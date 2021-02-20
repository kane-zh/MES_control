#ifndef RUNTHREAD01_H
#define RUNTHREAD01_H
#include "baseprocess.h"
class RunThread01: public BaseProcess
{
    Q_OBJECT
public:
    explicit RunThread01(QObject *parent = nullptr);
    ~RunThread01();
protected:
    virtual void initAndReset();
    virtual void startAndRun();
    virtual void monitor();
private:
    void init_0();
    void init_1();
    void run_0();
    void run_1();
    void run_2();
    void run_3();
};

#endif // RUNTHREAD01_H
