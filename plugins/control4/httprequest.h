#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>

class httpRequest : public QObject
{
    Q_OBJECT
public:
    explicit httpRequest(QObject *parent = nullptr);

signals:
public:
    QString  login(QString address,QString name,QString passwd);
    QString  post(QString address,QString token,QByteArray value);
    QString  get(QString address,QString token);
public slots:
};

#endif // HTTPREQUEST_H
