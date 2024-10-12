#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QPushButton>
#include <QTextBrowser>
#include <QRegion>
#include <QPainterPath>
#include <QBitmap>
#include <QPainter>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    // Действия (Actions)
    QAction *actionSave_project_to;
    QAction *actionImport_project;
    QAction *actionExport_bmp;
    QAction *actionOpen_bmp;
    QAction *actionOpen_server;
    QAction *actionJoin_server;
    QAction *actionJoin_local_server;
    QAction *actionExit_from_session;
    QAction *action_help;

    // Основные виджеты
    QWidget *centralwindow;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTreeWidget *leftMenu;
    QLineEdit *console;
    QFrame *workWindow;
    QWidget *topBar;
    QHBoxLayout *topBarLayout;

    // Меню
    QMenu *menuProject;
    QMenu *menuCollaboration;
    QMenu *menuHelp;

    // Кнопки меню
    QPushButton *projectButton;
    QPushButton *collaborationButton;
    QPushButton *helpButton;

    // Кнопки управления окном
    QPushButton *closeButton;
    QPushButton *minimizeButton;
    QPushButton *maximizeButton;

    void setupUi(QMainWindow *MainWindow)
    {
        // Установка флагов окна
        MainWindow->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
        MainWindow->setAttribute(Qt::WA_TranslucentBackground);

        // Установка объекта окна
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");

        // Размеры окна
        int width = 1280;
        int height = 720;
        MainWindow->resize(width, height);
        MainWindow->setMinimumSize(QSize(672, 378));

        // Настройка действий
        setupActions(MainWindow);

        // Создание центрального виджета и основного макета
        centralwindow = new QWidget(MainWindow);
        centralwindow->setObjectName("centralwindow");

        gridLayout_2 = new QGridLayout(centralwindow);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setSpacing(0);

        // Создание верхней панели с меню и кнопками управления окном
        setupTopBar(MainWindow);

        // Добавление верхней панели в основной макет
        gridLayout_2->addWidget(topBar, 0, 0, 1, 1, Qt::AlignTop);

        // Создание основного макета приложения
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setVerticalSpacing(0);

        // Настройка левого меню
        setupLeftMenu();

        // Настройка рабочего окна
        setupWorkWindow();

        // Настройка консоли
        setupConsole();

        // Создание правого вертикального макета
        QVBoxLayout *rightLayout = new QVBoxLayout();
        rightLayout->setContentsMargins(0, 10, 10, 0);
        rightLayout->setSpacing(10); // Добавляем отступ между console и workWindow

        // Добавляем workWindow в rightLayout
        rightLayout->addWidget(workWindow);

        // Добавляем console в rightLayout под workWindow
        rightLayout->addWidget(console);

        // Устанавливаем коэффициенты растяжения для rightLayout
        rightLayout->setStretch(0, 1); // workWindow растягивается по высоте

        // Создание горизонтального spacer между leftMenu и workWindow
        QSpacerItem *horizontalSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

        // Добавление виджетов и spacer в основной макет
        gridLayout->addWidget(leftMenu, 0, 0, 1, 1); // Левое меню в первой колонке
        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1); // Spacer во второй колонке
        gridLayout->addLayout(rightLayout, 0, 2, 1, 1); // Правый макет с рабочим окном и консолью в третьей колонке

        // Установка коэффициентов растяжения
        gridLayout->setColumnStretch(0, 0); // leftMenu не растягивается по ширине
        gridLayout->setColumnStretch(1, 0); // spacer фиксированного размера
        gridLayout->setColumnStretch(2, 1); // rightLayout растягивается по ширине
        gridLayout->setRowStretch(0, 1);    // Строка с leftMenu и workWindow растягивается по высоте

        // Добавление основного макета в центральный виджет
        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);

        // Установка центрального виджета
        MainWindow->setCentralWidget(centralwindow);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    }

    void setupActions(QMainWindow *MainWindow)
    {
        // Инициализация действий меню
        QFont font;
        font.setPointSize(9);

        actionSave_project_to = new QAction(MainWindow);
        actionSave_project_to->setObjectName("actionSave_project_to");
        actionSave_project_to->setFont(font);

        actionImport_project = new QAction(MainWindow);
        actionImport_project->setObjectName("actionImport_project");
        actionExport_bmp = new QAction(MainWindow);
        actionExport_bmp->setObjectName("actionExport_bmp");
        actionOpen_bmp = new QAction(MainWindow);
        actionOpen_bmp->setObjectName("actionOpen_bmp");
        actionOpen_server = new QAction(MainWindow);
        actionOpen_server->setObjectName("actionOpen_server");
        actionJoin_server = new QAction(MainWindow);
        actionJoin_server->setObjectName("actionJoin_server");
        actionJoin_local_server = new QAction(MainWindow);
        actionJoin_local_server->setObjectName("actionJoin_local_server");
        actionExit_from_session = new QAction(MainWindow);
        actionExit_from_session->setObjectName("actionExit_from_session");
        action_help = new QAction(MainWindow);
        action_help->setObjectName("action_help");
    }

    void setupTopBar(QMainWindow *MainWindow)
    {
        // Создание верхней панели
        topBar = new QWidget(MainWindow);
        topBar->setObjectName("topBar");
        topBar->setStyleSheet("QWidget#topBar { background-color: #494850; color: #D8D8F6; border: 0px solid #000000; border-radius: 3px; }");
        topBar->setFixedHeight(30);

        // Макет для верхней панели
        topBarLayout = new QHBoxLayout(topBar);
        topBarLayout->setContentsMargins(5, 0, 5, 0);
        topBarLayout->setSpacing(5);

        // Создание меню
        setupMenus(MainWindow);

        // Создание кнопок меню
        setupMenuButtons();

        // Добавление кнопок меню в верхнюю панель
        topBarLayout->addWidget(projectButton);
        topBarLayout->addWidget(collaborationButton);
        topBarLayout->addWidget(helpButton);

        // Кнопки управления окном
        setupWindowControlButtons(MainWindow);

        // Добавление кнопок управления окном в верхнюю панель
        topBarLayout->addStretch();
        topBarLayout->addWidget(minimizeButton);
        topBarLayout->addWidget(maximizeButton);
        topBarLayout->addWidget(closeButton);
    }

    void setupMenus(QMainWindow *MainWindow)
    {
        QFont font;
        font.setPointSize(9);

        // Меню "Project"
        menuProject = new QMenu(MainWindow);
        menuProject->setObjectName("menuProject");
        menuProject->setStyleSheet("QMenu#menuProject { background-color: #494850; color: #D8D8F6; border: 1px solid #443d3c; border-radius: 5px; }");
        menuProject->setFont(font);
        menuProject->addAction(actionSave_project_to);
        menuProject->addAction(actionImport_project);
        menuProject->addAction(actionExport_bmp);
        menuProject->addAction(actionOpen_bmp);

        // Меню "Collaboration"
        menuCollaboration = new QMenu(MainWindow);
        menuCollaboration->setObjectName("menuCollaboration");
        menuCollaboration->setStyleSheet("QMenu#menuCollaboration { background-color: #494850; color: #D8D8F6; border: 1px solid #443d3c; border-radius: 5px; }");
        menuCollaboration->setFont(font);
        menuCollaboration->addAction(actionOpen_server);
        menuCollaboration->addAction(actionJoin_server);
        menuCollaboration->addAction(actionJoin_local_server);
        menuCollaboration->addSeparator();
        menuCollaboration->addAction(actionExit_from_session);

        // Меню "Help"
        menuHelp = new QMenu(MainWindow);
        menuHelp->setObjectName("menuHelp");
        menuHelp->setStyleSheet("QMenu#menuHelp { background-color: #494850; color: #D8D8F6; border: 1px solid #443d3c; border-radius: 5px; }");
        menuHelp->setFont(font);
        menuHelp->addAction(action_help);
    }

    void setupMenuButtons()
    {
        // Создание кнопки "Project"
        projectButton = new QPushButton("Project", topBar);
        projectButton->setObjectName("projectButton");
        projectButton->setStyleSheet(
                "QPushButton#projectButton { "
                "background-color: #494850; "
                "color: #D8D8F6; "
                "}"
                "QPushButton#projectButton:hover {background-color: rgba(255, 255, 255, 0.3); }"
        );
        projectButton->setMenu(menuProject);

        // Создание кнопки "Collaboration"
        collaborationButton = new QPushButton("Collaboration", topBar);
        collaborationButton->setObjectName("collaborationButton");
        collaborationButton->setStyleSheet(
                "QPushButton#collaborationButton { "
                "background-color: #494850; "
                "color: #D8D8F6; "
                "}"
                "QPushButton#collaborationButton:hover { background-color: rgba(255, 255, 255, 0.3); }"
        );
        collaborationButton->setMenu(menuCollaboration);

        // Создание кнопки "Help"
        helpButton = new QPushButton("Help", topBar);
        helpButton->setObjectName("helpButton");
        helpButton->setStyleSheet(
                "QPushButton#helpButton { "
                "background-color: #494850; "
                "color: #D8D8F6; "
                "}"
                "QPushButton#helpButton:hover { background-color: rgba(255, 255, 255, 0.3); }"
        );
        helpButton->setMenu(menuHelp);
    }

    void setupWindowControlButtons(QMainWindow *MainWindow)
    {
        // Кнопка "Закрыть"
        closeButton = new QPushButton("X", topBar);
        closeButton->setFixedSize(30, 30);
        closeButton->setStyleSheet("QPushButton { background-color: transparent; color: white; }"
                                   "QPushButton:hover { background-color: rgba(255, 255, 255, 0.3); }"); // Подсветка при наведении
        QObject::connect(closeButton, &QPushButton::clicked, MainWindow, &QMainWindow::close);

        // Кнопка "Свернуть"
        minimizeButton = new QPushButton("-", topBar);
        minimizeButton->setFixedSize(30, 30);
        minimizeButton->setStyleSheet("QPushButton { background-color: transparent; color: white; }"
                                      "QPushButton:hover { background-color: rgba(255, 255, 255, 0.3); }"); // Подсветка при наведении
        QObject::connect(minimizeButton, &QPushButton::clicked, MainWindow, &QMainWindow::showMinimized);

        // Кнопка "Развернуть"
        maximizeButton = new QPushButton("[]", topBar);
        maximizeButton->setFixedSize(30, 30);
        maximizeButton->setStyleSheet("QPushButton { background-color: transparent; color: white; }"
                                      "QPushButton:hover { background-color: rgba(255, 255, 255, 0.3); }"); // Подсветка при наведении
        QObject::connect(maximizeButton, &QPushButton::clicked, [=]() {
            if (MainWindow->isMaximized())
                MainWindow->showNormal();
            else
                MainWindow->showMaximized();
        });
    }

    void setupLeftMenu()
    {
        // Создание и настройка левого меню
        leftMenu = new QTreeWidget(centralwindow);
        leftMenu->setObjectName("leftMenu");
        leftMenu->headerItem()->setText(0, QString());

        QFont font1;
        QFont font2;
        font1.setKerning(true);
        font2.setPointSize(12);

        font1.setStyleStrategy(QFont::PreferDefault);

        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        QTreeWidgetItem *itemFigures = new QTreeWidgetItem(leftMenu);
        QTreeWidgetItem *itemRequirements = new QTreeWidgetItem(leftMenu);

        __qtreewidgetitem->setFont(0, font1);
        leftMenu->setHeaderItem(__qtreewidgetitem);

        itemFigures->setFont(0, font2);
        itemRequirements->setFont(0, font2);

        leftMenu->setStyleSheet(QString::fromUtf8(R"(background: "#494850"; color: "#D8D8F6"; QHeaderView::section { background: "#494850"; color: "#D8D8F6"; } )"));
        leftMenu->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    }

    void setupConsole()
    {
        // Создание и настройка консоли
        console = new QLineEdit(centralwindow);
        console->setObjectName("console");
        QFont font2;
        font2.setPointSize(12);
        console->setFont(font2);
        console->setCursor(QCursor(Qt::CursorShape::IBeamCursor));
        console->setAutoFillBackground(false);
        console->setMinimumHeight(30);
        console->setStyleSheet(QString::fromUtf8(R"(background: "#3e3d3d"; color: "#D8D8F6"; border: 1px solid black; border-radius: 3px;)"));
    }

    void setupWorkWindow()
    {
        // Создание и настройка рабочего окна
        workWindow = new QFrame(centralwindow);
        workWindow->setObjectName("workWindow");
        workWindow->setFrameShape(QFrame::Shape::NoFrame);
        workWindow->setFrameShadow(QFrame::Shadow::Plain);
        workWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void retranslateUi(QMainWindow *MainWindow)
    {
        // Установка текста элементов интерфейса
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionSave_project_to->setText(QCoreApplication::translate("MainWindow", "Save project to...", nullptr));
        actionImport_project->setText(QCoreApplication::translate("MainWindow", "Import project", nullptr));
        actionExport_bmp->setText(QCoreApplication::translate("MainWindow", "Export bmp", nullptr));
        actionOpen_bmp->setText(QCoreApplication::translate("MainWindow", "Open bmp", nullptr));
        actionOpen_server->setText(QCoreApplication::translate("MainWindow", "Open server", nullptr));
        actionJoin_server->setText(QCoreApplication::translate("MainWindow", "Join server", nullptr));
        actionJoin_local_server->setText(QCoreApplication::translate("MainWindow", "Join local server", nullptr));
        actionExit_from_session->setText(QCoreApplication::translate("MainWindow", "Exit from session", nullptr));
        action_help->setText(QCoreApplication::translate("MainWindow",
                                                         "Консольные команды:\n"
                                                         "point X Y\n"
                                                         "circle X Y R\n"
                                                         "section X1 Y1 X2 Y2",
                                                         nullptr));

        // Установка текста элементов левого меню
        const bool __sortingEnabled = leftMenu->isSortingEnabled();
        leftMenu->setSortingEnabled(false);
        QTreeWidgetItem *itemFigures = leftMenu->topLevelItem(0);
        itemFigures->setText(0, QCoreApplication::translate("MainWindow", "Figures", nullptr));
        QTreeWidgetItem *itemRequirements = leftMenu->topLevelItem(1);
        itemRequirements->setText(0, QCoreApplication::translate("MainWindow", "Requirements", nullptr));
        leftMenu->setSortingEnabled(__sortingEnabled);

        // Очистка текста консоли
        console->setText(QCoreApplication::translate("MainWindow", "", nullptr));

        // Установка текста кнопок меню
        projectButton->setText(QCoreApplication::translate("MainWindow", "Project", nullptr));
        collaborationButton->setText(QCoreApplication::translate("MainWindow", "Collaboration", nullptr));
        helpButton->setText(QCoreApplication::translate("MainWindow", "Help", nullptr));
    }

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H