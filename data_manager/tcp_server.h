#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include <QTcpServer>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDebug>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QIODevice>
#include "communication.h"

class TcpServer:public QTcpServer{
public:
    QList<QHostAddress> localaddrs;

    /// To get the singleton of this class, and set its parent.
    static TcpServer& instance(QObject *parent = nullptr);

    /// Let the server start listening to the passed-in IP address and port number,
    /// and pass in the response function that will be called when the server receives data.
    bool startListening(const QHostAddress &address, quint16 port, const OnRead& onRead);

    /// Let the server send the passed-in data to all sockets that have been connected.
    /// Returns the number of bytes of data actually sent. If it fails, it will return -1.
    qint64 sendAll(const QString& data);

    /// Let the server send the passed-in data to the passed-in socket.
    /// Returns the number of bytes of data actually sent. If it fails, it will return -1.
    qint64 sendTo(QTcpSocket* tcpClient,const QString& data);

    // Most of the pointers do not need to be deleted
    // because their life cycle runs through the whole program execution period,
    // their resources will be released by the system when the program exits.
private:
    TcpServer(QObject *parent = nullptr)
        :QTcpServer(parent),_communication(Communication::instance())
    { connect(this, &QTcpServer::newConnection, this, &TcpServer::_newConnectionSlot); }

    Communication* _communication;
    static TcpServer* _instance; // undelete
    QList<QTcpSocket*> _tcpClients;
    QTcpSocket *_currentClient;
private slots:
    void _newConnectionSlot();
    void _onDisconnectedSlot();
};

#endif // TCP_SERVER_H
