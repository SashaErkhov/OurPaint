#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow),Index(-1) {

    ui->setupUi(this);

    commands = {"exit", "point 0 0", "circle 0 0 10", "section 0 0 10 10", "clear"};


    connect(ui->actionSave_project_to, &QAction::triggered, this, &MainWindow::saveProjectToFile);

    connect(ui->console, &QLineEdit::returnPressed, this, [this]() {
        QString input = ui->console->text();
        emit EnterPressed(input);
        ui->console->clear();
    });
}

QString MainWindow::saveProjectToFile() {
    QString baseName = "project";
    QString extension = ".ourp";
    QString fileName;
    int index = 1;

    // Set the default directory
    QString defaultDir = QDir::homePath() + "/OurPaint/project";

    // Create the directory if it doesn't exist
    QDir dir(defaultDir);
    if (!dir.exists()) {
        dir.mkpath("."); // Create the directory structure
    }

    // Generate the default file name
    fileName = QString("%1/%2%3").arg(defaultDir, baseName, extension);

    // Check if the file already exists and adjust the name if necessary
    while (QFile::exists(fileName)) {
        fileName = QString("%1/%2_%3%4").arg(defaultDir).arg(baseName).arg(index).arg(extension);
        index++;
    }

    // Open the file dialog
    QString selectedFileName = QFileDialog::getSaveFileName(this, tr("Save Project"), fileName, tr("Project Files (*.ourp);;All Files (*)"));

    if (!selectedFileName.isEmpty()) {
        emit projectSaved(selectedFileName); // Emit the signal with the filename
        return selectedFileName;
    }

    return QString(); // Return an empty string if no file was selected
}

void MainWindow::Print_LeftMenu(const std::string &text, const std::vector<int> &object) {
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
        paramNames = {"X", "Y"};
    } else if (text == "Circle" && object.size() == 3) {
        paramNames = {"X", "Y", "R"};
    } else if (text == "Section" && object.size() == 4) {
        paramNames = {"X1", "Y1", "X2", "Y2"};
    }

    for (size_t i = 0; i < paramNames.size() && i < object.size(); ++i) {
        QTreeWidgetItem *paramItem = new QTreeWidgetItem(itemFigure);
        paramItem->setText(0, QString("%1: %2").arg(paramNames[i]).arg(object[i]));
        itemFigure->addChild(paramItem);
    }

    itemFigure->setExpanded(true);
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


MainWindow::~MainWindow() {
    delete ui;
}