#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->console, &QLineEdit::returnPressed, this, [this]() {
        QString input = ui->console->text();
        emit EnterPressed(input);
        ui->console->clear();
    });

}

MainWindow::~MainWindow() {
    delete ui;
}






