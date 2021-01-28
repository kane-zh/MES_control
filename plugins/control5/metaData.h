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
/*记录信息*/
struct  RECORDINFOR
{
    QString type;
    QString drive;
    QString id;
    QString value;

 };
#endif // METADATA_H
