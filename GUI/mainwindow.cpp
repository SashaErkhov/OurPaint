#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), Index(-1),save(true) {

    ui->setupUi(this);
    setMouseTracking(true); // Включаем отслеживание мыши

    setWindowTitle("Приложение Евгения Бычкова");

    commands = {"exit", "point 0 0", "circle 0 0 10", "section 0 0 10 10", "clear"};

    connect(ui->actionSave_project_to, &QAction::triggered, this, &MainWindow::saveProjectToFile);
    connect(ui->actionImport_project, &QAction::triggered, this, &MainWindow::LoadProjectFile);

    connect(ui->console, &QLineEdit::returnPressed, this, [this]() {
        QString input = ui->console->text();
        emit EnterPressed(input);
        ui->console->clear();
    });


}




void MainWindow::closeEvent(QCloseEvent *event) {
    if (!save) {
        SaveDialog dialog(this);
        int result = dialog.exec();

        if (result == QMessageBox::Yes) {
            saveProjectToFile();
            if (save) { event->accept(); }
            else{
                event->ignore();
            }
        } else if (result == QMessageBox::No) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}




void MainWindow::LoadProjectFile() {

    if (!save) {
        SaveDialog dialog(this);
        int result = dialog.exec();

        if (result == QMessageBox::Yes) {
            saveProjectToFile();
        } else if (result == QMessageBox::Cancel) {
            return;
        }
    }


    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),
                                                    QDir::homePath() + "/OurPaint/project",
                                                    tr("Project Files (*.ourp);;All Files (*)"));

    if (!fileName.isEmpty()) {
        emit LoadFile(fileName);
    }

}




