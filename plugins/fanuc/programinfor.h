#ifndef PROGRAMINFOR_H
#define PROGRAMINFOR_H
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "Fwlib32.h"
#include "metadata.h"
#include <QtCore/qmath.h>

class programInfor
{
public:
    programInfor();
    QString getValue(DATASET dataSetInfor);
private:
    QString CNC_rdprgnum(DATASET dataSetInfor);
    QString CNC_exeprgname(DATASET dataSetInfor);
};

#endif // PROGRAMINFOR_H
