#include "httprequest.h"


httpRequest::httpRequest(QObject *parent) : QObject(parent)
{

}

QString httpRequest::get(QString address, QString port, QString name, QString passwd,QString db, QString cmd)
{
    QNetworkAccessManager *m_pHttpMgr = new QNetworkAccessManager(this);
    //设置url
    QString url = address+":"+port+"/query?u="+name+"&p="+passwd+"&d="+db+"&q="+cmd;
    //设置头信息
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    requestInfo.setRawHeader("Content-Type","application/json");//服务器要求的数据头部
    requestInfo.setRawHeader("Accept","text/json,*/*;q=0.5");//服务器要求的数据头部
    QNetworkReply *reply = m_pHttpMgr->sendCustomRequest (requestInfo,QByteArray("GET"));
    //添加事件循环机制，返回后再运行后面的
    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    //错误处理
    if (reply->error() != QNetworkReply::NoError)
    {
        return "err";
    }
    //请求收到的结果
    QByteArray responseByte = reply->readAll();
    QString  dat=QString(responseByte);
    if( dat.isNull() ){
        return "err";
    }
    delete  reply;
    delete  m_pHttpMgr;
    return dat;
}

QString httpRequest::post(QString address, QString port, QString name, QString passwd,QString db, QString cmd)
{
    QNetworkAccessManager *m_pHttpMgr = new QNetworkAccessManager(this);
    //设置url
    QString url = address+":"+port+"/write?db="+db+"&u="+name+"&p="+passwd;
    //设置头信息
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    requestInfo.setRawHeader("Content-Type","application/json");//服务器要求的数据头部
    requestInfo.setRawHeader("Accept","text/json,*/*;q=0.5");//服务器要求的数据头部
    //发送数据
    QByteArray value=cmd.toUtf8();
    QBuffer    *qBuffer =new QBuffer(&value);
    QNetworkReply *reply = m_pHttpMgr->sendCustomRequest (requestInfo,QByteArray("POST"),qBuffer);
    //添加事件循环机制，返回后再运行后面的
    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish;
    //错误处理
    QString s=reply->errorString();
    if (reply->error() != QNetworkReply::NoError)
    {
        delete qBuffer;
        delete  reply;
        delete  m_pHttpMgr;
        return "err";
    }
    else{
        delete qBuffer;
        delete  reply;
        delete  m_pHttpMgr;
        return "ok";
    }
}
