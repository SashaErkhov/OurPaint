#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          ui(new Ui::MainWindow),
          Index(-1),
          save(true),
          moving(false),
          resizing(false),
          resizeMargin(10) {

    ui->setupUi(this);
    setAllMouseTracking(this);

    commands = {"exit", "point 0 0", "circle 0 0 10", "section 0 0 10 10", "clear", "addreq "};

    connect(ui->actionSave_project_to, &QAction::triggered, this, &MainWindow::saveProjectToFile);
    connect(ui->actionImport_project, &QAction::triggered, this, &MainWindow::LoadProjectFile);

    connect(ui->console, &QLineEdit::returnPressed, this, [this]() {
        QString input = ui->console->text();
        emit EnterPressed(input);
        ui->console->clear();
    });
}




void MainWindow::setAllMouseTracking(QWidget *widget) {
    widget->setMouseTracking(true);
    for (QObject *child : widget->children()) {
        if (QWidget *childWidget = qobject_cast<QWidget *>(child)) {
            setAllMouseTracking(childWidget);
        }
    }
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
        } else if (event->key() == Qt::Key_W && event->modifiers() & Qt::ControlModifier) {
            emit REDO();
        } else if (event->key() == Qt::Key_Z && event->modifiers() & Qt::ControlModifier) {
            emit UNDO();
        }

        if (event->key() == Qt::Key_F11) {
            if (isFullScreen()) {
                showNormal();
            } else {
                showFullScreen();
            }
            event->accept();
        }
    }
    QWidget::keyPressEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*********************
 *
 *
 *        Функции перемещения
 *
 *
 *
 * ***************************************************************************************/


void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // Determine if we're on a resizing edge
        resizingEdges = Qt::Edges();

        if (event->pos().x() <= resizeMargin)
            resizingEdges |= Qt::LeftEdge;
        else if (event->pos().x() >= width() - resizeMargin)
            resizingEdges |= Qt::RightEdge;

        if (event->pos().y() <= resizeMargin)
            resizingEdges |= Qt::TopEdge;
        else if (event->pos().y() >= height() - resizeMargin)
            resizingEdges |= Qt::BottomEdge;

        if (resizingEdges != Qt::Edges()) {
            resizing = true;
            lastMousePosition = event->globalPosition().toPoint();
        } else {
            // Start moving the window
            moving = true;
            lastMousePosition = event->globalPosition().toPoint();
        }
        event->accept();
    } else {
        QMainWindow::mousePressEvent(event);
    }
}



void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (resizing) {
        QPoint delta = event->globalPosition().toPoint() - lastMousePosition;
        QRect geom = geometry();

        if (resizingEdges & Qt::LeftEdge)
            geom.setLeft(geom.left() + delta.x());
        if (resizingEdges & Qt::RightEdge)
            geom.setRight(geom.right() + delta.x());
        if (resizingEdges & Qt::TopEdge)
            geom.setTop(geom.top() + delta.y());
        if (resizingEdges & Qt::BottomEdge)
            geom.setBottom(geom.bottom() + delta.y());

        setGeometry(geom);
        lastMousePosition = event->globalPosition().toPoint();
        event->accept();
    } else if (moving) {
        QPoint delta = event->globalPosition().toPoint() - lastMousePosition;
        move(pos() + delta);
        lastMousePosition = event->globalPosition().toPoint();
        event->accept();
    } else {
        Qt::Edges edges = Qt::Edges();

        if (event->pos().x() <= resizeMargin)
            edges |= Qt::LeftEdge;
        else if (event->pos().x() >= width() - resizeMargin)
            edges |= Qt::RightEdge;

        if (event->pos().y() <= resizeMargin)
            edges |= Qt::TopEdge;
        else if (event->pos().y() >= height() - resizeMargin)
            edges |= Qt::BottomEdge;

        if (edges != Qt::Edges()) {
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
    if (event->button() == Qt::LeftButton) {
        resizing = false;
        moving = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    } else {
        QMainWindow::mouseReleaseEvent(event);
    }
}



void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        moving = true;
        setCursor(Qt::SizeAllCursor);
        lastMousePosition = event->globalPosition().toPoint();
        event->accept();
    } else {
        QMainWindow::mouseDoubleClickEvent(event);
    }
}



void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;

    if (!isMaximized()) {
        path.addRoundedRect(0, 0, width(), height(), 10, 10); // Rounded corners
    } else {
        path.addRect(0, 0, width(), height()); // Regular rectangle
    }

    painter.setClipPath(path);
    painter.fillPath(path, QColor("#978897"));
    QPen pen(QColor("#000000"), 1);
    painter.setPen(pen);

    painter.drawPath(path);
}