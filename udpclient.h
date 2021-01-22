#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QString>

class udpClient : public QObject
{
    Q_OBJECT
private:
 udpClient()
 {
    m_udpSocket = new QUdpSocket;
 }
 static udpClient* m_client;
 QUdpSocket* m_udpSocket;

public:
 static udpClient* getSingleton()
 {
     return m_client;
 }
 void setMessage(QByteArray msg);

};

#endif // UDPCLIENT_H
