#ifndef AXISINFOR_H
#define AXISINFOR_H
#include <QDebug>
#include "metadata.h"

class axisInfor
{
public:
    axisInfor();
    QString getValue(unsigned short handle,DATASET dataSetInfor);
private:
    QString cnc_actf(unsigned short handle,DATASET dataSetInfor);
};

#endif // AXISINFOR_H
