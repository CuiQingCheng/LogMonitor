#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>

class udpClient : public QObject
{
    Q_OBJECT
public:
    explicit udpClient(QObject *parent = 0);

signals:

public slots:
};

#endif // UDPCLIENT_H