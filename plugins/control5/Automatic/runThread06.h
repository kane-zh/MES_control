#ifndef RUNTHREAD06_H
#define RUNTHREAD06_H
#include "baseprocess.h"
class RunThread06: public BaseProcess
{
    Q_OBJECT
public:
    explicit RunThread06(QObject *parent = nullptr);
    ~RunThread06();
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

#endif // RUNTHREAD06_H
