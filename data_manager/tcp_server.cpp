#include "tcp_server.h"
#include <QMutex>

TcpServer* TcpServer::_instance=nullptr;

TcpServer& TcpServer::instance(QObject *parent){
    if(_instance==nullptr){
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if(_instance==nullptr){
            _instance=new TcpServer(parent);
        }
    }
    return *_instance;
}

bool TcpServer::startListening(const QHostAddress &address, quint16 port,
                          const OnRead& onRead){
    _communication->setOnRead(onRead);
    return QTcpServer::listen(address, port);
}

void TcpServer::_newConnectionSlot()
{
    _currentClient = nextPendingConnection();
    _tcpClients.append(_currentClient);
    _communication->readFor(_currentClient);
    connect(_currentClient, &QAbstractSocket::disconnected, this, &TcpServer::_onDisconnectedSlot);
}

void TcpServer::_onDisconnectedSlot()
{
    const unsigned length=_tcpClients.length();
    for(size_t i=0; i<length; ++i)
    {
        if(_tcpClients[i]->state() == QAbstractSocket::UnconnectedState)
        {
             emit _tcpClients[i]->destroyed();
             _tcpClients.removeAt(i);
        }
    }
}

qint64 TcpServer::sendAll(const QString& data)
{
    qint64 slen=0;
    const unsigned length=_tcpClients.length();
    for(size_t i=0; i<length; ++i){
        slen=_communication->send(_tcpClients[i],data);
    }
    return slen;
}

qint64 TcpServer::sendTo(QTcpSocket *tcpClient,const QString& data)
{
    qint64 slen;
    slen=_communication->send(tcpClient,data);
    return slen;
}
