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
private:
    QLineEdit *lineEdit;
signals:
    void textEnter(const QString &text);

private slots:
    void OkClicked() {
        QString text = lineEdit->text();
        if(!text.isEmpty()) {
            emit textEnter(text);
        }
        close();
    }
public:
    WindowServer(QWidget *parent = nullptr) : QWidget(parent){
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);

        setWindowModality(Qt::ApplicationModal);  // Для модальности только в пределах родительского окна

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(10, 10, 10, 10);

        lineEdit = new QLineEdit(this);
        layout->addWidget(lineEdit);

        QPushButton *okButton = new QPushButton("OK", this);
        connect(okButton, &QPushButton::clicked, this, &WindowServer::OkClicked);
        layout->addWidget(okButton);

        setLayout(layout);
        resize(300, 150);
    }
protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QColor(50, 50, 50, 240));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(rect(), 10, 10);
    }
};
#endif //OURPAINT_WINDOWSERVER_H
