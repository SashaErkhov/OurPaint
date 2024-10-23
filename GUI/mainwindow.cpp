#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow),
          save(true),
          moving(false),
          resizing(false),
          drawingFrame(false),
          frameOverlay(new FrameOverlay()),
          Index(0),
          helpWindow(nullptr),
          addElem(true) {

    ui->setupUi(this);
    setAllMouseTracking(this); // Отслеживание мыши
    setAttribute(Qt::WA_OpaquePaintEvent);

    // Кнопки сохранение/импорт
    connect(ui->actionSave_project_to, &QAction::triggered, this, &MainWindow::saveProjectToFile);
    connect(ui->actionImport_project, &QAction::triggered, this, &MainWindow::LoadProjectFile);

    // Кнопки сервера
    connect(ui->actionOpen_server, &QAction::triggered, this, &MainWindow::openServer);
    connect(ui->actionJoin_server, &QAction::triggered, this, &MainWindow::joinServer);
    connect(ui->actionJoin_local_server, &QAction::triggered, this, &MainWindow::joinLocalServer);
    connect(ui->actionJoin_local_server, &QAction::triggered, this, &MainWindow::exitSession);

    // Кнопка помощь
    connect(ui->helpButton, &QPushButton::clicked, this, &MainWindow::showHelp);

    // Обработка ввода в консоли
    connect(ui->console, &QLineEdit::returnPressed, this, [this]() {
        QString input = ui->console->text();
        if (!input.isEmpty()) {
            commands.push_back(input);
            emit EnterPressed(input);
            ui->console->clear();
        }
    });

    // Обработка ввода в консоль чата
    connect(ui->messageConsole, &QLineEdit::returnPressed, this, [this]() {
        QString input = ui->messageConsole->text();
        if (!input.isEmpty()) {
            emit EnterMessage(input);
            ui->messageConsole->clear();
        }
    });

    // Изменение параметров обьектов в левом меню
    connect(ui->leftMenu, &QTreeWidget::itemChanged, this, &MainWindow::LeftMenuChanged);

    connect(ui->componentGrid, &QCheckBox::toggled, [=](bool checked) {
        emit GridOn(checked);
    });


    this->setFocusPolicy(Qt::StrongFocus);

    frameOverlay->hide(); // Скрытие наложения рамки
}


// Отрисовка 60 раз в секундк
void MainWindow::resizeEvent(QResizeEvent *event) {
    emit resized();
    QMainWindow::resizeEvent(event);
}


// Кручение колёсиком или тачпадом
void MainWindow::wheelEvent(QWheelEvent *event) {
    if (ui->workWindow && ui->workWindow->underMouse()) { // В области workWindow
        if (event->angleDelta().y() > 0) {
            emit KeyPlus();
        } else {
            emit KeyMinus();
        }
        event->accept();
    } else {
        QMainWindow::wheelEvent(event);
    }
}


// Жесты в тачпаде в области workWindow
bool MainWindow::event(QEvent *event) {

    if (event->type() == QEvent::Gesture) {
        QGestureEvent *gestureEvent = static_cast<QGestureEvent *>(event);
        if (QGesture *pinch = gestureEvent->gesture(Qt::PinchGesture)) {
            QPinchGesture *pinchGesture = static_cast<QPinchGesture *>(pinch);
            if (pinchGesture->changeFlags() & QPinchGesture::ScaleFactorChanged) {
                if (pinchGesture->scaleFactor() > 1.0) {
                    emit KeyPlus();
                } else {
                    emit KeyMinus();
                }
            }
            return true;
        }
    }

    return QMainWindow::event(event);
}


// Обработка кнопки помощи
void MainWindow::showHelp() {
    if (!helpWindow) {
        helpWindow = new Help(this);
    }
    helpWindow->show();
    helpWindow->raise();
    helpWindow->activateWindow();
}


// Отслеживание мыши
void MainWindow::setAllMouseTracking(QWidget *widget) {
    widget->setMouseTracking(true);
    for (QObject *child: widget->children()) {
        if (QWidget *childWidget = qobject_cast<QWidget *>(child)) {
            setAllMouseTracking(childWidget);
        }
    }
}


// Обработка события закрытия окна
void MainWindow::closeEvent(QCloseEvent *event) {
    if (!save) {
        SaveDialog dialog(this);
        dialog.setModal(true);
        int result = dialog.exec(); // Показать окно

        if (result == QMessageBox::Yes) {
            saveProjectToFile();
            if (save) {
                event->accept();
                close();
            } else {
                event->ignore();
            }
        } else if (result == QMessageBox::No) {
            event->accept();
            close();
        } else {
            event->ignore();
        }

    } else {
        event->accept();
        close();
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
    addElem = true;

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

    itemFigure->setFlags(itemFigure->flags() | Qt::ItemIsEditable); // Для изменения имён

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

        if (paramNames[i] != "ID") {
            paramItem->setFlags(paramItem->flags() | Qt::ItemIsEditable); // Для изменения параметров
        }

        QIcon paramIcon("../Static/icons/Database.ico");
        paramItem->setIcon(0, paramIcon);
        if (paramNames[i] == "ID") {
            paramItem->setText(0, QString("%1: %2").arg(paramNames[i]).arg(id));
        } else {
            paramItem->setText(0, QString("%1: %2").arg(paramNames[i]).arg(
                    QString::number(object[i - 1], 'f', 6))); // Установка параметра
        }
        itemFigure->addChild(paramItem);
    }
    addElem = false; // Флаг для изменения обьектов
    //  itemFigure->setExpanded(true); Разворачивание
}


