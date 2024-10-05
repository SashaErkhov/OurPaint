#include "QTPainter.h"

QTPainter::QTPainter(Ui::MainWindow *ui, QWidget *parent)
        : QFrame(parent), ui(ui), scale(1.0), koef(1.0) {
    resize(parent->size());
    setStyleSheet("background: \"#ffffff\"");
    width_ = ui->workWindow->width();
    height_ = ui->workWindow->height();
    connect(ui->workWindow->window(), SIGNAL(resized()), this, SLOT(onWorkWindowResized()));
    //Отслеживаем размер окна отца
}

void QTPainter::clear() {
    points.clear();
    circles.clear();
    sections.clear();
    scale=1.0;
    koef=1.0;
    update();
}

void QTPainter::resizeEvent(QResizeEvent *event) {
    // При изменении размера окна отца, меняем размер окна
    QFrame::resizeEvent(event);
    resize(ui->workWindow->size());
    qDebug()<<"ui "<<ui->workWindow->width()<<" "<<ui->workWindow->height();
    //   update();
}

void QTPainter::drawPoint(point pt, bool isWhite) {
    points.push_back(pt);
    //update();
}

void QTPainter::drawCircle(circle c, bool isWhite) {
    circles.push_back(c);
  //  update();
}

void QTPainter::drawSection(section sec, bool isWhite) {
    sections.push_back(sec);
   // update();
}

void QTPainter::paintEvent(QPaintEvent *event) {
    double scaleX = width() / static_cast<double>(width_);
    double scaleY = height() / static_cast<double>(height_);
    scale = std::min(scaleX, scaleY) * koef;

    QFrame::paintEvent(event);
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::gray);

    // Оси координат
    painter.drawLine(width() / 2, 0, width() / 2, height());
    painter.drawLine(0, height() / 2, width(), height() / 2);
    painter.setPen(Qt::black);

    for (const auto &pt: points) {
        int X = static_cast<int>(pt.x * scale + width() / 2);
        int Y = static_cast<int>(-pt.y * scale + height() / 2);
        painter.drawPoint(X, Y);
    }

    for (const auto &c: circles) {
        int X = static_cast<int>(c.center->x * scale + width() / 2);
        int Y = static_cast<int>(-c.center->y * scale + height() / 2);
        int Radius = static_cast<int>(c.R * scale * 2);
        painter.drawEllipse(X - Radius / 2, Y - Radius / 2, Radius, Radius);
    }

    for (const auto &sec: sections) {
        int BegX = static_cast<int>(sec.beg->x * scale + width() / 2);
        int BegY = static_cast<int>(-sec.beg->y * scale + height() / 2);
        int EndX = static_cast<int>(sec.end->x * scale + width() / 2);
        int EndY = static_cast<int>(-sec.end->y * scale + height() / 2);
        painter.drawLine(BegX, BegY, EndX, EndY);
    }

}

void QTPainter::changeSize(const rectangle &allObjects) {
    // При отрисовке за границами уменьшаем размер элементов
    double maxX = std::max({std::abs(allObjects.x_1), std::abs(allObjects.x_2)});
    double maxY = std::max({std::abs(allObjects.y_1), std::abs(allObjects.y_2)});

    if (maxX >= (width() / 2) && maxY >= (height() / 2)) {
        double koefX = static_cast<double>(width()) / (2 * maxX);
        double koefY = static_cast<double>(height()) / (2 * maxY);
        koef = std::min(koefX, koefY) - margin * koef;
    } else if (maxX >= (width() / 2)) {
        koef = static_cast<double>(width()) / (2 * maxX) - margin * koef;
    } else if (maxY >= (height() / 2)) {
        koef = static_cast<double>(height()) / (2 * maxY) - margin * koef;
    }

  //  update();
}

unsigned long long QTPainter::getWeight() {
    return width();
}

unsigned long long QTPainter::getHeight() {
    return height();
}

void QTPainter::onWorkWindowResized() {
    // При изменении размера окна отца меняем размер
    resize(ui->workWindow->size());
  //  update();
}
