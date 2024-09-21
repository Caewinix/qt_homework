#include "communication.h"
#include <QHostAddress>
#include <QMutex>

Communication* Communication::_instance=nullptr;

Communication* Communication::instance(){
    if(_instance==nullptr){
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if(_instance==nullptr){
            _instance = new Communication();
        }
    }
    return _instance;
}

qint64 Communication::send(QTcpSocket* socket,const QString& data) const{
    return socket->write(data.toUtf8());
}

void Communication::readFor(const QTcpSocket* socket){
    connect(socket, &QIODevice::readyRead, this, &Communication::_onReadSlot);
}

void Communication::setOnRead(const OnRead& onRead){
    _onRead=new OnRead(onRead);
}

void Communication::resetOnRead(){
    delete _onRead;
    _onRead=nullptr;
}

void Communication::_onReadSlot(){
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray buffer = socket->readAll();
    (*_onRead)(socket->peerAddress().toString(),socket->peerPort(),socket,QString::fromUtf8(buffer));
}
