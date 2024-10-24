//
// Created by Eugene Bychkov on 13.10.2024.
//

#include "Server.h"

Server::Server(QObject *parent) : QObject(parent), tcpServer(new QTcpServer(this)) {
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

bool Server::startServer(quint16 port) {
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start!";
        return false;
    }
    qDebug() << "Server started!";
    return true;
}

void Server::sendToClients(const QString &command) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << command;

    for (QTcpSocket *client : clients) {
        client->write(data);
    }
}

void Server::onNewConnection() {
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    clients.insert(clientSocket);

    connect(clientSocket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);
}

void Server::onReadyRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    QDataStream in(clientSocket);

    while (!clientSocket->atEnd()) {
        QString command;
        in >> command;
        emit newCommandReceived(command);
    }
}
void Server::onClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        clients.remove(clientSocket);
        clientSocket->deleteLater();
    }
}