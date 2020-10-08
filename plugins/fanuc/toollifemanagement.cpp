#include "toollifemanagement.h"

toolLifeManagement::toolLifeManagement()
{

}
QString toolLifeManagement::getValue(DATASET dataSetInfor)
{
    if(dataSetInfor.function=="读取刀具寿命管理数据"){
       return CNC_rdtlinfo(dataSetInfor);
    }
    if(dataSetInfor.function=="读取最大刀具组数"){
       return CNC_rdmaxgrp(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具组数"){
        return CNC_rdngrp(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具组号"){
        return CNC_rdgrpid(dataSetInfor);
    }
    if(dataSetInfor.function=="读取使用的刀具组号"){
        return CNC_rdtlusegrp(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具组信息"){
        return CNC_rdtlgrp(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具组寿命"){
        return CNC_rdlife(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具组升降计数"){
        return CNC_rdcount(dataSetInfor);
    }
    if(dataSetInfor.function=="读取组内最大刀具数量"){
        return CNC_rdmaxtool(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具数量"){
        return CNC_rdntool(dataSetInfor);
    }
    if(dataSetInfor.function=="读取组内使用的刀具号"){
        return CNC_rdusetlno(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具数据"){
        return CNC_rdtltool(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具长度"){
        return CNC_rdlength(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具补偿"){
        return CNC_rdradius(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具信息"){
        return CNC_rdtoolinfo(dataSetInfor);
    }
    if(dataSetInfor.function=="读取刀具编号"){
        return CNC_toolnum(dataSetInfor);
    }
    if(dataSetInfor.function=="读取组内所有数据"){
        return CNC_rdtoolgrp(dataSetInfor);
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
/*读取刀具寿命元数据*/
QString toolLifeManagement::CNC_rdtlinfo(DATASET dataSetInfor)
{
    ODBTLINFO  result;
    short ret=cnc_rdtlinfo(dataSetInfor.flibhndl.toUInt(),&result);
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
        QJsonObject json1;
        json1.insert("max_group",QString::number(result.max_group));
        json1.insert("max_tool",QString::number(result.max_tool));
        json1.insert("max_cycle",QString::number(result.max_cycle));
        json1.insert("max_minute",QString::number(result.max_minute));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取最大刀具组数*/
QString toolLifeManagement::CNC_rdmaxgrp(DATASET dataSetInfor)
{
    ODBLFNO  result;
    short ret=cnc_rdmaxgrp(dataSetInfor.flibhndl.toUInt(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取刀具组数*/
QString toolLifeManagement::CNC_rdngrp(DATASET dataSetInfor)
{
    ODBTLIFE2  result;
    short ret=cnc_rdngrp(dataSetInfor.flibhndl.toUInt(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取刀具组号*/
QString toolLifeManagement::CNC_rdgrpid(DATASET dataSetInfor)
{
    ODBTLIFE5  result;
    short ret=cnc_rdgrpid2(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取使用的刀具组号*/
QString toolLifeManagement::CNC_rdtlusegrp(DATASET dataSetInfor)
{
    ODBUSEGRP  result;
    short ret=cnc_rdtlusegrp(dataSetInfor.flibhndl.toUInt(),&result);
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
        QJsonObject json1;
        json1.insert("use",QString::number(result.use));
        json1.insert("next",QString::number(result.next));
        json1.insert("slct",QString::number(result.slct));
        json1.insert("opt_use",QString::number(result.opt_use));
        json1.insert("opt_next",QString::number(result.opt_next));
        json1.insert("opt_slct",QString::number(result.opt_slct));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取刀具组信息*/
QString toolLifeManagement::CNC_rdtlgrp(DATASET dataSetInfor)
{
    IODBTLGRP  result;
    short num=1;
    short ret=cnc_rdtlgrp(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toLong(),&num,&result);
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
        QJsonObject json1;
        json1.insert("count",QString::number(result.count));
        json1.insert("count_type",QString::number(result.count_type));
        json1.insert("life",QString::number(result.life));
        json1.insert("life_rest",QString::number(result.life_rest));
        json1.insert("nfree",QString::number(result.nfree));
        json1.insert("ntool",QString::number(result.ntool));
        json1.insert("opt_grpno",QString::number(result.opt_grpno));
        json1.insert("rest_sig",QString::number(result.rest_sig));
        json1.insert("use_tool",QString::number(result.use_tool));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取刀具组寿命*/
QString toolLifeManagement::CNC_rdlife(DATASET dataSetInfor)
{
    ODBTLIFE3  result;
    short ret=cnc_rdlife(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取刀具组升降数*/
QString toolLifeManagement::CNC_rdcount(DATASET dataSetInfor)
{
    ODBTLIFE3  result;
    short ret=cnc_rdcount(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取组内最大刀具数量*/
QString toolLifeManagement::CNC_rdmaxtool(DATASET dataSetInfor)
{
    ODBLFNO  result;
    short ret=cnc_rdmaxtool(dataSetInfor.flibhndl.toUInt(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}

/*读取刀具数量*/
QString toolLifeManagement::CNC_rdntool(DATASET dataSetInfor)
{
    ODBTLIFE3  result;
    short ret=cnc_rdntool(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}

/*读取组内使用的刀具号*/
QString toolLifeManagement::CNC_rdusetlno(DATASET dataSetInfor)
{
    ODBTLUSE  result;
    short num=36;
    short ret=cnc_rdusetlno(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),dataSetInfor.parameter1.toShort(),36,&result);
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
        json.insert("value",QString::number(result.data[0]));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取刀具数据*/
QString toolLifeManagement::CNC_rdtltool(DATASET dataSetInfor)
{
    IODBTD2  result;
    short ret=cnc_rd1tlifedat2(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),dataSetInfor.parameter2.toShort(),&result);
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
        QJsonObject json1;
        json1.insert("type",QString::number(result.type));
        json1.insert("d_code",QString::number(result.d_code));
        json1.insert("h_code",QString::number(result.h_code));
        json1.insert("tool_inf",QString::number(result.tool_inf));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}

/*读取刀具长度*/
QString toolLifeManagement::CNC_rdlength(DATASET dataSetInfor)
{
    ODBTLIFE4  result;
    short ret=cnc_rd2length(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),dataSetInfor.parameter2.toShort(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}


/*读取刀具补偿*/
QString toolLifeManagement::CNC_rdradius(DATASET dataSetInfor)
{
    ODBTLIFE4  result;
    short ret=cnc_rd2radius(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),dataSetInfor.parameter2.toShort(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}


/*读取刀具信息*/
QString toolLifeManagement::CNC_rdtoolinfo(DATASET dataSetInfor)
{
    ODBTLIFE4  result;
    short ret=cnc_t2info(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),dataSetInfor.parameter2.toShort(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}


/*读取刀具编号*/
QString toolLifeManagement::CNC_toolnum(DATASET dataSetInfor)
{
    ODBTLIFE4  result;
    short ret=cnc_toolnum(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),dataSetInfor.parameter2.toShort(),&result);
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
        json.insert("value",QString::number(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}


/*读取组内所有数据*/
QString toolLifeManagement::CNC_rdtoolgrp(DATASET dataSetInfor)
{
    ODBTG  result;
    short ret=cnc_rdtoolgrp(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),256,&result);
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
        QJsonObject json1;
        json1.insert("life",QString::number(result.life));
        json1.insert("count",QString::number(result.count));
        json1.insert("ntool",QString::number(result.ntool));
        json1.insert("grp_num",QString::number(result.grp_num));
        json1.insert("tinfo",QString::number(result.data[0].tinfo));
        json1.insert("tool_num",QString::number(result.data[0].tool_num));
        json1.insert("tuse_num",QString::number(result.data[0].tuse_num));
        json1.insert("length_num",QString::number(result.data[0].length_num));
        json1.insert("radius_num",QString::number(result.data[0].radius_num));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}

