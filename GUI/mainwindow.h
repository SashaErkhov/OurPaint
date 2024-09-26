#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;

public:
    QWidget* getWorkWindow() const {
        return ui->workWindow;
    }
    QWidget* getCentralWindow() const {
        return ui->centralwindow;
    }
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void EnterPressed(const QString &command);



};
#endif // MAINWINDOW_H
