#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "FrameOverlay.h"
#include <QKeyEvent>
#include <QPainter>
#include <QFrame>
#include <QTreeWidgetItem>
#include <vector>
#include <string>
#include <QWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include "SaveDialog.h"
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

private:
    Ui::MainWindow *ui;
    std::vector<QString> commands; // Список команд для консоли
    int Index; // Индекс для навигации по командам
    bool resizing; // Флаг изменения размера окна
    const int resizeMargin = 10; // Коэф для рисования закруглений
    bool moving; // Флаг перемещения окна
    bool drawingFrame; // Флаг отображения рамки
    bool save; // Флаг сохранения проекта
    Qt::Edges resizingEdges; // Грани, которые изменяются при изменении размера
    QPoint lastMousePosition; // Последняя позиция мыши
    QRect frameRect; // Геометрия рамки
    FrameOverlay *frameOverlay; // Объект наложения рамки

public:
    MainWindow(QWidget *parent = nullptr);
    void setAllMouseTracking(QWidget *widget); // Отслеживание мыши
    ~MainWindow();

    // Добавление элементов в левое меню
    void Print_LeftMenu(unsigned long long id, const std::string &text, const std::vector<double> &object);

    // Добавление требований в левое меню
    void Requar_LeftMenu(unsigned long long int id, const std::string &text);

    QWidget *getWorkWindow() const { return ui->workWindow; }
    Ui::MainWindow *getUi() const { return ui; }
    void setSave(bool T) { save = T; }

    // Ловим сигнал изменения окна
    void resizeEvent(QResizeEvent *event) override {
        QMainWindow::resizeEvent(event);
        emit resized(); // Сигнал
    }

protected:
    // Обработчики событий клавиатуры и мыши
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override; // Обработка отрисовки окна

signals:
    void EnterPressed(const QString &command); // Сигнал при нажатии Enter
    void resized(); // Сигнал при изменении размера окна
    void projectSaved(const QString &fileName); // Сигнал о сохранении проекта
    void LoadFile(const QString &fileName); // Сигнал для загрузки файла
    void REDO(); // Сигнал для повторения действия
    void UNDO(); // Сигнал для отмены действия

public slots:
    void saveProjectToFile();
    void LoadProjectFile();
};

#endif // MAINWINDOW_H