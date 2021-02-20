#ifndef RUNTHREAD05_H
#define RUNTHREAD05_H
#include "baseprocess.h"
class RunThread05: public BaseProcess
{
    Q_OBJECT
public:
    explicit RunThread05(QObject *parent = nullptr);
    ~RunThread05();
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

#endif // RUNTHREAD05_H
