// UDPSocket.cpp

#include "UDPSocket.h"
#include <QUdpSocket>
#include <QDir>
#include <QNetworkDatagram>

namespace UDPSocket {

  QString sbemPath() {
    return QDir::home().absoluteFilePath(".local/sbemviewer.udp");
  }

  QString gvoxPath() {
    return QDir::home().absoluteFilePath(".local/gvox.udp");
  }

  Server::Server(QString path, QObject *parent): QObject(parent), path(path) {
    sock = new ::QUdpSocket(this);
    QFile f(path);
    if (!f.open(QFile::WriteOnly)) {
      qDebug() << "Failed to post socket port";
      return;
    }
    QTextStream ts(&f);
    int port = sock->bind(QHostAddress::LocalHost, 0,
                          QUdpSocket::DefaultForPlatform)
      ? sock->localPort() : 0;
    ts << QString("%1\n").arg(port);
    connect(sock, &QUdpSocket::readyRead,
            [this]() {
              while (sock->hasPendingDatagrams()) {
                QByteArray data(sock->receiveDatagram().data());
                if (data.size() != sizeof(Message)) {
                  qDebug() << "Invalid datagram of size" << data.size();
                  return;
                }
                Message const *msg
                  = reinterpret_cast<Message const *>(data.data());
                qDebug() << "Got message"
                         << msg->x << msg->y << msg->z << msg->id;
                messageReceived(*msg);
              }
            });
  }

  Server::~Server() {
    QDir::root().remove(path);
  }
  
  Client::Client(QString path, QObject *parent): QObject(parent), path(path) {
  }
  
  Client::~Client() {
  }

  void Client::sendMessage(Message msg) {
    QFile f(path);
    if (!f.open(QFile::ReadOnly)) {
      qDebug() << "No server for" << path;
      return;
    }
    QTextStream ts(&f);
    int port = 0;
    ts >> port;
    if (port<=0) {
      qDebug() << "Invalid port" << port << "for" << path;
      return;
    }
    QUdpSocket sock;
    QByteArray data(reinterpret_cast<char const *>(&msg), sizeof(msg));
    sock.writeDatagram(data, QHostAddress::LocalHost, port);
  }
};

