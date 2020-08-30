#pragma once

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QString>
/**
 * 插件间消息封装体
 **/
enum TYPE{
    getDrivesInfor,
    getDataSetInfor,
    getValue,
    setValue
};
struct RequestMetaData
{
    QString from;
    QString target;
    TYPE    type;
    QString drive;
    QString index;
    QString value;
};
struct ResponseMetaData
{
    QString from;
    QString target;
    TYPE    type;
    QString value;
};
Q_DECLARE_METATYPE(RequestMetaData);
Q_DECLARE_METATYPE(ResponseMetaData);