// Изменение обьектов левого меню
void MainWindow::LeftMenuChanged(QTreeWidgetItem *item) {
    if (!item || addElem) { return; }
    QTreeWidgetItem *Figure = item->parent();
    if (!Figure) { return; }

    std::vector<double> parameters;
    unsigned long long id = 0;

    for (int i = 0; i < Figure->childCount(); ++i) {
        QTreeWidgetItem *Parametrs = Figure->child(i);
        QString peremen = Parametrs->text(0);

        QStringList XYR = peremen.split(": ");
        if (XYR.size() != 2) continue;

        QString Name = XYR[0];
        QString value = XYR[1];

        if (Name == "ID") {
            id = value.toULongLong();
        } else {
            bool ok;
            double count = value.toDouble(&ok);
            if (ok) {
                parameters.push_back(count);
            }
        }
    }
    emit parameterChanged(id, parameters); // Сигнал
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

    // Обрадотка комбинаций ctrl + стрелочки -> перемещает окно в разные положения
    if (event->modifiers() & Qt::ControlModifier) {
        QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
        if (event->key() == Qt::Key_Left) {
            bool isRightDownHalf = this->geometry() == QRect(screenGeometry.left() + screenGeometry.width() / 2,
                                                             screenGeometry.height() / 2, screenGeometry.width() / 2,
                                                             screenGeometry.height() / 2);
            bool isRightTop = this->geometry() ==
                              QRect(screenGeometry.left() + screenGeometry.width() / 2, screenGeometry.top(),
                                    screenGeometry.width() / 2, screenGeometry.height() / 2);
            if (isRightTop) {
                this->setGeometry(screenGeometry.left(), screenGeometry.top(),
                                  screenGeometry.width() / 2, screenGeometry.height() / 2);
            } else if (isRightDownHalf) {
                this->setGeometry(screenGeometry.left(), screenGeometry.height() / 2,
                                  screenGeometry.width() / 2, screenGeometry.height() / 2);
            } else {
                this->setGeometry(screenGeometry.left(), screenGeometry.top(),
                                  screenGeometry.width() / 2, screenGeometry.height());
            }
        } else if (event->key() == Qt::Key_Right) {
            bool isLeftDownHalf = this->geometry() ==
                                  QRect(screenGeometry.left(), screenGeometry.height() / 2, screenGeometry.width() / 2,
                                        screenGeometry.height() / 2);
            bool isLeftUpHalf = this->geometry() ==
                                QRect(screenGeometry.left(), screenGeometry.height() / 2, screenGeometry.width() / 2,
                                      screenGeometry.height() / 2);
            if (isLeftUpHalf) {
                this->setGeometry(screenGeometry.left() + screenGeometry.width() / 2, screenGeometry.top(),
                                  screenGeometry.width() / 2, screenGeometry.height() / 2);
            } else if (isLeftDownHalf) {
                this->setGeometry(screenGeometry.left() + screenGeometry.width() / 2, screenGeometry.height() / 2,
                                  screenGeometry.width() / 2, screenGeometry.height() / 2);
            } else {
                this->setGeometry(screenGeometry.left() + screenGeometry.width() / 2, screenGeometry.top(),
                                  screenGeometry.width() / 2, screenGeometry.height());
            }
        } else if (event->key() == Qt::Key_Up) {
            bool isLeftDownHalf = this->geometry() ==
                                  QRect(screenGeometry.left(), screenGeometry.height() / 2, screenGeometry.width() / 2,
                                        screenGeometry.height() / 2);
            bool isRightDownHalf = this->geometry() == QRect(screenGeometry.left() + screenGeometry.width() / 2,
                                                             screenGeometry.height() / 2, screenGeometry.width() / 2,
                                                             screenGeometry.height() / 2);
            bool isLeft = this->geometry() ==
                          QRect(screenGeometry.left(), screenGeometry.top(), screenGeometry.width() / 2,
                                screenGeometry.height());
            bool isRight = this->geometry() ==
                           QRect(screenGeometry.left() + screenGeometry.width() / 2, screenGeometry.top(),
                                 screenGeometry.width() / 2, screenGeometry.height());
            if (isLeft || isLeftDownHalf) {
                this->setGeometry(screenGeometry.left(), screenGeometry.top(), screenGeometry.width() / 2,
                                  screenGeometry.height() / 2);
            } else if (isRight || isRightDownHalf) {
                this->setGeometry(screenGeometry.left() + screenGeometry.width() / 2, screenGeometry.top(),
                                  screenGeometry.width() / 2, screenGeometry.height() / 2);
            } else {
                this->showMaximized();
            }
        } else if (event->key() == Qt::Key_Down) {
            bool isOnRight =
                    this->geometry() == QRect(screenGeometry.left() + screenGeometry.width() / 2, screenGeometry.top(),
                                              screenGeometry.width() / 2, screenGeometry.height());
            bool isOnLeft = this->geometry() ==
                            QRect(screenGeometry.left(), screenGeometry.top(), screenGeometry.width() / 2,
                                  screenGeometry.height());
            bool isRightTop = this->geometry() ==
                              QRect(screenGeometry.left() + screenGeometry.width() / 2, screenGeometry.top(),
                                    screenGeometry.width() / 2, screenGeometry.height() / 2);
            bool isLeftTop = this->geometry() ==
                             QRect(screenGeometry.left(), screenGeometry.top(), screenGeometry.width() / 2,
                                   screenGeometry.height() / 2);
            if (this->isMaximized()) {
                this->showNormal();
            } else if (isOnRight || isRightTop) {
                this->setGeometry(screenGeometry.left() + screenGeometry.width() / 2, screenGeometry.height() / 2,
                                  screenGeometry.width() / 2, screenGeometry.height() / 2);
            } else if (isOnLeft || isLeftTop) {
                this->setGeometry(screenGeometry.left(), screenGeometry.height() / 2, screenGeometry.width() / 2,
                                  screenGeometry.height() / 2);
            } else {
                this->showMinimized();
            }
            // Обработка других комбинаций
        } else if (event->key() == Qt::Key_W) { // Ctrl+W
            emit REDO(); // Сигнал
        } else if (event->key() == Qt::Key_Z) { // Ctrl+Z
            emit UNDO(); // Сигнал
        } else if (event->key() == Qt::Key_Plus) {
            emit KeyPlus();
        } else if (event->key() == Qt::Key_Minus) {
            emit KeyMinus();
        } else if (event->key() == Qt::Key_0) {
            emit KeyZero();
        }
    }

    // Буфер команд
    if (ui->console->isActiveWindow()) { // Если консоль активна
        if (event->key() == Qt::Key_Up) { // Кнопка вверх
            if (Index == 0) {
                Index = static_cast<int>(commands.size()) - 1;
            } else {
                Index = (Index + 1) % commands.size();
            }
            ui->console->setText(commands[Index]);
        } else if (event->key() == Qt::Key_Down) { // Кнопка вниз
            Index = (Index - 1 + commands.size()) % commands.size();
            ui->console->setText(commands[Index]);
        }
    }

    if (event->key() == Qt::Key_F11) { // F11 - Полный экран
        if (isFullScreen()) {
            showNormal();
        } else {
            showFullScreen();
        }
        event->accept();
    }

    QWidget::keyPressEvent(event); // Вызов базового обработчика
}


