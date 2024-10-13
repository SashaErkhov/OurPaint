#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow),
          Index(-1),
          save(true),
          moving(false),
          resizing(false),
          drawingFrame(false),
          frameOverlay(new FrameOverlay()) {

    ui->setupUi(this);
    setAllMouseTracking(this); // Отслеживания мыши

    // Инициализация команд по умолчанию
    commands = {"exit", "point 0 0", "circle 0 0 10", "section 0 0 10 10", "clear", "addreq "};

    connect(ui->actionSave_project_to, &QAction::triggered, this, &MainWindow::saveProjectToFile);
    connect(ui->actionImport_project, &QAction::triggered, this, &MainWindow::LoadProjectFile);

    // Обработка ввода в консоли
    connect(ui->console, &QLineEdit::returnPressed, this, [this]() {
        QString input = ui->console->text();
        emit EnterPressed(input);
        ui->console->clear();
    });

    frameOverlay->hide(); // Скрытие наложения рамки
}




// Отслеживание мыши
void MainWindow::setAllMouseTracking(QWidget *widget) {
    widget->setMouseTracking(true);
    for (QObject *child : widget->children()) {
        if (QWidget *childWidget = qobject_cast<QWidget *>(child)) {
            setAllMouseTracking(childWidget);
        }
    }
}




