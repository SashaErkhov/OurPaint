#ifndef OURPAINT_CATOMEWINDOWSUCCESSFUL_H
#define OURPAINT_CATOMEWINDOWSUCCESSFUL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QPropertyAnimation>

class CastomeWindowSuccessful : public QWidget {
Q_OBJECT

public:
    CastomeWindowSuccessful(const QString &message, QWidget *parent = nullptr) : QWidget(parent) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);

        setWindowModality(Qt::ApplicationModal);  // Для модальности только в пределах родительского окна

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(10, 10, 10, 10);

        QLabel *label = new QLabel(message, this);
        layout->addWidget(label);

        QPushButton *okButton = new QPushButton("OK", this);
        connect(okButton, &QPushButton::clicked, this, &CastomeWindowSuccessful::close);
        layout->addWidget(okButton);

        setLayout(layout);
        resize(300, 150);
        updatePosition(parent);
    }

public slots:
    void updatePosition(QWidget *parent) {
        if (parent) {
            int x = parent->x() + (parent->width() - width()) / 2;
            int y = parent->y() + (parent->height() - height()) / 2;
            move(x, y);
        }
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QColor(50, 50, 50, 240));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(rect(), 10, 10);
    }

    void showEvent(QShowEvent *event) override {
        QWidget::showEvent(event);
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(500);// Устанавливает длительность анимации
        animation->setStartValue(0);  // Начальная прозрачность
        animation->setEndValue(1);    // Конечная прозрачность
        animation->start();
    }
};

#endif //OURPAINT_CATOMEWINDOWSUCCESSFUL_H
