#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
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

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

private:
    Ui::MainWindow *ui;
    std::vector<QString> commands; // Список команд
    int Index; // Индекс для вывода команд в консоль по стрелке
    bool save; // Для сохранения файла
    QPoint lastMousePosition;
    bool resizing ;
    const int resizeMargin = 10; // Закругление окна
    bool moving ;  //Перемещение
    Qt::Edges resizingEdges; // Объявление переменной resizingEdges

public:
    MainWindow(QWidget *parent = nullptr);
    void setAllMouseTracking(QWidget *widget);
    ~MainWindow();


    void Print_LeftMenu(unsigned long long id, const std::string &text,
                        const std::vector<double> &object); // Добавление элементов в меню
    void Requar_LeftMenu(unsigned long long int id, const std::string &text); // Добавление требований

    QWidget *getWorkWindow() const {
        return ui->workWindow;
    }

    Ui::MainWindow *getUi() const {
        return ui;
    }

    void setSave(bool T){
        save=T;
    }

    void resizeEvent(QResizeEvent *event) override {
        QMainWindow::resizeEvent(event);
        emit resized();
    }

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override; // Отрисовка окна

signals:
    void EnterPressed(const QString &command); // При нажатии Enter
    void resized(); // При изменении размера окна для QPainter
    void projectSaved(const QString &fileName);
    void LoadFile(const QString &fileName);
    void REDO();
    void UNDO();

public slots:
    void saveProjectToFile();
    void LoadProjectFile();
};

#endif // MAINWINDOW_H