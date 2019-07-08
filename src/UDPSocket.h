// UDPSocket.h

#ifndef UDPSOCKET_H

#define UDPSOCKET_H

#include <QObject>

class QUdpSocket;

namespace UDPSocket {
  struct Message {
    double x, y, z;
    int id;
  };

  QString sbemPath();
  QString gvoxPath();

  class Server: public QObject {
    Q_OBJECT;
  public:
    Server(QString path, QObject *parent=0);
    ~Server();
  signals:
    void messageReceived(Message msg);
  private:
    QString path;
    QUdpSocket *sock;
  };

  class Client: public QObject {
    Q_OBJECT;
  public:
    Client(QString path, QObject *parent=0);
    ~Client();
  public slots:
    void sendMessage(Message msg);
  private:
    QString path;
  };    
};

#endif
