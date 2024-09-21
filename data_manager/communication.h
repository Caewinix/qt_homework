#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <QTcpSocket>
#include <functional>

typedef QString PeerAddress;
typedef quint16 PeerPort;
typedef QTcpSocket* Sender;
typedef QString Data;

typedef std::function<void(const PeerAddress&, const PeerPort&, Sender&, const Data&)> OnRead;

class Communication:public QObject
{
public:
    /// To get the singleton of this class.
    static Communication* instance();

    /// Pass in the data to be sent and the socket to process it,
    /// which is used to send data to the specified client.
    /// Returns the number of bytes of data actually sent. If it fails, it will return -1.
    qint64 send(QTcpSocket* socket,const QString& data) const;

    /// Pass in the socket, so that it can respond when it receives data.
    void readFor(const QTcpSocket* socket);

    /// Set a function that is currently shared by all sockets to respond when receiving data.
    void setOnRead(const OnRead& onRead);

    /// Delete the response function and reset it.
    void resetOnRead();

    /// Most of the pointers are not needed to be deleted
    /// because their life cycle runs through the whole program execution period,
    /// their resources will be released by the system when the program exits.
    ~Communication(){ resetOnRead(); }
private:
    static Communication* _instance;
    const OnRead* _onRead;
private slots:
    /// When readyRead is emitted, it will be called to call the response function.
    void _onReadSlot();
};

#endif // COMMUNICATION_H
