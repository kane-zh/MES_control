#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H
#include <QDir>
#include <QFile>
#include <QObject>
#include <QMetaEnum>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>
#include <QHash>
#include <QSharedPointer>
#include "metaData.h"
class GlobalVariable:public QObject
{
   Q_OBJECT
public:
    static QSharedPointer<GlobalVariable>& instance()
    {
        QMutexLocker mutexLocker(&m_mutex);
        if (m_instance.isNull())
        {
            m_instance = QSharedPointer<GlobalVariable>(new GlobalVariable());
        }
        return m_instance;
    }
public:
    enum  variable
    {
        currentState,    //当前模块状态
    };
     Q_ENUM(variable)
private:
    GlobalVariable(){}
    GlobalVariable(const GlobalVariable&){}
    GlobalVariable& operator==(const GlobalVariable&){}
private:
    static QMutex m_mutex;
    static QSharedPointer<GlobalVariable> m_instance;
private:
    QHash<QString,QString> m_value;
    QHash<QString,QString> m_data;
public:
    bool setValue(QString key,QString value);
    QString getValue(QString key);
    bool setData(QString key,QString value);
    QString getData(QString key);
};


#endif // GLOBALVARIABLE_H

