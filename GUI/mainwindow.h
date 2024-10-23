#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
#include <QGuiApplication>
#include <QScreen>
#include <QGestureEvent>

#include "Windows/help.h"
#include "Windows/SaveDialog.h"
#include "Windows/WindowServer.h"
#include "ui_mainwindow.h"
#include "FrameOverlay.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

private:
    Ui::MainWindow *ui;
    std::vector<QString> commands; // Буфер команд для консоли
    const int resizeMargin = 10; // Коэф для рисования закруглений
    int Index; // Индекс для навигации по командам
    bool moving; // Флаг перемещения окна
    bool drawingFrame; // Флаг отображения рамки
    bool save; // Флаг сохранения проекта
    bool resizing; // Флаг изменения размера окна
    bool addElem; // Флаг для изменеия обьектов левого меню
    Qt::Edges resizingEdges; // Грани, которые изменяются при изменении размера
    QPoint lastMousePosition; // Последняя позиция мыши
    QRect frameRect; // Геометрия рамки
    FrameOverlay *frameOverlay; // Объект наложения рамки
    Help *helpWindow; // Окно справки

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void setAllMouseTracking(QWidget *widget); // Отслеживание мыши

    // Добавление элементов в левое меню
    void Print_LeftMenu(unsigned long long id, const std::string &text, const std::vector<double> &object);

    // Добавление требований в левое меню
    void Requar_LeftMenu(unsigned long long int id, const std::string &text);

    QWidget *getWorkWindow() const { return ui->workWindow; }

    Ui::MainWindow *getUi() const { return ui; }

    void setSave(bool T) {
        save = T;
    }

    void showHelp();


protected:
    // Обработчики событий клавиатуры и мыши
    void keyPressEvent(QKeyEvent *event) override;

    // Закрытие
    void closeEvent(QCloseEvent *event) override;

    // Нажатие мыши
    void mousePressEvent(QMouseEvent *event) override;

    // Перемещение мыши
    void mouseMoveEvent(QMouseEvent *event) override;

    // Отпускание мыши
    void mouseReleaseEvent(QMouseEvent *event) override;

    // Двойное нажатие клавиши
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    // Изменение размеров
    void resizeEvent(QResizeEvent *event) override;

    // Обработка отрисовки окна
    void paintEvent(QPaintEvent *event) override;

    // Кручение колёсиком или тачпадом
    void wheelEvent(QWheelEvent *event) override;

    // Жесты в тачпаде в области workWindow
    bool event(QEvent *event) override;

signals:

    void EnterPressed(const QString &command); // Сигнал при нажатии Enter
    void resized(); // Сигнал при изменении размера окна
    void projectSaved(const QString &fileName); // Сигнал о сохранении проекта
    void LoadFile(const QString &fileName); // Сигнал для загрузки файла

    void KeyPlus(); // Сигнал увелечения при тачпаде,колёсике и ctrl +
    void KeyMinus(); // Сигнал уменьшения при тачпаде,колёсике и ctrl -
    void KeyZero(); // Обнуление
    void REDO(); // Сигнал для повторения действия
    void UNDO(); // Сигнал для отмены действия

    void ChangeLeftMenu(); // Сигнал изменения параметров левого меню
    void parameterChanged(unsigned long long id, const std::vector<double> &parameters);

    // Кнопки сервера
    void SigOpenServer(const QString &text);
    void SigJoinServer(const QString &text);
    void SigJoinLocalServer(const QString &text);
    void SigExitSession();

    // Чат
    void EnterMessage(const QString &text);

    //Настройки
    void GridOn(bool T);


public slots:

     // Кнопки
    void saveProjectToFile();
    void LoadProjectFile();
    void openServer();
    void joinServer();
    void joinLocalServer();

    void exitSession() {
        emit SigExitSession();
    }

    void LeftMenuChanged(QTreeWidgetItem *item);


};

#endif // MAINWINDOW_H