// Обработка события закрытия окна
void MainWindow::closeEvent(QCloseEvent *event) {
    if (!save) {
        SaveDialog dialog(this);
        int result = dialog.exec(); // Показать окно

        if (result == QMessageBox::Yes) {
            saveProjectToFile();
            if (save) {
                event->accept();
            } else {
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




// Загрузка файла проекта
void MainWindow::LoadProjectFile() {
    if (!save) {
        SaveDialog dialog(this);
        int result = dialog.exec(); // Показать диалог

        if (result == QMessageBox::Yes) {
            saveProjectToFile();
        } else if (result == QMessageBox::Cancel) {
            return;
        }
    }

    // Открытие диалога выбора файла проекта
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"),
                                                    QDir::homePath() + "/OurPaint/project",
                                                    tr("Project Files (*.ourp);;All Files (*)"));

    if (!fileName.isEmpty()) {
        emit LoadFile(fileName); // Сигнал
    }
}




// Сохранение текущего проекта в файл
void MainWindow::saveProjectToFile() {
    QString baseName = "project";
    QString extension = ".ourp";
    QString fileName;
    int index = 1; // Уникальности имени

    // Директория по умолчанию
    QString defaultDir = QDir::homePath() + "/OurPaint/project";

    QDir dir(defaultDir);
    if (!dir.exists()) {
        dir.mkpath("."); // Создание директории, если она не существует
    }


    fileName = QString("%1/%2%3").arg(defaultDir, baseName, extension); // Формирование полного имени файла


    while (QFile::exists(fileName)) { // Проверка на существование файла
        fileName = QString("%1/%2_%3%4").arg(defaultDir).arg(baseName).arg(index).arg(extension);
        index++;
    }


    // Открытие диалога для сохранения файла
    QString selectedFileName = QFileDialog::getSaveFileName(this, tr("Save Project"), fileName,
                                                            tr("Project Files (*.ourp);;All Files (*)"));


    if (!selectedFileName.isEmpty()) {
        save = true;
        emit projectSaved(selectedFileName); //Сигнал
    } else {
        save = false;
    }
}




// Добавление элементов в левое меню
void MainWindow::Print_LeftMenu(unsigned long long id, const std::string &text, const std::vector<double> &object) {
    QTreeWidgetItem *itemFigures = ui->leftMenu->topLevelItem(1);

    if (text == "Clear") {
        itemFigures->takeChildren();
        return;
    }

    QString figureName = QString::fromStdString(text); // Преобразование имени фигуры
    int count = 1;

    while (true) { // Проверка уникальности имени фигуры
        bool exists = false;
        for (int i = 0; i < itemFigures->childCount(); ++i) {
            if (itemFigures->child(i)->text(0) == figureName) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            break;
        }

        figureName = QString("%1%2").arg(QString::fromStdString(text)).arg(count); // Изменение имени
        count++;
    }


    QTreeWidgetItem *itemFigure = new QTreeWidgetItem(itemFigures);
    itemFigure->setText(0, figureName);

    QIcon figureIcon("../Static/icons/Icon.ico");
    itemFigure->setIcon(0, figureIcon);

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
        QIcon paramIcon("../Static/icons/Database.ico");
        paramItem->setIcon(0, paramIcon);
        if (paramNames[i] == "ID") {
            paramItem->setText(0, QString("%1: %2").arg(paramNames[i]).arg(id));
        } else {
            paramItem->setText(0, QString("%1: %2").arg(paramNames[i]).arg(QString::number(object[i - 1], 'f', 6))); // Установка параметра
        }
        itemFigure->addChild(paramItem);
    }

  //  itemFigure->setExpanded(true); Разворачивание
}




// Добавление требований в левое меню
void MainWindow::Requar_LeftMenu(unsigned long long id, const std::string &text) {
    QTreeWidgetItem *itemReq = ui->leftMenu->topLevelItem(2);

    if (text == "Clear") {
        itemReq->takeChildren();
        return;
    }

    QString itemType = QString::fromStdString(std::to_string(id) + ": " + text);
    QTreeWidgetItem *newItem = new QTreeWidgetItem(itemReq);
    newItem->setText(0, itemType);

    itemReq->addChild(newItem);
   // ui->leftMenu->expandAll(); // Разворачивание
}



// Обработка нажатий клавиш
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (ui->console->isActiveWindow()) { // Если консоль активна
        if (event->key() == Qt::Key_Up) { // Кнопка вверх
            if (Index < static_cast<int>(commands.size()) - 1) {
                ++Index;
                ui->console->setText(commands[commands.size() - 1 - Index]);
            }
        } else if (event->key() == Qt::Key_Down) { // Кнопка вниз
            if (Index > 0) {
                --Index;
                ui->console->setText(commands[commands.size() - 1 - Index]);
            } else {
                Index = -1;
                ui->console->clear();
            }
        } else if (event->key() == Qt::Key_W && event->modifiers() & Qt::ControlModifier) { // Ctrl+W
            emit REDO(); // Сигнал
        } else if (event->key() == Qt::Key_Z && event->modifiers() & Qt::ControlModifier) { // Ctrl+Z
            emit UNDO(); // Сигнал
        }

        if (event->key() == Qt::Key_F11) { // F11 - Полный экран
            if (isFullScreen()) {
                showNormal();
            } else {
                showFullScreen();
            }
            event->accept();
        }
    }

    QWidget::keyPressEvent(event); // Вызов базового обработчика
}

MainWindow::~MainWindow() {
    delete frameOverlay; // Наложения рамки
    delete ui;
}

/****************************************************************************************
 *
 *  Функции перемещения и масштабирования
 *
 ***************************************************************************************/

// Обработка нажатий мыши
void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) { // Если нажата левая кнопка мыши
        resizingEdges = Qt::Edges(); // Сброс границ изменения размера

        // Определение, на каких гранях происходит изменение размера
        if (event->pos().x() <= resizeMargin)
            resizingEdges |= Qt::LeftEdge; //Левая
        if (event->pos().x() >= width() - resizeMargin)
            resizingEdges |= Qt::RightEdge; //Правая

        if (event->pos().y() <= resizeMargin)
            resizingEdges |= Qt::TopEdge; //Верхняя
        if (event->pos().y() >= height() - resizeMargin)
            resizingEdges |= Qt::BottomEdge; //Нижняя

        if (resizingEdges != Qt::Edges()) { // Если определены границы изменения размера
            resizing = true;
            drawingFrame = true;
            lastMousePosition = event->globalPosition().toPoint(); // Сохранение позиции мыши
            frameRect = geometry(); // Сохранение геометрии окна

            // Показ рамки
            QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
            frameOverlay->setGeometry(screenGeometry); // Установка геометрии
            frameOverlay->setFrameRect(frameRect); // Установка рамки
            frameOverlay->show();
            frameOverlay->raise(); // Над другими виджетами
        } else {
            moving = true;
            drawingFrame = true;
            lastMousePosition = event->globalPosition().toPoint(); // Сохранение позиции мыши
            frameRect = geometry(); // Сохранение геометрии окна

            // Показ рамки
            QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
            frameOverlay->setGeometry(screenGeometry);
            frameOverlay->setFrameRect(frameRect);
            frameOverlay->show();
            frameOverlay->raise();

            setCursor(Qt::SizeAllCursor); // Смена курсора на курсор перемещения
        }
        event->accept(); // Принятие события
    } else {
        QMainWindow::mousePressEvent(event); // Вызов базового обработчика
    }
}



