//
// Created by Eugene Bychkov on 13.10.2024.
//

#include "Client.h"
#include "Client.h"
#include <QDataStream>

Client::Client(QObject *parent) : QObject(parent), tcpSocket(new QTcpSocket(this)) {
    connect(tcpSocket, &QTcpSocket::connected, this, &Client::onConnected);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
}

void Client::connectToServer(const QString &ip, quint16 port) {
    tcpSocket->connectToHost(ip, port);
}

void Client::sendCommandToServer(const QString &command) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << command;
    tcpSocket->write(data);
}

void Client::onConnected() {
}
void Client::sendChatMessage(const QString &message) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << QString("CHAT|%1").arg(message);
    tcpSocket->write(data);
}
void Client::onReadyRead() {
    QDataStream in(tcpSocket);
    while (!tcpSocket->atEnd()) {
        QString command;
        in >> command;
        if (command.startsWith("CHAT|")) {
            QString msg = command.mid(5);
            emit newChatMessageReceived(msg, "User");
        }
        emit newStateReceived(command);
    }
}

void Client::onDisconnected() {
    emit disconnectedFromServer();
}