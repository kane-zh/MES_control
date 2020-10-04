#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QBuffer>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
class httpRequest : public QObject
{
    Q_OBJECT
public:
    explicit httpRequest(QObject *parent = nullptr);

signals:
public:
    QString  get(QString address,QString port,QString name,QString passwd,QString db,QString cmd);
    QString  post(QString address,QString port,QString name,QString passwd,QString db,QString cmd);
public slots:
};

#endif // HTTPREQUEST_H
