#include "globalvariable.h"
#include <QDebug>
QMutex GlobalVariable::m_mutex;
QSharedPointer<GlobalVariable> GlobalVariable::m_instance;

bool GlobalVariable::setValue(QString key,QString value)
{
    m_value.insert(key,value);
    return true;
}

QString GlobalVariable::getValue(QString key)
{
    return m_value.value(key);
}
