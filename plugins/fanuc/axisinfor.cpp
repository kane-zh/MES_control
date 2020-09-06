#include "axisinfor.h"


axisInfor::axisInfor()
{

}

QString axisInfor::getValue(DATASET dataSetInfor)
{
    if(dataSetInfor.function=="读取主轴进给率"){
       return CNC_actf(dataSetInfor);
    }
    if(dataSetInfor.function=="读取主轴转速"){

    }
    if(dataSetInfor.function=="读取恒定表面速度"){

    }
    if(dataSetInfor.function=="读取轴负载"){

    }
    if(dataSetInfor.function=="读取伺服负载表"){

    }
    if(dataSetInfor.function=="读取轴负载表"){

    }
    if(dataSetInfor.function=="读取轴最大rpm率"){

    }
    if(dataSetInfor.function=="读取轴转速"){
    }
    if(dataSetInfor.function=="读取轴速度"){

    }
    if(dataSetInfor.function=="读取轴齿轮比"){

    }
    if(dataSetInfor.function=="读取轴相对位置"){

    }
    if(dataSetInfor.function=="读取轴绝对位置"){

    }
    if(dataSetInfor.function=="读取轴机器位置"){

    }
    if(dataSetInfor.function=="读取轴剩余移动量"){

    }
    if(dataSetInfor.function=="读取轴避让位置"){

    }
    if(dataSetInfor.function=="读取轴伺服延时值"){

    }
    if(dataSetInfor.function=="读取轴加减速延时值"){
    }
    if(dataSetInfor.function=="读取显示的点动或空运行进给率"){

    }
    if(dataSetInfor.function=="读取轴刀具位置信息"){

    }
    if(dataSetInfor.function=="读取手动重叠运动值"){

    }
    if(dataSetInfor.function=="显示手轮中断的输入与输出值"){

    }
    if(dataSetInfor.function=="读取用于5轴加工的手动进给的机床轴移动量"){

    }
    if(dataSetInfor.function=="读取轴加工汇总数据"){

    }
    if(dataSetInfor.function=="读取轴所有动态数据"){

    }
    if(dataSetInfor.function=="读取轴所有数据"){

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
/*读取主轴进给率*/
QString axisInfor::CNC_actf(DATASET dataSetInfor)
{
    ODBACT result;
    short ret=cnc_actf(dataSetInfor.flibhndl.toUInt(),&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
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
/*读取主轴转速*/
QString axisInfor::CNC_acts(DATASET dataSetInfor)
{
    ODBACT  result;
    short ret=cnc_acts(dataSetInfor.flibhndl.toUInt(),&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
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
/*读取主轴转速及进给率*/
QString axisInfor::CNC_speed(DATASET dataSetInfor)
{
    ODBSPEED  result;
    short ret=cnc_rdspeed(dataSetInfor.flibhndl.toUInt(),-1,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QString acts_unit="";
        switch (result.acts.unit)
        {
            case 0:
            acts_unit = "mm";
            break;
            case 1:
            acts_unit = "inch";
            break;
            case 2:
            acts_unit = "degree";
            break;
            case 3:
            acts_unit = "mm/rev";
            break;
            case 4:
            acts_unit = "inch/rev";
            break;
        }
        QString actf_unit="";
        switch (result.actf.unit)
        {
            case 0:
            actf_unit = "mm";
            break;
            case 1:
            actf_unit = "inch";
            break;
            case 2:
            actf_unit = "degree";
            break;
            case 3:
            actf_unit = "mm/rev";
            break;
            case 4:
            actf_unit = "inch/rev";
            break;
        }
        QJsonDocument document;
        QJsonObject json;
        QJsonObject json1;
        json1.insert("acts_name",result.acts.name);
        json1.insert("actf_name",result.actf.name);
        json1.insert("acts_suff",result.acts.suff);
        json1.insert("actf_suff",result.actf.suff);
        json1.insert("acts_unit",acts_unit);
        json1.insert("actf_unit",actf_unit);
        json1.insert("acts_data",result.acts.data * qPow(10, -result.acts.dec));
        json1.insert("actf_data",result.actf.data * qPow(10, -result.actf.dec));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取主轴位置数据*/
QString axisInfor::CNC_rdposition(DATASET dataSetInfor)
{
    ODBPOS   result;    short data_num = 3;
    short ret=cnc_rdposition(dataSetInfor.flibhndl.toUInt(),-1,&data_num,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QString name(result.abs.name);           /* result.p1.abs.name.ToString();*/
        QString suf(result.abs.suff);
        QString unit="";
        switch (result.abs.unit)
        {
            case 0:
              unit = "mm";
              break;
            case 1:
              unit = "inch";
              break;
            case 2:
              unit = "degree";
              break;
        }
        QJsonDocument document;
        QJsonObject json;
        QJsonObject json1;
        json1.insert("name",name);
        json1.insert("suf",suf);
        json1.insert("unit",unit);
        json1.insert("abs_val",result.abs.data *qPow(10, -result.abs.dec));
        json1.insert("mach_val",result.mach.data *qPow(10, -result.mach.dec));
        json1.insert("rel_val",result.rel.data *qPow(10, -result.rel.dec));
        json1.insert("dist_val",result.dist.data *qPow(10, -result.dist.dec));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取指定轴负载表*/
QString axisInfor::CNC_rdspmeter(DATASET dataSetInfor)
{
    ODBSPLOAD   result;
    short num=dataSetInfor.parameter1.toInt();
    short ret=cnc_rdspmeter(dataSetInfor.flibhndl.toUInt(), 1,&num,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QString name(result.spload.name);           /* result.p1.abs.name.ToString();*/
        QString suf(result.spload.suff1);
        QString unit="";
        switch (result.spload.unit)
        {
            case 0:
            unit = "mm/min";
            break;
            case 1:
            unit = "inch/min";
            break;
            case 2:
            unit = "rpm";
            break;
            case 3:
            unit = "mm/rev";
            break;
            case 4:
            unit = "inch/rev";
            break;
        }
        QJsonDocument document;
        QJsonObject json;
        QJsonObject json1;
        json1.insert("name",name);
        json1.insert("suf",suf);
        json1.insert("unit",unit);
        json1.insert("speed",result.spload.data * qPow(10, -result.spload.dec));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取指定轴伺服负载表*/
QString axisInfor::CNC_rdsvmeter(DATASET dataSetInfor)
{
    ODBSVLOAD  result;
    short num=dataSetInfor.parameter1.toInt();
    short ret=cnc_rdsvmeter(dataSetInfor.flibhndl.toUInt(),&num,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QString name(result.svload.name);           /* result.p1.abs.name.ToString();*/
        QString suf(result.svload.suff1);
        QString unit="";
        switch (result.svload.unit)
        {
            case 0:
            unit = "mm/min";
            break;
            case 1:
            unit = "inch/min";
            break;
            case 2:
            unit = "rpm";
            break;
            case 3:
            unit = "mm/rev";
            break;
            case 4:
            unit = "inch/rev";
            break;
        }
        QJsonDocument document;
        QJsonObject json;
        QJsonObject json1;
        json1.insert("name",name);
        json1.insert("suf",suf);
        json1.insert("unit",unit);
        json1.insert("speed",result.svload.data * qPow(10, -result.svload.dec));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取所有轴负载*/
QString axisInfor::CNC_rdspload(DATASET dataSetInfor)
{
    ODBSPN  result;
    short ret=cnc_rdspload(dataSetInfor.flibhndl.toUInt(),ALL_SPINDLES,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList  value;
        value.clear();
        for(int i=0;i<MAX_SPINDLE;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取所有轴齿轮比*/
QString axisInfor::CNC_rdspgear(DATASET dataSetInfor)
{
    ODBSPN  result;
    short ret=cnc_rdspgear(dataSetInfor.flibhndl.toUInt(),ALL_SPINDLES,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_SPINDLE;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取所有轴转速*/
QString axisInfor::CNC_acts2(DATASET dataSetInfor)
{
    ODBACT2  result;
    short ret=cnc_acts2(dataSetInfor.flibhndl.toUInt(),ALL_SPINDLES,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_SPINDLE;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取所有轴绝对位置*/
QString axisInfor::CNC_absolute(DATASET dataSetInfor)
{
    ODBAXIS  result;
    short ret=cnc_absolute2(dataSetInfor.flibhndl.toUInt(),ALL_AXES,4+4*MAX_AXIS,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_AXIS;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取所有轴机器位置*/
QString axisInfor::CNC_machine(DATASET dataSetInfor)
{
    ODBAXIS   result;
    short ret=cnc_machine(dataSetInfor.flibhndl.toUInt(),ALL_AXES,4+4*MAX_AXIS,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_AXIS;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取所有轴相对位置*/
QString axisInfor::CNC_relative(DATASET dataSetInfor)
{
    ODBAXIS  result;
    short ret=cnc_relative2(dataSetInfor.flibhndl.toUInt(),ALL_AXES,4+4*MAX_AXIS,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_AXIS;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取所有轴剩余移动量*/
QString axisInfor::CNC_distance(DATASET dataSetInfor)
{
    ODBAXIS  result;
    short ret=cnc_distance2(dataSetInfor.flibhndl.toUInt(),ALL_AXES,4+4*MAX_AXIS,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_AXIS;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取所有轴避让位置*/
QString axisInfor::CNC_skip(DATASET dataSetInfor)
{
    ODBAXIS  result;
    short ret=cnc_skip(dataSetInfor.flibhndl.toUInt(),ALL_AXES,256,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_AXIS;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取所有伺服延迟值*/
QString axisInfor::CNC_srvdelay(DATASET dataSetInfor)
{
    ODBAXIS  result;
    short ret=cnc_srvdelay(dataSetInfor.flibhndl.toUInt(),ALL_AXES,256,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_AXIS;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取所有加减速延迟值*/
QString axisInfor::CNC_accdecdly(DATASET dataSetInfor)
{
    ODBAXIS  result;
    short ret=cnc_accdecdly(dataSetInfor.flibhndl.toUInt(),ALL_AXES,256,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_AXIS;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}


/*读取所有轴手动重叠运动值*/
QString axisInfor::CNC_rdmovrlapm(DATASET dataSetInfor)
{
    IODBOVL  result;
    short ret=cnc_rdmovrlap(dataSetInfor.flibhndl.toUInt(),MAX_AXIS,256,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_AXIS;i++){
            value.append(QString::number(result.data[0][i])+QString::number(result.data[1][i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}

/*读取所有主轴最大rpm比例*/
QString axisInfor::CNC_rdspmaxrpm(DATASET dataSetInfor)
{
    ODBSPN  result;
    short ret=cnc_rdspmaxrpm(dataSetInfor.flibhndl.toUInt(),MAX_AXIS,&result);
    if(ret!=EW_OK){
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","err");
        json.insert("value","读取数值失败");
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
    else{
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        QStringList value;
        for(int i=0;i<MAX_AXIS;i++){
            value.append(QString::number(result.data[i]));
        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}




///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
///*读取绝对轴位置*/
//QString axisInfor::CNC_absolute(DATASET dataSetInfor)
//{
//    ODBAXIS  result;
//    short ret=cnc_absolute(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),8,&result);
//    if(ret!=EW_OK){
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","err");
//        json.insert("value","读取数值失败");
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//    else{
//        QJsonDocument document;
//        QJsonObject json;
//        json.insert("result","ok");
//        json.insert("value",QString::number(result.data[0]));
//        document.setObject(json);
//        QString json_str(document.toJson(QJsonDocument::Compact));
//        return json_str;
//    }
//}
