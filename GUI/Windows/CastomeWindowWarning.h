/*
 *
 *  Окошко предупреждение (уведомление)
 *
 * */


#ifndef OURPAINT_CATOMEWINDOWWARNING_H
#define OURPAINT_CATOMEWINDOWWARNING_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <QPropertyAnimation>
#include <QMessageBox>

class CastomeWindowWarning : public QWidget {
Q_OBJECT

public:
    CastomeWindowWarning(const QString &message, QWidget *parent = nullptr) : QWidget(parent) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);

        QHBoxLayout *hLayout = new QHBoxLayout(this);
        hLayout->setContentsMargins(10, 10, 10, 10);

        QLabel *label = new QLabel(message, this);
        label->setStyleSheet("color: #D8D8F6;");
        hLayout->addWidget(label);

        QPixmap iconPixmap("../Static/icons/warning.ico");
        QLabel *iconLabel = new QLabel(this);
        iconLabel->setPixmap(iconPixmap.scaled(32, 32, Qt::KeepAspectRatio)); // Масштабируем иконку
        hLayout->addWidget(iconLabel);

        setLayout(hLayout);
        updatePosition(parent);

        QTimer::singleShot(2000, this, &CastomeWindowWarning::close);
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
        animation->setDuration(500); // Устанавливает длительность анимации
        animation->setStartValue(0);  // Начальная прозрачность
        animation->setEndValue(1);    // Конечная прозрачность
        animation->start();
    }
};

#endif //OURPAINT_CATOMEWINDOWWARNING_H
