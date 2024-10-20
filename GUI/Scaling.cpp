#include "../GUI/Scaling.h"

Scaling::Scaling(double width, double height)
        : scale(1.0), width_(width), height_(height), zoom(1.0), usersResize(false),
          deltaX(0), deltaY(0), rightMousePressed(false) {}


double Scaling::scaleCoordinate(double X) {
    return X * scale * zoom;
}

double Scaling::scaleCoordinateX(double X) {
    return X * scale * zoom + deltaX;
}

double Scaling::scaleCoordinateY(double Y) {
    return Y * scale * zoom + deltaY;
}

void Scaling::scaling(int widgetWidth, int widgetHeight, std::vector<double> size) {
    double scaleX = widgetWidth / width_;
    double scaleY = widgetHeight / height_;
    scale = std::min(scaleX, scaleY);

    if (!usersResize && !size.empty()) {
        deltaX = 0;
        deltaY = 0;
        // Вычисление границ окна в координатах
        int widthX = static_cast<int>(widgetWidth / (2 * scale * zoom));
        int heightY = static_cast<int>(widgetHeight / (2 * scale * zoom));

        double maxX = size[0];
        double minX = size[1];
        double maxY = size[2];
        double minY = size[3];

        double scaleMaxX = (maxX / (scale * zoom));
        double scaleMaxY = (maxY / (scale * zoom));
        double scaleMinX = (minX / (scale * zoom));
        double scaleMinY = (minY / (scale * zoom));

        double ZoomMaxX = abs(widthX / scaleMaxX);
        double ZoomMinX = abs(widthX / scaleMinX);
        double ZoomMaxY = abs(heightY / scaleMaxY);
        double ZoomMinY = abs(heightY / scaleMinY);

        double newZoomX = std::min(ZoomMaxX, ZoomMinX);
        double newZoomY = std::min(ZoomMaxY, ZoomMinY);
        double newZoom = std::min(newZoomX, newZoomY);


        if (newZoom < zoom) {
            const double margin = 1.1;
            zoom = newZoom / margin;
        }
    }


}

void Scaling::setZoomPlus(double maxZoom) {
    usersResize = true;
    zoom *= 1.1;
    if (zoom > maxZoom) zoom = maxZoom;
}

void Scaling::setZoomMinus() {
    usersResize = true;
    zoom /= 1.1;
}

void Scaling::setZoomZero() {
    usersResize = true;
    zoom = 1.0;
    deltaX = 0;
    deltaY = 0;
}

double Scaling::getScale() const {
    return scale;
}

double Scaling::getZoom() const {
    return zoom;
}

int Scaling::getDeltaX() const {
    return deltaX;
}

int Scaling::getDeltaY() const {
    return deltaY;
}

void Scaling::setDelta(int dx, int dy) {
    deltaX += dx;
    deltaY += dy;
}

void Scaling::startMousePress(const QPoint &pos) {
    rightMousePressed = true;
    lastMousePos = pos;
}

void Scaling::mouseMove(const QPoint &pos) {
   // usersResize = true;
    if (rightMousePressed) {
        int deltaX_ = pos.x() - lastMousePos.x();
        int deltaY_ = pos.y() - lastMousePos.y();
        setDelta(deltaX_, deltaY_);
        lastMousePos = pos;
    }
}

void Scaling::endMousePress() {
    rightMousePressed = false;
}

bool Scaling::isRightMousePressed() const {
    return rightMousePressed;
}

bool Scaling::isUsersResize() const {
    return usersResize;
}

void Scaling::resetUsersResize() {
    usersResize = false;
}