// Обработка кнопки сервера
void MainWindow::openServer() {
    WindowServer *windowServer = new WindowServer;
    QObject::connect(windowServer, &WindowServer::textEnter, [this](const QString &text) {
        emit SigOpenServer(text);
    });
    windowServer->show();
}


// Обработка кнопки сервера
void MainWindow::joinServer() {
    WindowServer *windowServer = new WindowServer;
    QObject::connect(windowServer, &WindowServer::textEnter, [this](const QString &text) {
        emit SigJoinServer(text);
    });
    windowServer->show();
}


// Обработка кнопки сервера
void MainWindow::joinLocalServer() {
    WindowServer *windowServer = new WindowServer;
    QObject::connect(windowServer, &WindowServer::textEnter, [this](const QString &text) {
        emit SigJoinLocalServer(text);
    });
    windowServer->show();
}


MainWindow::~MainWindow() {
    delete frameOverlay; // Наложения рамки
    delete ui;
    delete helpWindow;
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
            drawingFrame = true; // Создаём рамку
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


// Перемщение курсора
void MainWindow::mouseMoveEvent(QMouseEvent *event) {

    // Отслеживаем на границе ли он
    if (drawingFrame) { // Создание рамки
        QPoint delta = event->globalPosition().toPoint() - lastMousePosition;

        if (resizing) {  // Создание рамки
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


// Обработка отпускания мыши
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


// Двойное нажатие мыши
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
        ui->messageConsole->setStyleSheet(QString::fromUtf8(R"(
        background: "#3e3d3d";
        color: "#D8D8F6";
        border: 1px solid black;
        border-bottom-left-radius: 10px;
        border-bottom-right-radius: 0px;
    )"));
        ui->collapsedPanel->setStyleSheet("background-color: #494850; border-bottom-left-radius: 10px;\n"
                                          "border-bottom-right-radius: 0px;");
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
        ui->messageConsole->setStyleSheet(QString::fromUtf8(R"(
        background: "#3e3d3d";
        color: "#D8D8F6";
        border: 1px solid black;
    )"));
        ui->collapsedPanel->setStyleSheet("background-color: #494850; ");
    }


    painter.setClipPath(path);
    painter.fillPath(path, QColor("#978897"));
    QPen pen(QColor("#000000"), 1);
    painter.setPen(pen);

    painter.drawPath(path);

    emit resized();
}