#include "programinfor.h"

programInfor::programInfor()
{

}

QString programInfor::getValue(DATASET dataSetInfor)
{
    if(dataSetInfor.function=="读取程序号"){
        return CNC_rdprgnum(dataSetInfor);
    }
    if(dataSetInfor.function=="读取程序名称"){
        return CNC_exeprgname(dataSetInfor);
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","当前功能暂未实现");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }

}


/*读取程序号*/
QString programInfor::CNC_rdprgnum(DATASET dataSetInfor)
{
    ODBPRO result;
    short ret=cnc_rdprgnum(dataSetInfor.flibhndl.toUInt(),&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败"+QString::number(ret));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        json.insert("value", QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取程序名称*/
QString programInfor::CNC_exeprgname(DATASET dataSetInfor)
{
    ODBEXEPRG result;
    short ret=cnc_exeprgname(dataSetInfor.flibhndl.toUInt(),&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败"+QString::number(ret));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        json.insert("value", QString(result.name));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
