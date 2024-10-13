/*
 *
 * // Класс для создания наложения рамки на виджет при перемещении или масштабированнии
 *    Мы сначала перемещаем её,  затем окно принимает ращмер или положение рамки
 *
 *
 * */


#ifndef FRAMEOVERLAY_H
#define FRAMEOVERLAY_H

#include <QWidget>
#include <QRect>
#include <QColor>
#include <QPainter>
#include <QScreen>
#include <QGuiApplication>


class FrameOverlay : public QWidget
{
Q_OBJECT
public:
    explicit FrameOverlay(QWidget *parent = nullptr);
    void setFrameRect(const QRect &rect); // Устанавливает прямоугольник рамки

protected:
    void paintEvent(QPaintEvent *event) override;     // Обработчик события рисования

private:
    QRect frameRect;  // Прямоугольник рамки
    QColor frameColor; // Цвет рамки
    void updateFrameColor(); // Обновляет цвет рамки на основе фона
};

#endif // FRAMEOVERLAY_H