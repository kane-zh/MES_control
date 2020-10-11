#ifndef OTHERINFOR_H
#define OTHERINFOR_H
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "Fwlib32.h"
#include "metadata.h"
#include <QtCore/qmath.h>

class otherInfor
{
public:
    otherInfor();
    QString getValue(DATASET dataSetInfor);
private:
    QString CNC_sysinfo(DATASET dataSetInfor);
    QString CNC_statinfo(DATASET dataSetInfor);
    QString CNC_rdopmsg(DATASET dataSetInfor);
    QString CNC_alarm(DATASET dataSetInfor);
    QString CNC_rdalmmsg(DATASET dataSetInfor);
    QString CNC_getdtailerr(DATASET dataSetInfor);
    QString CNC_rddiaginfor(DATASET dataSetInfor);
    QString CNC_rdsyshard(DATASET dataSetInfor);
};

#endif // OTHERINFOR_H
