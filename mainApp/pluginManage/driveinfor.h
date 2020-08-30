#ifndef DRIVEINFOR_H
#define DRIVEINFOR_H

#include <QObject>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>

#include "pluginmanage.h"

class DriveInfor : public QObject
{
    Q_OBJECT
public:
    explicit DriveInfor(QObject *parent = nullptr);
public:
    QString  getDrivesInfor();                                  //获取驱动信息
    QString  getDataSetInfor(QString drive);                    //获取指定驱动的数据集信息
    QString  getValue(QString drive,QString id);                //获取数值
    QString  setValue(QString drive,QString id,QString  value); //设置数值
private:
    QHash<QString, QString> m_drives;                       //驱动类型--插件名称对应

};

#endif // DRIVEINFOR_H
