#include "udpclient.h"

#define HOSTADDR "10.0.1.8"

const quint16  SENDPORT = 58823;
udpClient* udpClient::m_client = new udpClient;

void udpClient::setMessage(QByteArray msg)
{
    m_udpSocket->writeDatagram(msg, msg.size(), QHostAddress(HOSTADDR), SENDPORT);
}
