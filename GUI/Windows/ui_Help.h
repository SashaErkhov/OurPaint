#ifndef UI_HELP_H
#define UI_HELP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QSizePolicy>

QT_BEGIN_NAMESPACE

class Ui_Help
{
public:
    // Main widgets
    QWidget *centralwidget;
    QGridLayout *mainLayout;
    QFrame *topBar;
    QHBoxLayout *topBarLayout;
    QLabel *titleLabel;
    QPushButton *closeButton;

    // Left menu
    QWidget *leftMenu;
    QVBoxLayout *leftMenuLayout;
    QPushButton *commandsButton;
    QPushButton *requirementsButton;
    QPushButton *aboutButton;

    // Content area
    QFrame *contentFrame;
    QVBoxLayout *contentLayout;
    QLabel *helpContentLabel;

    void setupUi(QMainWindow *HelpWindow)
    {
        HelpWindow->setObjectName("HelpWindow");
        HelpWindow->resize(600, 400);
        HelpWindow->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
        HelpWindow->setAttribute(Qt::WA_TranslucentBackground);
        HelpWindow->setStyleSheet("QWidget { color: #D8D8F6; }");

        centralwidget = new QWidget(HelpWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QGridLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mainLayout->setSpacing(0);

        setupTopBar(HelpWindow);

        mainLayout->addWidget(topBar, 0, 0, 1, 2);

        setupLeftMenu();

        mainLayout->addWidget(leftMenu, 1, 0, 1, 1);

        setupContent();

        mainLayout->addWidget(contentFrame, 1, 1, 1, 1);

        // Adjust spacings to zero
        mainLayout->setHorizontalSpacing(0);
        mainLayout->setVerticalSpacing(0);

        mainLayout->setColumnStretch(0, 0);
        mainLayout->setColumnStretch(1, 1);
        mainLayout->setRowStretch(1, 1);

        HelpWindow->setCentralWidget(centralwidget);

        retranslateUi(HelpWindow);

        QMetaObject::connectSlotsByName(HelpWindow);
    }

    void setupTopBar(QMainWindow *HelpWindow)
    {
        topBar = new QFrame(centralwidget);
        topBar->setObjectName("topBar");
        topBar->setStyleSheet("QFrame#topBar { "
                              "background-color: #494850; "
                              "color: #D8D8F6; "
                              "border-top-left-radius: 10px; "
                              "border-top-right-radius: 10px; "
                              "border-bottom: 1px solid #262222; }");
        topBar->setFixedHeight(30);

        topBarLayout = new QHBoxLayout(topBar);
        topBarLayout->setContentsMargins(5, 0, 5, 0);
        topBarLayout->setSpacing(0);
        titleLabel = new QLabel("Справка", topBar);
        titleLabel->setStyleSheet("QLabel { color: #D8D8F6; font-size: 14px; }");

        setupWindowControlButtons(HelpWindow);

        topBarLayout->addWidget(titleLabel);
        topBarLayout->addStretch();
        topBarLayout->addWidget(closeButton);
    }

    void setupWindowControlButtons(QMainWindow *HelpWindow)
    {
        closeButton = new QPushButton("", topBar);
        closeButton->setFixedSize(25, 25);
        closeButton->setIcon(QIcon("../Static/icons/close1.ico"));
        closeButton->setStyleSheet("QPushButton { background: none; border: none; color: white; border-radius: 5px; }"
                                   "QPushButton:hover { background-color: rgba(255, 255, 255, 0.3); }");
        QObject::connect(closeButton, &QPushButton::clicked, HelpWindow, &QMainWindow::close);
    }

    void setupLeftMenu()
    {
        leftMenu = new QWidget(centralwidget);
        leftMenu->setObjectName("leftMenu");
        leftMenu->setStyleSheet("QWidget#leftMenu { "
                                "background-color: #494850; "
                                "color: #D8D8F6; "
                                "border: none;"
                                "border-bottom-left-radius: 10px; "
                                "border-top-left-radius: 0px; "
                                "border-top-right-radius: 0px; "
                                "border-bottom-right-radius: 0px; }");
        leftMenu->setMinimumWidth(150);

        leftMenuLayout = new QVBoxLayout(leftMenu);
        leftMenuLayout->setContentsMargins(0, 0, 0, 0);
        leftMenuLayout->setSpacing(0);

        QFont buttonFont;
        buttonFont.setPointSize(10);

        commandsButton = new QPushButton("Commands", leftMenu);
        commandsButton->setObjectName("commandsButton");
        commandsButton->setFont(buttonFont);
        commandsButton->setStyleSheet(buttonStyle());
        leftMenuLayout->addWidget(commandsButton);

        requirementsButton = new QPushButton("Requirements", leftMenu);
        requirementsButton->setObjectName("requirementsButton");
        requirementsButton->setFont(buttonFont);
        requirementsButton->setStyleSheet(buttonStyle());
        leftMenuLayout->addWidget(requirementsButton);

        aboutButton = new QPushButton("About", leftMenu);
        aboutButton->setObjectName("aboutButton");
        aboutButton->setFont(buttonFont);
        aboutButton->setStyleSheet(buttonStyle());
        leftMenuLayout->addWidget(aboutButton);

        leftMenuLayout->addStretch();
    }

    QString buttonStyle()
    {
        return QString(
                "QPushButton { "
                "background-color: #494850; "
                "color: #D8D8F6; "
                "border: none; "
                "padding: 10px; "
                "text-align: left; "
                "}"
                "QPushButton:hover { "
                "background-color: rgba(255, 255, 255, 0.1); "
                "}"
                "QPushButton:checked { "
                "background-color: rgba(255, 255, 255, 0.2); "
                "}"
        );
    }

    void setupContent()
    {
        contentFrame = new QFrame(centralwidget);
        contentFrame->setObjectName("contentFrame");
        contentFrame->setStyleSheet("QFrame#contentFrame { "
                                    "background-color: #3e3d3d; "
                                    "color: #D8D8F6; "
                                    "border-bottom-right-radius: 10px; "
                                    "border-top-right-radius: 0px; }");

        contentLayout = new QVBoxLayout(contentFrame);
        contentLayout->setContentsMargins(15, 15, 15, 15);
        contentLayout->setSpacing(0);

        helpContentLabel = new QLabel(contentFrame);
        helpContentLabel->setObjectName("helpContentLabel");
        helpContentLabel->setWordWrap(true);
        helpContentLabel->setStyleSheet("QLabel { color: #D8D8F6; font-size: 12px; }");

        contentLayout->addWidget(helpContentLabel);
    }

    void retranslateUi(QMainWindow *HelpWindow)
    {
        HelpWindow->setWindowTitle(QCoreApplication::translate("HelpWindow", "Help", nullptr));
        titleLabel->setText(QCoreApplication::translate("HelpWindow", "Help", nullptr));

        commandsButton->setText(QCoreApplication::translate("HelpWindow", "Commands", nullptr));
        requirementsButton->setText(QCoreApplication::translate("HelpWindow", "Requirements", nullptr));
        aboutButton->setText(QCoreApplication::translate("HelpWindow", "About", nullptr));
    }
};

namespace Ui {
    class Help: public Ui_Help {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELP_H