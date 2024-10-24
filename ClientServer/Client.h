//
// Created by Eugene Bychkov on 13.10.2024.
//

#ifndef OURPAINT_CLIENT_H
#define OURPAINT_CLIENT_H

#include <QTcpSocket>
#include <QObject>
#include "../painter/paint.h"

class Client : public QObject {
Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);

    void connectToServer(const QString &ip, quint16 port);
    void sendCommandToServer(const QString &command);
    void sendChatMessage(const QString &message);

signals:
    void newChatMessageReceived(const QString& message, const QString& user);
    void newStateReceived(const QString& command);
    void disconnectedFromServer();

private slots:
    void onConnected();

    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket *tcpSocket;
};


#endif //OURPAINT_CLIENT_H
