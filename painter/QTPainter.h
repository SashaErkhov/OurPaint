#ifndef QTPAINTER_H
#define QTPAINTER_H

#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QFrame>
#include <vector>
#include "paint.h"
#include "../GUI/mainwindow.h"
#include "../GUI/Scaling.h"

class QTPainter : public QFrame, public Painter {
Q_OBJECT

private:
    Ui::MainWindow *ui;
    std::vector<point> points;
    std::vector<circle> circles;
    std::vector<section> sections;
    Scaling Scaling;

    const int CellSize;    // Изначальный размер клетки при отрисовке
    const int maxCellSize; // Максимальный размер клетки при масштабировании
    const int minCellSize; // Минимальный размер клетки при масштабировании
    int currentCellSize;   // Текущий размер клетки
    bool CellView;         // Флаг отрисовки сетки

public:
    // Функция включения сетки
    void setCell(bool On_Off);

    void draw();

    void getUsers(bool var) { Scaling.getUsers(var); }

    void setZoomPlus();

    void setZoomMinus();

    void setZoomZero();

    QTPainter(Ui::MainWindow *ui, QWidget *parent);

    void clear(); // Очистка

    std::vector<double> FindMaxMin();

protected:
    void resizeEvent(QResizeEvent *event) override;

    void drawPoint(point pt, bool isWhite = false) override;

    void drawCircle(circle c, bool isWhite = false) override;

    void drawSection(section sec, bool isWhite = false) override;

    void paintEvent(QPaintEvent *event) override; // Отрисовка

    void changeSize(const rectangle &allObjects) override;

    unsigned long long getWeight() override;

    unsigned long long getHeight() override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;


signals:

    void RightPress();



private slots:

    void onWorkWindowResized();


};

#endif // QTPAINTER_H