#ifndef OURPAINT_SAVEDIALOG_H
#define OURPAINT_SAVEDIALOG_H

/*
 *
 *  Кастомное окошко для вопроса сохранения окна
 *
 *
 * */



class SaveDialog : public QDialog {
private:
    QString file;
    bool mousePressed;
    QPoint mousePos;
public:
    //void setFile(QString File){file=File;}
    SaveDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);
        setWindowModality(Qt::ApplicationModal);  // Для модальности только в пределах родительского окна

       // setStyleSheet("background-color: gray; color: black;");

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(10, 10, 10, 10);
        QLabel *label = new QLabel("Сохранить проект?", this);
        layout->addWidget(label);

        QHBoxLayout *buttonLayout = new QHBoxLayout();

        QPushButton *yesButton = new QPushButton("Да", this);
        QPushButton *noButton = new QPushButton("Нет", this);
        QPushButton *cancelButton = new QPushButton("Отмена", this);

      //  yesButton->setStyleSheet("background-color: darkgray; color: black;");
      //  noButton->setStyleSheet("background-color: darkgray; color: black;");
       // cancelButton->setStyleSheet("background-color: darkgray; color: black;");

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
protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QColor(50, 50, 50, 240));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(rect(), 10, 10);
    }

    void mousePressEvent(QMouseEvent *event) override { // Нажатие
        if (event->button() == Qt::LeftButton) {
            setCursor(Qt::SizeAllCursor);
            mousePressed = true;
            mousePos = event->pos();
            event->accept();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override { // Перемещение
        if (mousePressed) {
            move(event->globalPos() - mousePos);
            event->accept();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override { // Отпускание
        if (event->button() == Qt::LeftButton) {
            setCursor(Qt::ArrowCursor);
            mousePressed = false;
            event->accept();
        }
    }

};


#endif //OURPAINT_SAVEDIALOG_H
