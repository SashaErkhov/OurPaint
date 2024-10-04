#ifndef QTPAINTER_H
#define QTPAINTER_H

#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QFrame>
#include <vector>
#include <algorithm>
#include <cmath>
#include "paint.h"
#include "../GUI/mainwindow.h"

class QTPainter : public QFrame, public Painter {
Q_OBJECT

private:
    Ui::MainWindow *ui;
    std::vector<point> points;
    std::vector<circle> circles;
    std::vector<section> sections;
    double scale; // Коэфицент при сужении окна
    double koef; // Коэфицент при отрисовке за границами
    double width_; // Ширина окна начальная
    double height_; // Высота окна начальная
    const double margin = 0.05;  // 5% запаса

public:

    void draw() {
        update();
    }

    double getScale(){return scale;}
    void setScale(double scale);

    QTPainter(Ui::MainWindow *ui, QWidget *parent);
    void clear();

protected:
    void resizeEvent(QResizeEvent *event) override;
    //При изменении размера окна меняем его размеры

    void drawPoint(point pt, bool isWhite = false) override;

    void drawCircle(circle c, bool isWhite = false) override;

    void drawSection(section sec, bool isWhite = false) override;

    void paintEvent(QPaintEvent *event) override; // Отрисовка

    void changeSize(const rectangle &allObjects) override;

    unsigned long long getWeight() override;

    unsigned long long getHeight() override;

private slots:

    void onWorkWindowResized() ;


};

#endif // QTPAINTER_H