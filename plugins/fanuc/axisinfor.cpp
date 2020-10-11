#include "axisinfor.h"


axisInfor::axisInfor()
{

}

QString axisInfor::getValue(DATASET dataSetInfor)
{
    if(dataSetInfor.function=="读取主轴名称"){
        return CNC_rdspdlname(dataSetInfor);
    }
    if(dataSetInfor.function=="读取受控主轴号"){
        return CNC_rdnspdl(dataSetInfor);
    }
    if(dataSetInfor.function=="读取主轴进给率"){
        return CNC_actf(dataSetInfor);
    }
    if(dataSetInfor.function=="读取主轴转速"){
        return CNC_acts(dataSetInfor);
    }
    if(dataSetInfor.function=="读取主轴速度"){
        return CNC_rdsspeed(dataSetInfor);
    }
    if(dataSetInfor.function=="读取主轴齿轮比"){
        return CNC_rdspgear(dataSetInfor);
    }
    if(dataSetInfor.function=="读取主轴负载"){
        return CNC_rdspload(dataSetInfor);
    }
    if(dataSetInfor.function=="读取主轴负载表"){
        return  CNC_rdspmeter(dataSetInfor);
    }
    if(dataSetInfor.function=="读取主轴最大rpm率"){
        return CNC_rdspmaxrpm(dataSetInfor);
    }
    if(dataSetInfor.function=="读取主轴报警"){
        QString s=CNC_rdspdlalm(dataSetInfor);
        return CNC_rdspdlalm(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴名称"){
        return CNC_rdaxisname(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴相对位置"){
        return CNC_relative(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴绝对位置"){
        return CNC_absolute(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴机器位置"){
        return CNC_machine(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴待走移动量"){
        return CNC_distance(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴避让位置"){
        return CNC_skip(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴位置信息"){
        return CNC_rdposition(dataSetInfor);
    }
    if(dataSetInfor.function=="读取伺服负载表"){
        return CNC_rdsvmeter(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴伺服延时值"){
        return CNC_srvdelay(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴加减速延时值"){
        return CNC_accdecdly(dataSetInfor);
    }
    if(dataSetInfor.function=="读取点动或空运行进给率"){
        return CNC_rdjogdrun(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴的回路增益"){
        return CNC_rdloopgain(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴的实际电流"){
        return CNC_rdcurrent(dataSetInfor);
    }
    if(dataSetInfor.function=="读取轴的实际速度"){
        return CNC_rdsrvspeed(dataSetInfor);
    }
//    if(dataSetInfor.function=="读取轴状态信息"){
//        return CNC_rdmovestate(dataSetInfor);
//    }
    if(dataSetInfor.function=="读取恒定表面速度"){
        return CNC_rdspcss(dataSetInfor);
    }
    if(dataSetInfor.function=="读取手轮中断的输入与输出值"){
        return CNC_rdhndintrpt(dataSetInfor);
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



/*读取主轴名称*/
QString axisInfor::CNC_rdspdlname(DATASET dataSetInfor)
{
    ODBSPDLNAME result;
    short num = 32;
    short ret=cnc_rdspdlname(dataSetInfor.flibhndl.toUInt(),&num,&result);
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
/*读取受控主轴号*/
QString axisInfor::CNC_rdnspdl(DATASET dataSetInfor)
{
    short result;
    short ret=cnc_rdnspdl(dataSetInfor.flibhndl.toUInt(),&result);
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
        json.insert("value", QString::number(result));
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
/*读取主轴转速*/
QString axisInfor::CNC_acts(DATASET dataSetInfor)
{
    ODBACT2 result;
    short ret=cnc_acts2(dataSetInfor.flibhndl.toUInt(),ALL_SPINDLES,&result);
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
/*读取主轴速度*/
QString axisInfor::CNC_rdsspeed(DATASET dataSetInfor)
{
    ODBSPEED  result;
    short ret=cnc_rdspeed(dataSetInfor.flibhndl.toUInt(),-1,&result);
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
/*读取主轴齿轮比*/
QString axisInfor::CNC_rdspgear(DATASET dataSetInfor)
{
    ODBSPN  result;
    short ret=cnc_rdspgear(dataSetInfor.flibhndl.toUInt(),ALL_SPINDLES,&result);
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
        json.insert("value", QString::number(result.data[0]));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
 }
/*读取主轴负载*/
QString axisInfor::CNC_rdspload(DATASET dataSetInfor)
{
    ODBSPN  result;
    short ret=cnc_rdspload(dataSetInfor.flibhndl.toUInt(),ALL_SPINDLES,&result);
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
        json.insert("value", QString::number(result.data[0]));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取主轴负载表*/
QString axisInfor::CNC_rdspmeter(DATASET dataSetInfor)
{
    ODBSPLOAD   result[MAX_SPINDLE];
    short num=MAX_SPINDLE;
    short ret=cnc_rdspmeter(dataSetInfor.flibhndl.toUInt(),0,&num,result);
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
        QString name(result[0].spload.name);           /* result.p1.abs.name.ToString();*/
        QString suf(result[0].spload.suff1);
        QString unit="";
        switch (result[0].spload.unit)
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
        json1.insert("speed",result[0].spload.data * qPow(10, -result[0].spload.dec));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取主轴最大rpm比例*/
QString axisInfor::CNC_rdspmaxrpm(DATASET dataSetInfor)
{
    ODBSPN  result;
    short ret=cnc_rdspmaxrpm(dataSetInfor.flibhndl.toUInt(),ALL_SPINDLES,&result);
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
        json.insert("value", QString::number(result.data[0]));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取主轴报警*/
QString axisInfor::CNC_rdspdlalm(DATASET dataSetInfor)
{
    char* result;
    short ret=cnc_rdspdlalm(dataSetInfor.flibhndl.toUInt(),result);
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
        json.insert("value",QString(result));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取轴名称*/
QString axisInfor::CNC_rdaxisname(DATASET dataSetInfor)
{
    ODBAXISNAME result[MAX_AXISNAME];
    short num = 256;
    short id=dataSetInfor.parameter1.toUInt();
    short ret=cnc_rdaxisname(dataSetInfor.flibhndl.toUInt(),&num,result);
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
      if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
          QJsonObject json;
          QJsonObject json1;
          QJsonDocument document;
          QStringList value;
          for(int i=0;i<MAX_AXISNAME;i++){              
             json1.insert(QString::number(i+1),QString(result[i].name));
          }
          json.insert("value",json1);
          document.setObject(json);
          QString json_str(document.toJson(QJsonDocument::Compact));
          return json_str;
      }
      else{
          QJsonObject json;
          QJsonDocument document;
          json.insert("value",QString(result[id-1].name));
          document.setObject(json);
          QString json_str(document.toJson(QJsonDocument::Compact));
          return json_str;
      }
    }
}
/*读取轴相对位置*/
QString axisInfor::CNC_relative(DATASET dataSetInfor)
{
    ODBAXIS   result;
    short ret=cnc_relative2(dataSetInfor.flibhndl.toUInt(),ALL_AXES,256,&result);
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
     if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
         QJsonObject json;
         QJsonObject json1;
         QJsonDocument document;
         QStringList value;
         for(int i=0;i<MAX_AXIS;i++){
            json1.insert(QString::number(i+1),QString::number(result.data[i]));
         }
         json.insert("value",json1);
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
     else{
         QJsonDocument document;
         QJsonObject json;
         json.insert("result","ok");
         json.insert("value", QString::number(result.data[dataSetInfor.parameter1.toInt()]));
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
 }
}
/*读取轴绝对位置*/
QString axisInfor::CNC_absolute(DATASET dataSetInfor)
{
    ODBAXIS   result;
    short ret=cnc_absolute2(dataSetInfor.flibhndl.toUInt(),ALL_AXES,256,&result);
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
     if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
         QJsonObject json;
         QJsonObject json1;
         QJsonDocument document;
         QStringList value;
         for(int i=0;i<MAX_AXIS;i++){
            json1.insert(QString::number(i+1),QString::number(result.data[i]));
         }
         json.insert("value",json1);
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
     else{
         QJsonDocument document;
         QJsonObject json;
         json.insert("result","ok");
         json.insert("value", QString::number(result.data[dataSetInfor.parameter1.toInt()]));
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
 }
}
/*读取轴机器位置*/
QString axisInfor::CNC_machine(DATASET dataSetInfor)
{
    ODBAXIS   result;
    short ret=cnc_machine(dataSetInfor.flibhndl.toUInt(),ALL_AXES,256,&result);
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
     if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
         QJsonObject json;
         QJsonObject json1;
         QJsonDocument document;
         QStringList value;
         for(int i=0;i<MAX_AXIS;i++){
            json1.insert(QString::number(i+1),QString::number(result.data[i]));
         }
         json.insert("value",json1);
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
     else{
         QJsonDocument document;
         QJsonObject json;
         json.insert("result","ok");
         json.insert("value", QString::number(result.data[dataSetInfor.parameter1.toInt()]));
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
 }
}
/*读取轴待走移动量*/
QString axisInfor::CNC_distance(DATASET dataSetInfor)
{
    ODBAXIS   result;
    short ret=cnc_distance(dataSetInfor.flibhndl.toUInt(),ALL_AXES,256,&result);
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
     if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
         QJsonObject json;
         QJsonObject json1;
         QJsonDocument document;
         QStringList value;
         for(int i=0;i<MAX_AXIS;i++){
            json1.insert(QString::number(i+1),QString::number(result.data[i]));
         }
         json.insert("value",json1);
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
     else{
         QJsonDocument document;
         QJsonObject json;
         json.insert("result","ok");
         json.insert("value", QString::number(result.data[dataSetInfor.parameter1.toInt()]));
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
 }
}
/*读取轴避让位置*/
QString axisInfor::CNC_skip(DATASET dataSetInfor)
{
    ODBAXIS   result;
    short ret=cnc_skip(dataSetInfor.flibhndl.toUInt(),ALL_AXES,256,&result);
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
     if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
         QJsonObject json;
         QJsonObject json1;
         QJsonDocument document;
         QStringList value;
         for(int i=0;i<MAX_AXIS;i++){
            json1.insert(QString::number(i+1),QString::number(result.data[i]));
         }
         json.insert("value",json1);
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
     else{
         QJsonDocument document;
         QJsonObject json;
         json.insert("result","ok");
         json.insert("value", QString::number(result.data[dataSetInfor.parameter1.toInt()]));
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
 }
}
/*读取轴位置信息*/
QString axisInfor::CNC_rdposition(DATASET dataSetInfor)
{
    ODBPOS   result[MAX_AXIS];
    short data_num = MAX_AXIS;
    short id=dataSetInfor.parameter1.toUInt();
    short ret=cnc_rdposition(dataSetInfor.flibhndl.toUInt(),-1,&data_num,result);
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
      if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
          QJsonDocument document;
          QJsonObject json;
          QJsonObject json1;
          for(int i=0;i<MAX_AXIS;i++){
               QString name(result[i].abs.name);
            if(!name.isEmpty()){
                QJsonObject json2;

                QString suf(result[i].abs.suff);
                QString unit="";
                switch (result[i].abs.unit)
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
                json2.insert("name",name);
                json2.insert("suf",suf);
                json2.insert("unit",unit);
                json2.insert("abs_val",result[i].abs.data *qPow(10, -result[i].abs.dec));
                json2.insert("mach_val",result[i].mach.data *qPow(10, -result[i].mach.dec));
                json2.insert("rel_val",result[i].rel.data *qPow(10, -result[i].rel.dec));
                json2.insert("dist_val",result[i].dist.data *qPow(10, -result[i].dist.dec));
                json1.insert(name,json2);
            }
          }
            json.insert("result","ok");
            json.insert("value",json1);
            document.setObject(json);
            QString json_str(document.toJson(QJsonDocument::Compact));
            return json_str;
      }
      else{
            QString name(result[id-1].abs.name);           /* result.p1.abs.name.ToString();*/
            QString suf(result[id-1].abs.suff);
            QString unit="";
            switch (result[id-1].abs.unit)
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
            json1.insert("abs_val",result[id-1].abs.data *qPow(10, -result[id-1].abs.dec));
            json1.insert("mach_val",result[id-1].mach.data *qPow(10, -result[id-1].mach.dec));
            json1.insert("rel_val",result[id-1].rel.data *qPow(10, -result[id-1].rel.dec));
            json1.insert("dist_val",result[id-1].dist.data *qPow(10, -result[id-1].dist.dec));
            json.insert("result","ok");
            json.insert("value",json1);
            document.setObject(json);
            QString json_str(document.toJson(QJsonDocument::Compact));
            return json_str;
      }
    }
}
/*读取伺服负载表*/
QString axisInfor::CNC_rdsvmeter(DATASET dataSetInfor)
{
    ODBSVLOAD  result[MAX_AXIS];
    short num=MAX_AXIS;
    short ret=cnc_rdsvmeter(dataSetInfor.flibhndl.toUInt(),&num,result);
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
        for(int i=0;i<MAX_AXIS;i++){
        QJsonObject json2;
        QString name(result[i].svload.name);           /* result.p1.abs.name.ToString();*/
        QString suf(result[i].svload.suff1);
        QString unit="";
        switch (result[i].svload.unit)
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
        json2.insert("name",name);
        json2.insert("suf",suf);
        json2.insert("unit",unit);
        json2.insert("speed",result[i].svload.data * qPow(10, -result[i].svload.dec));
        json1.insert(name,json2);
        }
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取伺服延迟值*/
QString axisInfor::CNC_srvdelay(DATASET dataSetInfor)
{
    ODBAXIS   result;
    short ret=cnc_srvdelay(dataSetInfor.flibhndl.toUInt(),ALL_AXES,256,&result);
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
     if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
         QJsonObject json;
         QJsonObject json1;
         QJsonDocument document;
         QStringList value;
         for(int i=0;i<MAX_AXIS;i++){
            json1.insert(QString::number(i+1),QString::number(result.data[i]));
         }
         json.insert("value",json1);
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
     else{
         QJsonDocument document;
         QJsonObject json;
         json.insert("result","ok");
         json.insert("value", QString::number(result.data[dataSetInfor.parameter1.toInt()]));
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
 }
}
/*读取轴加减速延迟值*/
QString axisInfor::CNC_accdecdly(DATASET dataSetInfor)
{
    ODBAXIS   result;
    short ret=cnc_accdecdly(dataSetInfor.flibhndl.toUInt(),ALL_AXES,256,&result);
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
     if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
         QJsonObject json;
         QJsonObject json1;
         QJsonDocument document;
         QStringList value;
         for(int i=0;i<MAX_AXIS;i++){
            json1.insert(QString::number(i+1),QString::number(result.data[i]));
         }
         json.insert("value",json1);
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
     else{
         QJsonDocument document;
         QJsonObject json;
         json.insert("result","ok");
         json.insert("value", QString::number(result.data[dataSetInfor.parameter1.toInt()]));
         document.setObject(json);
         QString json_str(document.toJson(QJsonDocument::Compact));
         return json_str;
     }
    }
}
/*读取点动或空运行进给率*/
QString axisInfor::CNC_rdjogdrun(DATASET dataSetInfor)
{
    ODBJOGDRUN   result[MAX_AXIS];
    short ret=cnc_rdjogdrun(dataSetInfor.flibhndl.toUInt(),ALL_AXES,result);
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
       if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
            QJsonObject json;
            QJsonObject json1;
            QJsonDocument document;
            QStringList value;
            for(int i=0;i<MAX_AXIS;i++){
               json1.insert(QString::number(i+1),QString::number(result[i].jogdrun.data));
            }
            json.insert("value",json1);
            document.setObject(json);
            QString json_str(document.toJson(QJsonDocument::Compact));
            return json_str;
       }
       else{
           QJsonDocument document;
           QJsonObject json;
           json.insert("result","ok");
           json.insert("value", QString::number(result[dataSetInfor.parameter1.toInt()].jogdrun.data));
           document.setObject(json);
           QString json_str(document.toJson(QJsonDocument::Compact));
           return json_str;
       }     
    }   
}
/*读取轴的回路增益*/
QString axisInfor::CNC_rdloopgain(DATASET dataSetInfor)
{
    long   result[MAX_AXIS];
    short ret=cnc_rdloopgain(dataSetInfor.flibhndl.toUInt(),result);
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
       if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
            QJsonObject json;
            QJsonObject json1;
            QJsonDocument document;
            QStringList value;
            for(int i=0;i<MAX_AXIS;i++){
               json1.insert(QString::number(i+1),QString::number(result[i]));
            }
            json.insert("value",json1);
            document.setObject(json);
            QString json_str(document.toJson(QJsonDocument::Compact));
            return json_str;
       }
       else{
           QJsonDocument document;
           QJsonObject json;
           json.insert("result","ok");
           json.insert("value", QString::number(result[dataSetInfor.parameter1.toInt()]));
           document.setObject(json);
           QString json_str(document.toJson(QJsonDocument::Compact));
           return json_str;
       }
    }
}
/*读取轴的实际电流*/
QString axisInfor::CNC_rdcurrent(DATASET dataSetInfor)
{
    short   result[MAX_AXIS];
    short ret=cnc_rdcurrent(dataSetInfor.flibhndl.toUInt(),result);
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
       if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
            QJsonObject json;
            QJsonObject json1;
            QJsonDocument document;
            QStringList value;
            for(int i=0;i<MAX_AXIS;i++){
               json1.insert(QString::number(i+1),QString::number(result[i]));
            }
            json.insert("value",json1);
            document.setObject(json);
            QString json_str(document.toJson(QJsonDocument::Compact));
            return json_str;
       }
       else{
           QJsonDocument document;
           QJsonObject json;
           json.insert("result","ok");
           json.insert("value", QString::number(result[dataSetInfor.parameter1.toInt()]));
           document.setObject(json);
           QString json_str(document.toJson(QJsonDocument::Compact));
           return json_str;
       }
    }
}
/*读取轴的实际速度*/
QString axisInfor::CNC_rdsrvspeed(DATASET dataSetInfor)
{
    long   result[MAX_AXIS];
    short ret=cnc_rdsrvspeed(dataSetInfor.flibhndl.toUInt(),result);
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
       if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
            QJsonObject json;
            QJsonObject json1;
            QJsonDocument document;
            QStringList value;
            for(int i=0;i<MAX_AXIS;i++){
               json1.insert(QString::number(i+1),QString::number(result[i]));
            }
            json.insert("value",json1);
            document.setObject(json);
            QString json_str(document.toJson(QJsonDocument::Compact));
            return json_str;
       }
       else{
           QJsonDocument document;
           QJsonObject json;
           json.insert("result","ok");
           json.insert("value", QString::number(result[dataSetInfor.parameter1.toInt()]));
           document.setObject(json);
           QString json_str(document.toJson(QJsonDocument::Compact));
           return json_str;
       }
    }
}



/*读取轴状态信息*/
QString axisInfor::CNC_rdmovestate(DATASET dataSetInfor)
{
    unsigned char *result;
    short num=256;
//    if(dataSetInfor.parameter1.isEmpty()||dataSetInfor.parameter1=="-1"){
//        short ret=cnc_rdmovestate(dataSetInfor.flibhndl.toUInt(),1,&num,result);
//        if(ret!=EW_OK){
//            QJsonDocument document;
//            QJsonObject json;
//            json.insert("result","err");
//            json.insert("value","读取数值失败"+QString::number(ret));
//            document.setObject(json);
//            QString json_str(document.toJson(QJsonDocument::Compact));
//            return json_str;
//        }
//        else{
//            QJsonDocument document;
//            QJsonObject json;
//            json.insert("result","ok");
//            QStringList value;
//            for(int i=0;i<MAX_AXIS;i++){
//                value.append(QString(result[i]));
//            }
//            json.insert("value", value.join(";"));
//            document.setObject(json);
//            QString json_str(document.toJson(QJsonDocument::Compact));
//            return json_str;
//        }
//    }
//    else{
////        short ret=cnc_rdmovestate(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toInt(),&num,result);
//        if(ret!=EW_OK){
//            QJsonDocument document;
//            QJsonObject json;
//            json.insert("result","err");
//            json.insert("value","读取数值失败"+QString::number(ret));
//            document.setObject(json);
//            QString json_str(document.toJson(QJsonDocument::Compact));
//            return json_str;
//        }
//        else{
//            QJsonDocument document;
//            QJsonObject json;
//            json.insert("result","ok");
//            json.insert("value", QString(result[0]));
//            document.setObject(json);
//            QString json_str(document.toJson(QJsonDocument::Compact));
//            return json_str;
//        }
    }
/*读取恒定表面速度*/
QString axisInfor::CNC_rdspcss(DATASET dataSetInfor)
{
    ODBCSS  result;
    short ret=cnc_rdspcss(dataSetInfor.flibhndl.toUInt(),&result);
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
        json1.insert("smax",QString::number(result.smax));
        json1.insert("srpm",QString::number(result.srpm));
        json1.insert("sspm",QString::number(result.sspm));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}

///*显示手轮中断的输入与输出值*/
QString axisInfor::CNC_rdhndintrpt(DATASET dataSetInfor)
{
    ODBHND  result[8];
    short type = -1;
    short data_num =3;
    QString unit = "";
    short ret=cnc_rdhndintrpt(dataSetInfor.flibhndl.toUInt(),type,&data_num,result);
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
        QStringList value;
//        for(int i=0;i<MAX_AXIS;i++){
//            value.append(QString::number(result.data[0][i])+QString::number(result.data[1][i]));
//        }
        json.insert("value", value.join(";"));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
