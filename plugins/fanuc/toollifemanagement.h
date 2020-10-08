#ifndef TOOLLIFEMANAGEMENT_H
#define TOOLLIFEMANAGEMENT_H
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "Fwlib32.h"
#include "metadata.h"
#include <QtCore/qmath.h>

class toolLifeManagement
{
public:
    toolLifeManagement();
    QString getValue(DATASET dataSetInfor);
private:
    QString CNC_rdgrpid(DATASET dataSetInfor);
    QString CNC_rdntool(DATASET dataSetInfor);
    QString CNC_rdngrp(DATASET dataSetInfor);
    QString CNC_rdlife(DATASET dataSetInfor);
    QString CNC_rdcount(DATASET dataSetInfor);
    QString CNC_rdtlinfo(DATASET dataSetInfor);
    QString CNC_rdmaxgrp(DATASET dataSetInfor);
    QString CNC_rdtlusegrp(DATASET dataSetInfor);
    QString CNC_rdtlgrp(DATASET dataSetInfor);
    QString CNC_rdmaxtool(DATASET dataSetInfor);
    QString CNC_rdusetlno(DATASET dataSetInfor);
    QString CNC_rdtltool(DATASET dataSetInfor);
    QString CNC_rdlength(DATASET dataSetInfor);
    QString CNC_rdradius(DATASET dataSetInfor);
    QString CNC_rdtoolinfo(DATASET dataSetInfor);
    QString CNC_toolnum(DATASET dataSetInfor);
    QString CNC_rdtoolgrp(DATASET dataSetInfor);
};

#endif // TOOLLIFEMANAGEMENT_H

