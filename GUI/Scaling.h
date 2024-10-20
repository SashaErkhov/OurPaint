#ifndef SCALING_H
#define SCALING_H

#include <QWidget>
#include <QPoint>
#include <algorithm>
#include <cmath>

class Scaling {
private:
    double scale;      // Коэффициент при сужении окна
    double width_;     // Начальная ширина окна
    double height_;    // Начальная высота окна
    double zoom;       // Увеличение
    bool usersResize;  // Флаг масштабирования от пользователя
    int deltaX;        // Перемещение по оси X
    int deltaY;        // Перемещение по оси Y
    QPoint lastMousePos; // Предыдущее положение мыши
    bool rightMousePressed; // Флаг нажатия правой кнопки мыши

public:
    Scaling(double width, double height);

    double scaleCoordinate(double X);
    double scaleCoordinateX(double X);
    double scaleCoordinateY(double Y);

    void scaling(int widgetWidth, int widgetHeight,std::vector<double> size);

    void setZoomPlus(double maxZoom);

    void setZoomMinus();

    void getUsers(bool var){usersResize=var;}

    void setZoomZero();

    double getScale() const;

    double getZoom() const;

    int getDeltaX() const;

    int getDeltaY() const;

    void setDelta(int dx, int dy);

    void startMousePress(const QPoint& pos);

    void mouseMove(const QPoint& pos);

    void endMousePress();

    bool isRightMousePressed() const;

    bool isUsersResize() const;

    void resetUsersResize();
};

#endif // SCALING_H