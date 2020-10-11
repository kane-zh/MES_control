#include "operationhistroydata.h"

operationHistroyData::operationHistroyData()
{

}

QString operationHistroyData::getValue(DATASET dataSetInfor)
{
    if(dataSetInfor.function=="读取操作历史数据"){
       return CNC_rdophistry(dataSetInfor);
    }
    if(dataSetInfor.function=="读取报警历史记录数据"){
       return CNC_rdalmhistry(dataSetInfor);
    }
    if(dataSetInfor.function=="读取信号相关的操作历史"){

    }
    if(dataSetInfor.function=="读取操作员历史记录数据的数量"){

    }
    if(dataSetInfor.function=="读取操作员历史记录数据"){

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

/*读取操作历史数据*/
QString operationHistroyData::CNC_rdophistry(DATASET dataSetInfor)
{
    short ret = cnc_stopophis(dataSetInfor.flibhndl.toUInt());
    ushort hisno = 0;
    ret = cnc_rdalmhisno(dataSetInfor.flibhndl.toUInt(), &hisno);
    ushort s_no = 1;
    ushort e_no = hisno;//报警履历数是功能选项
    ushort length = 6000;
    ODBAHIS5 result[11];
    ret = cnc_rdalmhistry5(dataSetInfor.flibhndl.toUInt(),s_no,e_no,length,result);
    ret = cnc_startophis(dataSetInfor.flibhndl.toUInt());
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
//            json.insert("value",QString::number(result.data));
            document.setObject(json);
            QString json_str(document.toJson(QJsonDocument::Compact));
            return json_str;
        }
}
/*读取主轴进给率*/
QString operationHistroyData::CNC_rdalmhistry(DATASET dataSetInfor)
{
//    short ret;
//    ALMINFO result;
//    ret= cnc_rdalminfo_bg(dataSetInfor.flibhndl.toUShort(),1,1,1,&result);
//    QJsonDocument document;
//    QJsonObject json;
//    json.insert("result","ok");
////            json.insert("value",QString::number(result.data));
//    document.setObject(json);
//    QString json_str(document.toJson(QJsonDocument::Compact));
//    return json_str;
}
