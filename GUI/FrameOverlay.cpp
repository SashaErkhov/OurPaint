#include "FrameOverlay.h"

FrameOverlay::FrameOverlay(QWidget *parent)
        : QWidget(parent), frameColor(Qt::white) {

    setAttribute(Qt::WA_TransparentForMouseEvents); // Игнор события мыши
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);// Устанавливает рамку всегда сверху
    setAttribute(Qt::WA_NoSystemBackground);  // Прозрачный фона
    setAttribute(Qt::WA_TranslucentBackground);
}



// Устанавливает прямоугольник рамки и обновляет цвет
void FrameOverlay::setFrameRect(const QRect &rect) {
    frameRect = rect; // Сохраняем новый прямоугольник
    updateFrameColor(); // Обновляем цвет рамки
    update();
}



// Обновляет цвет рамки в зависимости от фона
void FrameOverlay::updateFrameColor() {
    QScreen *screen = QGuiApplication::primaryScreen();// Фиксируем область рамки
    QPixmap screenPixmap = screen->grabWindow(0, frameRect.x(), frameRect.y(), frameRect.width(), frameRect.height());
    QImage image = screenPixmap.toImage();

    int totalPixels = image.width() * image.height();

    if (totalPixels == 0) { // Установка цвета
        frameColor = Qt::white;
        return;
    }

    qint64 sumR = 0, sumG = 0, sumB = 0;

    // Вычисляем цвет
    int step = qMax(1, qMin(image.width(), image.height()) / 100);

    for (int y = 0; y < image.height(); y += step) {
        for (int x = 0; x < image.width(); x += step) {
            QColor color = image.pixelColor(x, y); // Получаем цвет пикселя
            sumR += color.red();
            sumG += color.green();
            sumB += color.blue();
        }
    }

    // Количество пикселей
    int samplePixels = ((image.width() + step - 1) / step) * ((image.height() + step - 1) / step);

    // Вычисление среднего значения
    int avgR = sumR / samplePixels;
    int avgG = sumG / samplePixels;
    int avgB = sumB / samplePixels;

    // Вычисление яркости
    double backgroundLuminance = (0.2126 * avgR + 0.7152 * avgG + 0.0722 * avgB) / 255.0;

    // Яркость черного и белого цветов
    double blackLuminance = 0.0;
    double whiteLuminance = 1.0;

    // Вычисление контрастности
    double contrastWithBlack = (backgroundLuminance + 0.05) / (blackLuminance + 0.05);
    double contrastWithWhite = (whiteLuminance + 0.05) / (backgroundLuminance + 0.05);

    // Устанавливаем цвет рамки в зависимости от контраста
    if (contrastWithBlack > contrastWithWhite) {
        frameColor = Qt::black;
    } else {
        frameColor = Qt::white;
    }
}




// Обработчик события рисования
void FrameOverlay::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event); // Игнор

    QPainter painter(this);
    QPen pen(frameColor, 3, Qt::DashLine);
    painter.setPen(pen);

    // Рисуем прямоугольник рамки
    painter.drawRect(frameRect.translated(-geometry().topLeft()));
}