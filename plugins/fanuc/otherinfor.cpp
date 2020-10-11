#include "otherinfor.h"

otherInfor::otherInfor()
{

}

QString otherInfor::getValue(DATASET dataSetInfor)
{
    if(dataSetInfor.function=="读取CNC系统信息"){
      return CNC_sysinfo(dataSetInfor);
    }
    if(dataSetInfor.function=="读取CNC系统硬件信息"){
      return CNC_rdsyshard(dataSetInfor);
    }
    if(dataSetInfor.function=="读取CNC状态信息"){
      return CNC_statinfo(dataSetInfor);
    }
    if(dataSetInfor.function=="读取操作员消息"){
        return CNC_rdopmsg(dataSetInfor);
    }
    if(dataSetInfor.function=="读取模态数据"){

    }
    if(dataSetInfor.function=="读取诊断数据"){

    }
    if(dataSetInfor.function=="读取警报消息"){
        return CNC_rdalmmsg(dataSetInfor);
    }
    if(dataSetInfor.function=="读取警报状态"){
        return CNC_alarm(dataSetInfor);
    }
    if(dataSetInfor.function=="清除CNC警报"){

    }
    if(dataSetInfor.function=="清除报警"){

    }
    if(dataSetInfor.function=="获取详细错误信息"){
        return CNC_getdtailerr(dataSetInfor);
    }
    if(dataSetInfor.function=="读取CNC参数的信息"){

    }
    if(dataSetInfor.function=="读取CNC参数的最大值最小值和总数"){

    }
    if(dataSetInfor.function=="读取CNC配置信息"){

    }
    if(dataSetInfor.function=="读取CNC设定数据的信息"){

    }
    if(dataSetInfor.function=="读取CNC设置数据的最大最小和总数"){

    }
    if(dataSetInfor.function=="读取CNC诊断数据的信息"){
        return  CNC_rddiaginfor(dataSetInfor);
    }
    if(dataSetInfor.function=="读取CNC诊断数据的最大最小和总数"){

    }
    if(dataSetInfor.function=="读取DMG Netservice状态信息"){

    }
    if(dataSetInfor.function=="读取G代码"){

    }
    if(dataSetInfor.function=="读取程序重新启动信息"){

    }
    if(dataSetInfor.function=="读取命令值"){

    }
    if(dataSetInfor.function=="获得最大有效数字和小数位数"){

    }
    if(dataSetInfor.function=="获取屏幕状态"){

    }
    if(dataSetInfor.function=="更改屏幕模式"){

    }
    if(dataSetInfor.function=="读取AD转换数据"){

    }
    if(dataSetInfor.function=="读取块状态"){

    }
    if(dataSetInfor.function==""){

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

/*读取CNC系统信息*/
QString otherInfor::CNC_sysinfo(DATASET dataSetInfor)
{
    ODBSYS result;
    short ret=cnc_sysinfo(dataSetInfor.flibhndl.toUInt(),&result);
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
        QString addinfo="";
        if((result.addinfo&0x01)==0x00){
            addinfo.append("without loader control function");
            addinfo.append(",");
        }
        else if((result.addinfo&0x01)==0x01){
            addinfo.append("with loader control function");
            addinfo.append(",");
        }
        if(((result.addinfo>>1)&0x01)==0x00){
            addinfo.append("not an i Series CNC");
            addinfo.append(",");
        }
        else if(((result.addinfo>>1)&0x01)==0x01){
            addinfo.append("i Series CNC");
            addinfo.append(",");
        }
        if(((result.addinfo>>3)&0x01)==0x00){
            addinfo.append("without compound machining function");
            addinfo.append(",");
        }
        else if(((result.addinfo>>3)&0x01)==0x01){
            addinfo.append("with compound machining function");
            addinfo.append(",");
        }
        if(((result.addinfo>>4)&0x01)==0x00){
            addinfo.append("without transfer line function");
            addinfo.append(",");
        }
        else if(((result.addinfo>>4)&0x01)==0x01){
            addinfo.append("with transfer line function");
            addinfo.append(",");
        }
        if((result.addinfo>>8)==0x00){
            addinfo.append("MODEL information is not supported");
        }
        else if((result.addinfo>>8)==0x01){
            addinfo.append("MODEL A");
        }
        else if((result.addinfo>>8)==0x02){
            addinfo.append("MODEL B");
        }
        else if((result.addinfo>>8)==0x03){
            addinfo.append("MODEL C");
        }
        else if((result.addinfo>>8)==0x04){
            addinfo.append("MODEL D");
        }
        QString cnc_type=QString(result.cnc_type[0])+QString(result.cnc_type[1]);
        if(cnc_type=="0")
        {
            cnc_type="Series 0i";
        }
        if(cnc_type=="15")
        {
            cnc_type="Series 15/15i";
        }
        if(cnc_type=="16")
        {
            cnc_type="Series 16/16i";
        }
        if(cnc_type=="18")
        {
            cnc_type="Series 18/18i";
        }
        if(cnc_type=="21")
        {
            cnc_type="Series 21/21i";
        }
        if(cnc_type=="30")
        {
            cnc_type="Series 30i";
        }
        if(cnc_type=="31")
        {
            cnc_type="Series 31i";
        }
        if(cnc_type=="32")
        {
            cnc_type="Series 32i";
        }
        if(cnc_type=="PD")
        {
            cnc_type="Power Mate i-D";
        }
        if(cnc_type=="PH")
        {
            cnc_type="Power Mate i-H";
        }
        QString mt_type=QString(result.mt_type[0])+QString(result.mt_type[1]);
        if(mt_type==" M")
        {
            mt_type="Machining center";
        }
        if(mt_type==" T")
        {
            mt_type="Lathe";
        }
        if(mt_type=="MM")
        {
            mt_type="M series with 2 path control";
        }
        if(mt_type=="TT")
        {
            mt_type="T series with 2/3 path control";
        }
        if(mt_type==" P")
        {
            mt_type="Punch press";
        }
        if(mt_type==" L")
        {
            mt_type="Laser";
        }
        if(mt_type==" W")
        {
            mt_type="Wire cut";
        }
        QJsonDocument document;
        QJsonObject json;
        QJsonObject json1;
        json1.insert("axes",QString(result.axes[0])+QString(result.axes[1]));
        json1.insert("addinfo",addinfo);
        json1.insert("cnc_type",cnc_type);
        json1.insert("max_axis",QString::number(result.max_axis));
        json1.insert("mt_type",mt_type);
        json1.insert("series",QString(result.series[0])+QString(result.series[1])+QString(result.series[2])+QString(result.series[3]));
        json1.insert("version",QString(result.version[0])+QString(result.version[1])+QString(result.version[2])+QString(result.version[3]));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取CNC系统硬件信息*/
QString otherInfor::CNC_rdsyshard(DATASET dataSetInfor)
{
    ODBSYSH result;
    short hard_num = 1;
    short read_num = 1;
    QJsonDocument document;
    QJsonObject json;
    QJsonObject json1;
    QJsonObject json2;
    short ret;
    do{
      ret=cnc_rdsyshard(dataSetInfor.flibhndl.toUInt(),hard_num,&read_num,&result);
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
        json2.insert("group_id",QString::number(result.group_id));
        json2.insert("hard_id",QString::number(result.hard_id));
        json2.insert("hard_num",QString::number(result.hard_num));
        json2.insert("id1",QString::number(result.id1));
        json2.insert("id1_format",QString::number(result.id1_format));
        json2.insert("id2",QString::number(result.id2));
        json2.insert("id2_format",QString::number(result.id2_format));
        json2.insert("slot_no",QString::number(result.slot_no));
        json1.insert(QString::number(hard_num),json2);
      }
        hard_num++;
      }
    while (read_num != 0);
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;

}

/*读取CNC状态信息*/
QString otherInfor::CNC_statinfo(DATASET dataSetInfor)
{
    ODBST2 result;
    short ret=cnc_statinfo2(dataSetInfor.flibhndl.toUInt(),&result);
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
        json1.insert("alarm",QString::number(result.alarm));
        json1.insert("aut",QString::number(result.aut));
        json1.insert("edit",QString::number(result.edit));
        json1.insert("emergency",QString::number(result.emergency));
        json1.insert("ext_opt",QString::number(result.ext_opt));
        json1.insert("hdck",QString::number(result.hdck));
        json1.insert("motion",QString::number(result.motion));
        json1.insert("mstb",QString::number(result.mstb));
        json1.insert("o3dchk",QString::number(result.o3dchk));
        json1.insert("restart",QString::number(result.restart));
        json1.insert("run",QString::number(result.run));
        json1.insert("tmmode",QString::number(result.tmmode));
        json1.insert("warning",QString::number(result.warning));
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}

/*读取操作员消息*/
QString otherInfor::CNC_rdopmsg(DATASET dataSetInfor)
{
    OPMSG3 result;
    short number = 1;
    short ret=cnc_rdopmsg3(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toShort(),&number,&result);
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
        json.insert("value", QString(result.data));
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取警报状态*/
QString otherInfor::CNC_alarm(DATASET dataSetInfor)
{
    long result=0;
    short ret=cnc_alarm2(dataSetInfor.flibhndl.toUInt(),&result);
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
        QString alarm="";
         switch (result)
           {
               case 0:
                   alarm="Parameter switch on (SW)";
                   break;
               case 1:
                   alarm="Power off parameter set (PW)";
                   break;
               case 2:
                   alarm="I/O error (IO)";
                   break;
               case 3:
                   alarm="Foreground P/S (PS)";
                   break;
               case 4:
                   alarm="Overtravel,External data (OT)";
                   break;
               case 5:
                   alarm="Overheat alarm (OH)";
                   break;
               case 6:
                   alarm="Servo alarm (SV)";
                   break;
               case 7:
                   alarm="Data I/O error (SR)";
                   break;
               case 8:
                   alarm="Macro alarm (MC)";
                   break;
               case 9:
                   alarm="Spindle alarm (SP)";
                   break;
               case 10:
                   alarm="Other alarm(DS) (DS)";
                   break;
               case 11:
                   alarm="Alarm concerning Malfunction prevent functions (IE)";
                   break;
               case 12:
                   alarm="Background P/S (BG)";
                   break;
               case 13:
                   alarm="Syncronized error (SN)";
                   break;
               case 15:
                   alarm="External alarm message (EX)";
                   break;
               case 19:
                   alarm="PMC error (PC)";
                   break;
             case 32768:
                 alarm="Emergency stop press";
                 break;
           }
        QJsonDocument document;
        QJsonObject json;
        json.insert("result","ok");
        json.insert("value", alarm);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
/*读取警报消息*/
QString otherInfor::CNC_rdalmmsg(DATASET dataSetInfor)
{
    long alarn_state=0;
    short ret=cnc_alarm2(dataSetInfor.flibhndl.toUInt(),&alarn_state);
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
        ODBALMMSG2 result;
        short number = 512;
        short ret=cnc_rdalmmsg2(dataSetInfor.flibhndl.toUInt(),alarn_state,&number,&result);
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
            json.insert("value", QString(QLatin1String(result.alm_msg)));
            document.setObject(json);
            QString json_str(document.toJson(QJsonDocument::Compact));
            return json_str;
        }
    }
}

/*获取详细错误信息*/
QString otherInfor::CNC_getdtailerr(DATASET dataSetInfor)
{
    ODBERR result;
    short number = 1;
    short ret=cnc_getdtailerr(dataSetInfor.flibhndl.toUInt(),&result);
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
        json1.insert("err_no",result.err_no);
        json1.insert("err_dtno",result.err_dtno);
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}

/*读取CNC诊断数据的信息*/
QString otherInfor::CNC_rddiaginfor(DATASET dataSetInfor)
{
    ODBDIAGIF result;
    short number = 1;
    short ret=cnc_rddiaginfo(dataSetInfor.flibhndl.toUInt(),dataSetInfor.parameter1.toUInt(),dataSetInfor.parameter2.toUInt(),&result);
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
//        json1.insert("err_no",result.err_no);
//        json1.insert("err_dtno",result.err_dtno);
        json.insert("result","ok");
        json.insert("value",json1);
        document.setObject(json);
        QString json_str(document.toJson(QJsonDocument::Compact));
        return json_str;
    }
}
