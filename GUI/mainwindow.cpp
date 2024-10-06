#include "mainwindow.h"

class SaveDialog : public QDialog {
public:
    SaveDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("Сохранение");

        setStyleSheet("background-color: gray; color: black;");

        QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel("Сохранить изменения?", this);
        layout->addWidget(label);

        QHBoxLayout *buttonLayout = new QHBoxLayout();

        QPushButton *yesButton = new QPushButton("Да", this);
        QPushButton *noButton = new QPushButton("Нет", this);
        QPushButton *cancelButton = new QPushButton("Отмена", this);

        yesButton->setStyleSheet("background-color: darkgray; color: black;");
        noButton->setStyleSheet("background-color: darkgray; color: black;");
        cancelButton->setStyleSheet("background-color: darkgray; color: black;");

        buttonLayout->addWidget(yesButton);
        buttonLayout->addWidget(noButton);
        buttonLayout->addWidget(cancelButton);

        layout->addLayout(buttonLayout);

        connect(yesButton, &QPushButton::clicked, this, [this]() {
            done(QMessageBox::Yes);
        });
        connect(noButton, &QPushButton::clicked, this, [this]() {
            done(QMessageBox::No);
        });
        connect(cancelButton, &QPushButton::clicked, this, [this]() {
            done(QMessageBox::Cancel);
        });
    }
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), Index(-1),save(true) {

    ui->setupUi(this);

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