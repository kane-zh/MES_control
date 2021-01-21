#include "httprequest.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QBuffer>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

httpRequest::httpRequest(QObject *parent) : QObject(parent)
{

}

QString httpRequest::login(QString address, QString name, QString passwd)
{
        QNetworkAccessManager *m_pHttpMgr = new QNetworkAccessManager(this);
        //设置url
        QString url = address+"/login";
        //设置头信息
        QNetworkRequest requestInfo;
        requestInfo.setUrl(QUrl(url));
        requestInfo.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
        requestInfo.setRawHeader("Content-Type","application/json");//服务器要求的数据头部
        requestInfo.setRawHeader("Accept","text/json,*/*;q=0.5");//服务器要求的数据头部
        //发送数据
        QJsonObject json;
        json.insert("username",name);
        json.insert("password",passwd);
        QJsonDocument document;
        document.setObject(json);
        QByteArray qByteHttpData = document.toJson(QJsonDocument::Compact);
        QNetworkReply *reply =  m_pHttpMgr->post(requestInfo, qByteHttpData);
        //添加事件循环机制，返回后再运行后面的
        QEventLoop eventLoop;
        connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        eventLoop.exec();       //block until finish
        //错误处理
        if (reply->error() != QNetworkReply::NoError)
        {
            delete  reply;
            delete  m_pHttpMgr;
            return "err";
        }
        //请求收到的结果
        QByteArray responseByte = reply->readAll();
        delete  reply;
        delete  m_pHttpMgr;
        QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(responseByte).toLocal8Bit().data());
        if( jsonDocument.isNull() ){
            return "err";
        }
        QJsonObject jsonObject = jsonDocument.object();
        return "Jwt "+jsonObject.value("token").toString();
}

QString httpRequest::get(QString address, QString token)
{
    QNetworkAccessManager *m_pHttpMgr = new QNetworkAccessManager(this);
    //设置头信息
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(address));
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    requestInfo.setRawHeader("Content-Type","application/json");//服务器要求的数据头部
    requestInfo.setRawHeader("Accept","text/json,*/*;q=0.5");//服务器要求的数据头部
    requestInfo.setRawHeader("Authorization",token.toUtf8());//服务器要求的数据头部
    QNetworkReply *reply = m_pHttpMgr->sendCustomRequest (requestInfo,QByteArray("GET"));
    //添加事件循环机制，返回后再运行后面的
    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    //错误处理
    if (reply->error() != QNetworkReply::NoError)
    {
        delete  reply;
        delete  m_pHttpMgr;
        return "err";
    }
    //请求收到的结果
    QByteArray responseByte = reply->readAll();
    delete  reply;
    delete  m_pHttpMgr;
    return QString(responseByte);
}

QString httpRequest::post(QString address, QString token, QByteArray value)
{
    QNetworkAccessManager *m_pHttpMgr = new QNetworkAccessManager(this);
    //设置头信息
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(address));
    requestInfo.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    requestInfo.setRawHeader("Content-Type","application/json");//服务器要求的数据头部
    requestInfo.setRawHeader("Accept","text/json,*/*;q=0.5");//服务器要求的数据头部
    requestInfo.setRawHeader("Authorization",token.toUtf8());//服务器要求的数据头部
    //发送数据
    QNetworkReply *reply = m_pHttpMgr->sendCustomRequest (requestInfo,QByteArray("POST"),value);
    //添加事件循环机制，返回后再运行后面的
    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();       //block until finish
    //错误处理
    if (reply->error() != QNetworkReply::NoError)
    {
        delete  reply;
        delete  m_pHttpMgr;
        return "err";
    }
    //请求收到的结果
    QByteArray responseByte = reply->readAll();
    delete  reply;
    delete  m_pHttpMgr;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(responseByte).toLocal8Bit().data());
    if( jsonDocument.isNull() ){
        return "err";
    }
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject.value("result").toString();
}
