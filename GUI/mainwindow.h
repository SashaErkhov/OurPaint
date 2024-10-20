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
#include "Help.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

private:

    Help *helpWindow;
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
    bool addElem;



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

    void setSave(bool T) { save = T; }

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

    void paintEvent(QPaintEvent *event) override; // Обработка отрисовки окна

    // Кручение колёсиком или тачпадом
    void wheelEvent(QWheelEvent *event) override;

    // Если курсор в WorkWindow
    bool event(QEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

signals:

    void EnterPressed(const QString &command); // Сигнал при нажатии Enter
    void resized(); // Сигнал при изменении размера окна
    void projectSaved(const QString &fileName); // Сигнал о сохранении проекта
    void NoSaved();
    void LoadFile(const QString &fileName); // Сигнал для загрузки файла
    void NoLoadFile();
    void KeyPlus(); // Сигнал увелечения при тачпаде,колёсике и ctrl +
    void KeyMinus(); // Сигнал уменьшения при тачпаде,колёсике и ctrl -
    void KeyZero(); // Обнуление
    void REDO(); // Сигнал для повторения действия
    void UNDO(); // Сигнал для отмены действия
    void PressRightMouse(); // Нажатие правой клавиши в области workWindow
    void KeyPress();
    void CloseWindow();
    void NoCloseWindow();
    void positionChanged();
    void  ChangeLeftMenu();

    void SigOpenServer(const QString &text);
    void SigJoinServer(const QString &text);
    void SigJoinLocalServer(const QString &text);
    void SigExitSession();
    void parameterChanged(unsigned long long id, const std::vector<double> &parameters);

    void EnterMessage(const QString &text);



public slots:

    void updateDrawing() {
        emit CloseWindow();
        update();
    }

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