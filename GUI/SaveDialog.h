#ifndef OURPAINT_SAVEDIALOG_H
#define OURPAINT_SAVEDIALOG_H

/*
 *
 *  Кастомное окошко для вопроса сохранения окна
 *
 *
 * */



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


#endif //OURPAINT_SAVEDIALOG_H
