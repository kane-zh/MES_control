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
    QString  update(QString address,QString token,QByteArray value);
public slots:
};

#endif // HTTPREQUEST_H
