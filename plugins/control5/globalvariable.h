#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMutex>
#include <QHash>
#include <QSharedPointer>
#include "metaData.h"
//class GlobalVariable
//{
//protected:
//    GlobalVariable(){}
//    virtual ~GlobalVariable(){}

//public:
//    static GlobalVariable * getInstance()
//    {
//        if(m_instance == NULL)
//        {
//            m_mutex.lock();
//            if (m_instance == NULL)
//            {
//                m_instance = new  GlobalVariable();
//            }
//            m_mutex.unlock();
//        }

//        return m_instance;
//    }

//    //摧毁单例
//    static void deleteInstance()
//    {
//        if(m_instance != NULL)
//        {
//            delete m_instance;
//            m_instance = NULL;
//        }
//    }
//public:
//    bool StartMonitor();
//    bool SetValue(QString key,QString value);
//    QString GetValue(QString key);
//private:
//    static GlobalVariable * m_instance;
//    static QMutex m_mutex;    //加入互斥锁,保证线程安全(如果存在多线程)
//private:
//    QHash<QString,DATAPOINT> m_datapoint;
//    QHash<QString,QString> m_value;
//};
class GlobalVariable
{
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
private:
    GlobalVariable(){}
    GlobalVariable(const GlobalVariable&){}
    GlobalVariable& operator==(const GlobalVariable&){}
private:
    static QMutex m_mutex;
    static QSharedPointer<GlobalVariable> m_instance;
private:
    QHash<QString,QString> m_value;
public:
    bool setValue(QString key,QString value);
    QString getValue(QString key);
};


#endif // GLOBALVARIABLE_H

