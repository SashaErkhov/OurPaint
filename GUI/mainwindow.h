
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
    bool save;
    bool isMousePressed = false;
    QPoint lastMousePosition;
    bool resizing = false;
    const int resizeMargin = 10;
    bool moving = false;


public:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


    bool getSave(){return save;}

    void setSave(bool T){
        save=T;
    }

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void Print_LeftMenu(unsigned long long id, const std::string &text, const std::vector<double> &object); // Добавление элементов в меню
    void Requar_LeftMenu(unsigned long long int id, const std::string &text); // Добавление требований

    QWidget *getWorkWindow() const {
        return ui->workWindow;
    }

    Ui::MainWindow *getUi() const {
        return ui;
    }

    void resizeEvent(QResizeEvent *event) { // При изменении размера окна для QTPainter
        QMainWindow::resizeEvent(event);
        emit resized();
    }

public slots:
    void saveProjectToFile();
    void LoadProjectFile();


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override ;


signals:
    void EnterPressed(const QString &command); // При нажатии кнопки
    void resized(); // При изменении размера окна для QTPAinter
    void projectSaved(const QString &fileName);
    void LoadFile(const QString &fileName);
    void REDO();
    void UNDO();


};




#endif // MAINWINDOW_H
