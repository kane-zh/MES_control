#ifndef OPERATIONHISTROYDATA_H
#define OPERATIONHISTROYDATA_H

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "Fwlib32.h"
#include "metadata.h"
#include <QtCore/qmath.h>

class operationHistroyData
{
public:
    operationHistroyData();
    QString getValue(DATASET dataSetInfor);
private:
    QString CNC_rdophistry(DATASET dataSetInfor);
    QString CNC_rdalmhistry(DATASET dataSetInfor);
};

#endif // OPERATIONHISTROYDATA_H
