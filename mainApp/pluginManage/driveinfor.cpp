#include "driveinfor.h"


DriveInfor::DriveInfor(QObject *parent) : QObject(parent)
{
    m_drives.insert("OPCUA", "opcUA");
    m_drives.insert("MODBUS", "modbusRTU");
    m_drives.insert("MODBUSTCP", "modbusTCP");
//    m_drives.insert("MQTT", "mqtt");
    m_drives.insert("KEYENCE", "keyence");
}

/*获取驱动信息*/
QString DriveInfor::getDrivesInfor()
{
    QJsonObject json;
    foreach (QString key, m_drives.keys()) {
        json.insert(key,key);
    }
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    return json_str;
}
/*获取指定驱动下的数据集信息*/
QString DriveInfor::getDataSetInfor(QString drive)
{
    QString result="";
    QString name=m_drives.value(drive);
    if(name==""){
         QJsonObject json;
         json.insert("result", "不存在指定的驱动类型");
         QJsonDocument document;
         document.setObject(json);
         QByteArray byte_array = document.toJson(QJsonDocument::Compact);
         QString result(byte_array);
         return result;
    }
    QString path=PluginManage::getInstance().m_names.key(name);
    if(path==""){
         QJsonObject json;
         json.insert("result", "指定的驱动插件不存在");
         QJsonDocument document;
         document.setObject(json);
         QByteArray byte_array = document.toJson(QJsonDocument::Compact);
         QString result(byte_array);
         return result;
    }
    QPluginLoader *loader = PluginManage::getInstance().m_loaders.value(path); //通过键值获取保存的插件loader
    DriveInterface *plugin = qobject_cast<DriveInterface *>(loader->instance());
    result=plugin->getDataSetInfor();
    return result;
}

/*获取指定驱动下的指定数据集的数据*/
QString DriveInfor::getValue(QString drive, QString id)
{
    QString result="";
    QString name=m_drives.value(drive);
    if(name==""){
         QJsonObject json;
         json.insert("result", "不存在指定的驱动类型");
         QJsonDocument document;
         document.setObject(json);
         QByteArray byte_array = document.toJson(QJsonDocument::Compact);
         QString result(byte_array);
         return result;
    }
    QString path=PluginManage::getInstance().m_names.key(name);
    if(path==""){
         QJsonObject json;
         json.insert("result", "指定的驱动插件不存在");
         QJsonDocument document;
         document.setObject(json);
         QByteArray byte_array = document.toJson(QJsonDocument::Compact);
         QString result(byte_array);
         return result;
    }
    QPluginLoader *loader = PluginManage::getInstance().m_loaders.value(path); //通过键值获取保存的插件loader
    DriveInterface *plugin = qobject_cast<DriveInterface *>(loader->instance());
    result=plugin->getValue(id);
    return result;
}

/*设置指定驱动下的指定数据集的数据*/
QString DriveInfor::setValue(QString drive, QString id, QString value)
{
    QString result="";
    QString name=m_drives.value(drive);
    if(name==""){
         QJsonObject json;
         json.insert("result", "不存在指定的驱动类型");
         QJsonDocument document;
         document.setObject(json);
         QByteArray byte_array = document.toJson(QJsonDocument::Compact);
         QString result(byte_array);
         return result;
    }
    QString path=PluginManage::getInstance().m_names.key(name);
    if(path==""){
         QJsonObject json;
         json.insert("result", "指定的驱动插件不存在");
         QJsonDocument document;
         document.setObject(json);
         QByteArray byte_array = document.toJson(QJsonDocument::Compact);
         QString result(byte_array);
         return result;
    }
    QPluginLoader *loader = PluginManage::getInstance().m_loaders.value(path); //通过键值获取保存的插件loader
    DriveInterface *plugin = qobject_cast<DriveInterface *>(loader->instance());
    result=plugin->setValue(id,value);
    return result;
}

