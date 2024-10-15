//
// Created by Eugene Bychkov on 13.10.2024.
//

#ifndef OURPAINT_SERVER_H
#define OURPAINT_SERVER_H


#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QSet>
#include <QDataStream>
#include "../painter/paint.h"

class Server : public QObject {
Q_OBJECT

private:
    QTcpServer *tcpServer;
    QSet<QTcpSocket *> clients;

public:
    explicit Server(QObject *parent = nullptr);

    bool startServer(quint16 port);

    void sendToClients(const QString& paint);

signals:

    void newCommandReceived(const QString &command);

private slots:

    void onNewConnection();

    void onReadyRead();

    void onClientDisconnected();
};


#endif //OURPAINT_SERVER_H
