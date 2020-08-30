#ifndef METADATA_H
#define METADATA_H
#include <QtSerialPort/QSerialPort>
#include <QString>

/*串口信息*/
struct  SERIALINFOR
{
    QString name;       //名称
    bool    enable;     //使能
    QString desc;       //备注
    QString com;        //端口号
    qint32 baudRate;    //串口波特率
    QString   stringbaudRate;
    QSerialPort::Parity parity;
    QString  stringparity;
    QSerialPort::DataBits dataBits;
    QString  stringdataBits;
    QSerialPort::StopBits stopBits;
    QString stringstopBits;
    QString responseTime = "1000";
    QString numberOfRetries = "3";
    QSerialPort *serialDevice = nullptr;
 };
/*mes信息*/
struct  MESINFOR
{
    QString name;          //名称
    QString projectcode;     //项目编码
    QString floorcode;     //楼层编码
    QString linename;      //线体名称
    QString producttype;   //产品类型
 };
#endif // METADATA_H
