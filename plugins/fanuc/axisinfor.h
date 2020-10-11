#ifndef AXISINFOR_H
#define AXISINFOR_H
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "Fwlib32.h"
#include "metadata.h"
#include <QtCore/qmath.h>
class axisInfor
{
public:
    axisInfor();
    QString getValue(DATASET dataSetInfor);
private:
    QString CNC_actf(DATASET dataSetInfor);
    QString CNC_absolute(DATASET dataSetInfor);
    QString CNC_machine(DATASET dataSetInfor);
    QString CNC_relative(DATASET dataSetInfor);
    QString CNC_distance(DATASET dataSetInfor);
    QString CNC_skip(DATASET dataSetInfor);
    QString CNC_srvdelay(DATASET dataSetInfor);
    QString CNC_accdecdly(DATASET dataSetInfor);
    QString CNC_acts(DATASET dataSetInfor);
    QString CNC_rdspload(DATASET dataSetInfor);
    QString CNC_rdspmaxrpm(DATASET dataSetInfor);
    QString CNC_rdspgear(DATASET dataSetInfor);
    QString CNC_rdposition(DATASET dataSetInfor);
    QString CNC_feedrate(DATASET dataSetInfor);
    QString CNC_rdsvmeter(DATASET dataSetInfor);
    QString CNC_acts2(DATASET dataSetInfor);
    QString CNC_rdspmeter(DATASET dataSetInfor);
    QString CNC_rdmovrlapm(DATASET dataSetInfor);
    QString CNC_rdjogdrun(DATASET dataSetInfor);
    QString CNC_rdaxisname(DATASET dataSetInfor);
    QString CNC_rdhndintrpt(DATASET dataSetInfor);
    QString CNC_rdnspdl(DATASET dataSetInfor);
    QString CNC_rdloopgain(DATASET dataSetInfor);
    QString CNC_rdcurrent(DATASET dataSetInfor);
    QString CNC_rdsrvspeed(DATASET dataSetInfor);
    QString CNC_rdmovestate(DATASET dataSetInfor);
    QString CNC_rdspcss(DATASET dataSetInfor);
    QString CNC_rdspdlname(DATASET dataSetInfor);
    QString CNC_rdspdlalm(DATASET dataSetInfor);
    QString CNC_axisnum(DATASET dataSetInfor);
    QString CNC_rdsspeed(DATASET dataSetInfor);
};

#endif // AXISINFOR_H