void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (drawingFrame) {
        QPoint delta = event->globalPosition().toPoint() - lastMousePosition;

        if (resizing) {
            QRect newFrameRect = frameRect;

            // Корректируем размеры рамки в зависимости от того, какие края изменяются
            if (resizingEdges & Qt::LeftEdge) {
                newFrameRect.setLeft(newFrameRect.left() + delta.x());
            }
            if (resizingEdges & Qt::RightEdge) {
                newFrameRect.setRight(newFrameRect.right() + delta.x());
            }
            if (resizingEdges & Qt::TopEdge) {
                newFrameRect.setTop(newFrameRect.top() + delta.y());
            }
            if (resizingEdges & Qt::BottomEdge) {
                newFrameRect.setBottom(newFrameRect.bottom() + delta.y());
            }

            frameRect = newFrameRect;
            lastMousePosition = event->globalPosition().toPoint();
            frameOverlay->setFrameRect(frameRect);
            event->accept();

        } else if (moving) {
            // Перемещаем рамку
            frameRect.moveTopLeft(frameRect.topLeft() + delta);
            lastMousePosition = event->globalPosition().toPoint();
            setCursor(Qt::SizeAllCursor); // Проверка, что курсор остается для перемещения
            frameOverlay->setFrameRect(frameRect);
            event->accept();
        }
    } else {
        // Обновляем курсор в зависимости от положения мыши относительно краев окна
        Qt::Edges edges = Qt::Edges();

        if (event->pos().x() <= resizeMargin)
            edges |= Qt::LeftEdge;
        if (event->pos().x() >= width() - resizeMargin)
            edges |= Qt::RightEdge;

        if (event->pos().y() <= resizeMargin)
            edges |= Qt::TopEdge;
        if (event->pos().y() >= height() - resizeMargin)
            edges |= Qt::BottomEdge;

        if (edges != Qt::Edges()) {
            // Устанавливаем соответствующий курсор для изменения размера
            if ((edges & (Qt::LeftEdge | Qt::RightEdge)) && (edges & (Qt::TopEdge | Qt::BottomEdge))) {
                if ((edges & Qt::LeftEdge) && (edges & Qt::TopEdge))
                    setCursor(Qt::SizeFDiagCursor);
                else if ((edges & Qt::RightEdge) && (edges & Qt::BottomEdge))
                    setCursor(Qt::SizeFDiagCursor);
                else
                    setCursor(Qt::SizeBDiagCursor);
            } else if (edges & (Qt::LeftEdge | Qt::RightEdge)) {
                setCursor(Qt::SizeHorCursor);
            } else if (edges & (Qt::TopEdge | Qt::BottomEdge)) {
                setCursor(Qt::SizeVerCursor);
            }
        } else {
            setCursor(Qt::ArrowCursor);
        }
        QMainWindow::mouseMoveEvent(event);
    }
}




void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && drawingFrame) {
        resizing = false;
        moving = false;
        drawingFrame = false;
        setCursor(Qt::ArrowCursor);

        setGeometry(frameRect);
        frameOverlay->hide();

        event->accept();
    } else {
        QMainWindow::mouseReleaseEvent(event);
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
}

void MainWindow::paintEvent(QPaintEvent *event) {
    // Отрисовка окна с закругленными углами
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;

    if (!isMaximized() && !isFullScreen()) {
        path.addRoundedRect(0, 0, width(), height(), 10, 10);

        ui->topBar->setStyleSheet("QWidget#topBar { "
                                  "background-color: #494850; "
                                  "color: #D8D8F6; "
                                  "border-top-left-radius: 10px; "
                                  "border-top-right-radius: 10px; "
                                  "border-bottom-left-radius: 0px; "
                                  "border-bottom-right-radius: 0px; "
                                  "border: none; "
                                  "border-bottom: 1px solid #262222; }");

        ui->leftMenu->setStyleSheet(QString::fromUtf8(R"(
        background: #494850;
        color: #D8D8F6;
        border: none; /* Убираем все границы */
        border-bottom-left-radius: 10px;   /* Закругление нижнего левого угла */
        border-bottom-right-radius: 0px;
    )"));
    } else {
        path.addRect(0, 0, width(), height());
        ui->topBar->setStyleSheet("QWidget#topBar { "
                                  "background-color: #494850; "
                                  "color: #D8D8F6; "
                                  "border-radius: 0px; "
                                  "border: none; "
                                  "border-bottom: 1px solid #262222; }");

        ui->leftMenu->setStyleSheet(QString::fromUtf8(R"(
        background: #494850;
        color: #D8D8F6;
        QHeaderView::section {
            background: #494850;
            color: #D8D8F6;
        }
    )"));
    }


    painter.setClipPath(path);
    painter.fillPath(path, QColor("#978897"));
    QPen pen(QColor("#000000"), 1);
    painter.setPen(pen);

    painter.drawPath(path);
}