#ifndef OURPAINT_WINDOWSERVER_H
#define OURPAINT_WINDOWSERVER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QLineEdit>

class WindowServer : public QWidget{
Q_OBJECT
    WindowServer(QWidget *parent = nullptr) : QWidget(parent){
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);

        setWindowModality(Qt::ApplicationModal);  // Для модальности только в пределах родительского окна

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(10, 10, 10, 10);

        QLineEdit line(parent);
        QPushButton *okButton = new QPushButton("OK", this);
        connect(okButton, &QPushButton::clicked, this, &WindowServer::close);
        layout->addWidget(okButton);

        setLayout(layout);
        resize(300, 150);
    }

};
#endif //OURPAINT_WINDOWSERVER_H
