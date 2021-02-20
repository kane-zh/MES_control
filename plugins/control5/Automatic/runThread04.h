#ifndef RUNTHREAD04_H
#define RUNTHREAD04_H
#include "baseprocess.h"
class RunThread04: public BaseProcess
{
    Q_OBJECT
public:
    explicit RunThread04(QObject *parent = nullptr);
    ~RunThread04();
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

#endif // RUNTHREAD04_H