void MainWindow::saveProjectToFile() {
    QString baseName = "project";
    QString extension = ".ourp";
    QString fileName;
    int index = 1;

    QString defaultDir = QDir::homePath() + "/OurPaint/project";

    QDir dir(defaultDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    fileName = QString("%1/%2%3").arg(defaultDir, baseName, extension);

    while (QFile::exists(fileName)) {
        fileName = QString("%1/%2_%3%4").arg(defaultDir).arg(baseName).arg(index).arg(extension);
        index++;
    }

    QString selectedFileName = QFileDialog::getSaveFileName(this, tr("Save Project"), fileName,
                                                            tr("Project Files (*.ourp);;All Files (*)"));

    if (!selectedFileName.isEmpty()) {
        save = true;
        emit projectSaved(selectedFileName);
    } else {
        save = false;
    }


}




void MainWindow::Print_LeftMenu(unsigned long long id, const std::string &text, const std::vector<double> &object) {
    // Добавление элементов в левое меню

    QTreeWidgetItem *itemFigures = ui->leftMenu->topLevelItem(0);

    if (text == "Clear") {
        itemFigures->takeChildren();
        return;
    }

    QString figureName = QString::fromStdString(text);
    int count = 1;

    while (true) {
        bool ex = false;
        for (int i = 0; i < itemFigures->childCount(); ++i) {
            if (itemFigures->child(i)->text(0) == figureName) {
                ex = true;
                break;
            }
        }

        if (!ex) {
            break;
        }

        figureName = QString("%1%2").arg(QString::fromStdString(text)).arg(count);
        count++;
    }

    QTreeWidgetItem *itemFigure = new QTreeWidgetItem(itemFigures);
    itemFigure->setText(0, figureName);
    itemFigures->addChild(itemFigure);

    std::vector<QString> paramNames;

    if (text == "Point" && object.size() == 2) {
        paramNames = {"ID", "X", "Y"};
    } else if (text == "Circle" && object.size() == 3) {
        paramNames = {"ID", "X", "Y", "R"};
    } else if (text == "Section" && object.size() == 4) {
        paramNames = {"ID", "X1", "Y1", "X2", "Y2"};
    }

    for (size_t i = 0; i < paramNames.size() && i < object.size() + 1; ++i) {
        QTreeWidgetItem *paramItem = new QTreeWidgetItem(itemFigure);
        if (paramNames[i] == "ID") {
            paramItem->setText(0, QString("%1: %2").arg(paramNames[i]).arg(id));
        } else {
            paramItem->setText(0, QString("%1: %2").arg(paramNames[i]).arg(QString::number(object[i - 1], 'f', 6)));
        }
        itemFigure->addChild(paramItem);
    }

    itemFigure->setExpanded(true);
}




void MainWindow::Requar_LeftMenu(unsigned long long id, const std::string &text) {
    // Добавление Требований в левое меню
    QTreeWidgetItem *itemReq = ui->leftMenu->topLevelItem(0);

    if (text == "Clear") {
        itemReq->takeChildren();
        return;
    }

    QString itemType = QString::fromStdString(std::to_string(id) + ": " + text);
    QTreeWidgetItem *newItem = new QTreeWidgetItem(itemReq);
    newItem->setText(0, itemType);

    itemReq->addChild(newItem);
    ui->leftMenu->expandAll();
}





void MainWindow::keyPressEvent(QKeyEvent *event) {
    // Перелистывание команд стрелочкой
    if (ui->console->isActiveWindow()) {
        if (event->key() == Qt::Key_Up) {
            if (Index < static_cast<int>(commands.size()) - 1) {
                ++Index;
                ui->console->setText(commands[commands.size() - 1 - Index]);
            }
        } else if (event->key() == Qt::Key_Down) {
            if (Index > 0) {
                --Index;
                ui->console->setText(commands[commands.size() - 1 - Index]);
            } else {
                Index = -1;
                ui->console->clear();
            }
        }
    }
    QWidget::keyPressEvent(event);
}



MainWindow::~MainWindow()
{
    delete ui;
}
/*********************************************************************/

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    // Проверка, находится ли курсор на краю окна
    if (event->x() >= width() - resizeMargin && event->y() >= height() - resizeMargin) {
        setCursor(Qt::SizeFDiagCursor); // Правый нижний угол
    } else if (event->x() >= width() - resizeMargin && event->y() <= resizeMargin) {
        setCursor(Qt::SizeFDiagCursor); // Правый верхний угол
    } else if (event->x() <= resizeMargin && event->y() >= height() - resizeMargin) {
        setCursor(Qt::SizeFDiagCursor); // Левый нижний угол
    } else if (event->x() <= resizeMargin && event->y() <= resizeMargin) {
        setCursor(Qt::SizeFDiagCursor); // Левый верхний угол
    } else if (event->x() >= width() - resizeMargin) {
        setCursor(Qt::SizeHorCursor); // Правая сторона
    } else if (event->x() <= resizeMargin) {
        setCursor(Qt::SizeHorCursor); // Левая сторона
    } else if (event->y() >= height() - resizeMargin) {
        setCursor(Qt::SizeVerCursor); // Нижняя сторона
    } else if (event->y() <= resizeMargin) {
        setCursor(Qt::SizeVerCursor); // Верхняя сторона
    } else {
        setCursor(Qt::ArrowCursor); // Стандартный указатель
    }

    if (resizing) {
        // Изменение размера окна
        int dx = event->globalX() - lastMousePosition.x();
        int dy = event->globalY() - lastMousePosition.y();
        resize(width() + dx, height() + dy);
        lastMousePosition = event->globalPosition().toPoint();
        event->accept();
    } else if (moving) {
        // Перемещение окна
        move(event->globalPosition().toPoint() - lastMousePosition);
        event->accept();
    }

    QMainWindow::mouseMoveEvent(event);
}





void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // Проверка, находится ли курсор на краю окна
        if (event->x() >= width() - resizeMargin && event->y() >= height() - resizeMargin) {
            resizing = true; // Начинаем изменение размера
        } else if (event->y() < resizeMargin) {
            moving = true; // Начинаем перемещение
        }
        lastMousePosition = event->globalPosition().toPoint();
        event->accept();
    }
}



void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    resizing = false; // Останавливаем изменение размера
    moving = false; // Останавливаем перемещение
    setCursor(Qt::ArrowCursor); // Возвращаем стандартный указатель
    event->accept();
}




