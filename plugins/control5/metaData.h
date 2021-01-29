#ifndef  METADATA_H
#define  METADATA_H
#include <QString>

struct RequestMetaData_dialog
{
    QString type;
    QString drive;
    QString id;
    QString value;
};
struct ResponseMetaData_dialog
{
    QString type;
    QString value;
};
/*数据点信息*/
struct  DATAPOINT
{
    QString name;
    QString drive;
    QString dataName;
    QString dataId;
    long    errCount;
};
#endif // METADATA_H
