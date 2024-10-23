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
#include <QtWidgets/QVBoxLayout>
#include <QTextBrowser>
#include <QRegion>
#include <QPainterPath>
#include <QBitmap>
#include <QPainter>
#include <QIcon>
#include <QCheckBox>

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

    // Кнопки меню
    QPushButton *projectButton;
    QPushButton *collaborationButton;
    QPushButton *helpButton;
    QPushButton *settings;

    // Кнопки управления окном
    QPushButton *closeButton;
    QPushButton *minimizeButton;
    QPushButton *maximizeButton;

    // Левое меню
    QPushButton *collapseButton;
    QWidget *leftMenuContainer;
    QGridLayout *leftMenuLayout;
    QWidget *collapsedPanel;
    QVBoxLayout *collapsedPanelLayout;
    QPushButton *leftMenuElements;
    QTreeWidgetItem *itemFigures;
    QTreeWidgetItem *itemRequirements;
    QPushButton *leftMenuMessage;

    // Чат
    QFrame *message;
    QLineEdit *messageConsole;
    QPushButton *messageCollapseButton;
    QGridLayout *messageLayout;
    QWidget *messageContainer;
    QGridLayout *messageContainerLayout;

    // Панель настроек
    QWidget *settingsPanel;
    QVBoxLayout *settingsLayout;
    QCheckBox *componentGrid;

    void setupUi(QMainWindow *MainWindow)
    {
        // Установка флагов окна
        MainWindow->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
        MainWindow->setAttribute(Qt::WA_TranslucentBackground); // Прозрачность

        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");

        MainWindow->setStyleSheet(
                "QWidget { "
                "color: #D8D8F6; "
                "}"
        );

        MainWindow->resize(960, 540);
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

        // Создание основного макета
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setVerticalSpacing(0);

        // Настройка левого меню
        setupLeftMenu();

        // Настройка чата
        setupMessage();

        // Настройка свернутой панели
        setupCollapsedPanel();

        // Настройка рабочего окна
        setupWorkWindow();

        // Настройка консоли
        setupConsole();

        // Настройка панели настроек
        setupSettingsPanel();

        // Настройка соединений
        setupConnections();

        // Создание правого вертикального макета
        QVBoxLayout *rightLayout = new QVBoxLayout();
        rightLayout->setContentsMargins(0, 15, 15, 0);
        rightLayout->setSpacing(5); // Добавляем отступ между console и workWindow

        // Добавляем workWindow в rightLayout
        rightLayout->addWidget(workWindow);

        // Добавляем console в rightLayout под workWindow
        rightLayout->addWidget(console);

        // Устанавливаем коэффициенты растяжения для rightLayout
        rightLayout->setStretch(0, 1); // workWindow растягивается по высоте

        // Создание горизонтального spacer между leftMenu и workWindow
        QSpacerItem *horizontalSpacer = new QSpacerItem(15, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

        // Добавление виджетов и spacer в основной макет
        gridLayout->addWidget(leftMenuContainer, 0, 0, 1, 1);
        gridLayout->addWidget(messageContainer, 0, 0, 1, 1);
        gridLayout->addWidget(collapsedPanel, 0, 0, 1, 1); // Они находятся в одной позиции, но одна из них скрыта
        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1); // Spacer во второй колонке
        gridLayout->addLayout(rightLayout, 0, 2, 1, 1);    // Правый макет с рабочим окном и консолью в третьей колонке

        // Установка коэффициентов растяжения
        gridLayout->setColumnStretch(0, 0); // leftMenu не растягивается по ширине
        gridLayout->setColumnStretch(1, 0);
        gridLayout->setColumnStretch(2, 1); // rightLayout растягивается по ширине
        gridLayout->setRowStretch(0, 1);

        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwindow);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);

    }

    void setupActions(QMainWindow *MainWindow)
    {
        // Инициализация действий меню
        QFont font;
        font.setPointSize(9);

        QIcon fileIn("../Static/icons/filein.ico");
        QIcon fileOn("../Static/icons/fileon.ico");

        actionSave_project_to = new QAction(MainWindow);
        actionSave_project_to->setObjectName("actionSave_project_to");
        actionSave_project_to->setIcon(fileIn);
        actionSave_project_to->setFont(font);

        actionImport_project = new QAction(MainWindow);
        actionImport_project->setObjectName("actionImport_project");
        actionImport_project->setIcon(fileOn);

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
        topBar->setStyleSheet("QWidget#topBar { "
                              "background-color: #494850; "
                              "color: #D8D8F6; "
                              "border-top-left-radius: 10px; "
                              "border-top-right-radius: 10px; "
                              "border-bottom-left-radius: 0px; "
                              "border-bottom-right-radius: 0px; "
                              "border: none; "
                              "border-bottom: 1px solid #262222; }");
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
        topBarLayout->addSpacing(10);
        topBarLayout->addWidget(collaborationButton);
        topBarLayout->addSpacing(10); // Пробел
        topBarLayout->addWidget(helpButton);

        // Кнопки управления окном
        setupWindowControlButtons(MainWindow);

        // Добавление кнопок управления окном в верхнюю панель
        topBarLayout->addStretch();
        topBarLayout->addWidget(settings);
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
                "border: none; "
                "border-radius: 5px; "
                "padding: 5px 10px; "
                "}"
                "QPushButton#projectButton:hover { "
                "background-color: rgba(255, 255, 255, 0.3); "
                "}"
        );
        projectButton->setMenu(menuProject);

        // Создание кнопки "Collaboration"
        collaborationButton = new QPushButton("Collaboration", topBar);
        collaborationButton->setObjectName("collaborationButton");
        collaborationButton->setStyleSheet(
                "QPushButton#collaborationButton { "
                "background-color: #494850; "
                "color: #D8D8F6; "
                "border: none; "
                "border-radius: 5px; "
                "padding: 5px 10px; "
                "}"
                "QPushButton#collaborationButton:hover { "
                "background-color: rgba(255, 255, 255, 0.3); "
                "}"
        );
        collaborationButton->setMenu(menuCollaboration);

        // Создание кнопки "Help"
        helpButton = new QPushButton("Help", topBar);
        helpButton->setObjectName("helpButton");
        helpButton->setStyleSheet(
                "QPushButton#helpButton { "
                "background-color: #494850; "
                "color: #D8D8F6; "
                "border: none; "
                "border-radius: 5px; "
                "padding: 5px 10px; "
                "}"
                "QPushButton#helpButton:hover { "
                "background-color: rgba(255, 255, 255, 0.3); "
                "}"
        );
    }

    void setupWindowControlButtons(QMainWindow *MainWindow)
    {
        // Кнопка "Закрыть"
        closeButton = new QPushButton("", topBar);
        closeButton->setFixedSize(25, 25);
        closeButton->setIcon(QIcon("../Static/icons/close.ico")); // Установка иконки
        closeButton->setStyleSheet("QPushButton { background: none; border: none; color: white; border-radius: 5px;}"
                                   "QPushButton:hover { background-color: rgba(255, 255, 255, 0.3); }"); // Подсветка при наведении
        QObject::connect(closeButton, &QPushButton::clicked, MainWindow, &QMainWindow::close);

        // Кнопка "Свернуть"
        minimizeButton = new QPushButton("", topBar);
        minimizeButton->setFixedSize(25, 25);
        minimizeButton->setIcon(QIcon("../Static/icons/minus.ico"));
        minimizeButton->setStyleSheet("QPushButton { background: none; border: none; color: white; border-radius: 5px; }"
                                      "QPushButton:hover { background-color: rgba(255, 255, 255, 0.3); }"); // Подсветка при наведении
        QObject::connect(minimizeButton, &QPushButton::clicked, MainWindow, &QMainWindow::showMinimized);

        // Кнопка "Развернуть"
        maximizeButton = new QPushButton("", topBar);
        maximizeButton->setFixedSize(25, 25);
        maximizeButton->setIcon(QIcon("../Static/icons/big.ico"));
        maximizeButton->setStyleSheet("QPushButton { background: none; border: none; color: white; border-radius: 5px; }"
                                      "QPushButton:hover { background-color: rgba(255, 255, 255, 0.3); }"); // Подсветка при наведении
        QObject::connect(maximizeButton, &QPushButton::clicked, [=]() {
            if (MainWindow->isMaximized() || MainWindow->isFullScreen()) {
                MainWindow->showNormal(); // Возвращаем к обычному размеру
            } else {
                MainWindow->showMaximized(); // Разворачиваем окно
                MainWindow->update();        // Обновить стиль
            }
        });

        // Кнопка "Настройки"
        settings = new QPushButton("", topBar);
        settings->setFixedSize(25, 25);
        settings->setIcon(QIcon("../Static/icons/Vertical-Dots-Icon.ico"));
        settings->setStyleSheet("QPushButton { background: none; border: none; color: white; border-radius: 5px; }"
                                "QPushButton:hover { background-color: rgba(255, 255, 255, 0.3); }"); // Подсветка при наведении
    }

    void setupLeftMenu()
    {
        // Создание и настройка левого меню
        leftMenu = new QTreeWidget();
        leftMenu->setObjectName("leftMenu");
        leftMenu->headerItem()->setText(0, QString());

        QFont font1;
        QFont font2;
        font1.setKerning(true);
        font2.setPointSize(12);

        font1.setStyleStrategy(QFont::PreferDefault);

        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        QTreeWidgetItem *NotSee = new QTreeWidgetItem(leftMenu); // Объект для отступа
        NotSee->setFlags(NotSee->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEnabled); // Делаем неактивным

        itemFigures = new QTreeWidgetItem(leftMenu);
        itemRequirements = new QTreeWidgetItem(leftMenu);

        __qtreewidgetitem->setFont(0, font1);
        leftMenu->setHeaderItem(__qtreewidgetitem);
        leftMenu->setHeaderHidden(true);

        itemFigures->setFont(0, font2);
        itemRequirements->setFont(0, font2);

        QBrush blackBrush(QColor("#494850"));
        __qtreewidgetitem->setBackground(0, blackBrush);

        leftMenu->setStyleSheet(QString::fromUtf8(R"(
            QTreeWidget {
                background-color: #494850;
                color: #D8D8F6;
                border: none;
                border-bottom-left-radius: 10px;
                border-bottom-right-radius: 0px;
            }
        )"));

        leftMenu->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

        // Создание кнопки свернуть влево
        collapseButton = new QPushButton("⮘");
        collapseButton->setFixedSize(30, 30);
        collapseButton->setStyleSheet(
                "QPushButton { background: none; border: none; color: #D8D8F6; }"
                "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); }"
        );

        // Создание макета для leftMenuContainer
        leftMenuLayout = new QGridLayout();
        leftMenuLayout->setContentsMargins(0, 0, 0, 0);
        leftMenuLayout->setSpacing(0);

        // Добавление leftMenu и collapseButton в макет
        leftMenuLayout->addWidget(leftMenu, 0, 0);
        leftMenuLayout->addWidget(collapseButton, 0, 0, Qt::AlignTop | Qt::AlignRight);

        // Обновление контейнера для leftMenu
        leftMenuContainer = new QWidget();
        leftMenuContainer->setLayout(leftMenuLayout);

        leftMenuContainer->hide();
    }

    void setupMessage()
    {
        // Создание и настройка message
        message = new QFrame();
        message->setObjectName("message");

        message->setStyleSheet(QString::fromUtf8(R"(
            QFrame {
                background-color: #494850;
                color: #D8D8F6;
                border: none;
                border-bottom-left-radius: 10px;
                border-bottom-right-radius: 0px;
            }
        )"));

        message->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

        // Создание консоли для ввода внутри message
        messageConsole = new QLineEdit();
        messageConsole->setObjectName("messageConsole");

        QFont font2; // Шрифт
        font2.setPointSize(12);
        messageConsole->setFont(font2);
        messageConsole->setCursor(QCursor(Qt::CursorShape::IBeamCursor));
        messageConsole->setMinimumHeight(30);
        messageConsole->setStyleSheet(QString::fromUtf8(R"(
            background-color: #3e3d3d;
            color: #D8D8F6;
            border: 1px solid black;
            border-bottom-left-radius: 10px;
            border-bottom-right-radius: 0px;
        )"));

        // Создание кнопки свернуть
        messageCollapseButton = new QPushButton("⮘");
        messageCollapseButton->setFixedSize(30, 30);
        messageCollapseButton->setStyleSheet(
                "QPushButton { background: none; border: none; color: #D8D8F6; }"
                "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); }"
        );

        // Создание макета для message
        messageLayout = new QGridLayout();
        messageLayout->setContentsMargins(0, 0, 0, 0);
        messageLayout->setSpacing(0);

        // Добавление элементов в макет message
        messageLayout->addWidget(messageCollapseButton, 0, 0, Qt::AlignTop | Qt::AlignRight);
        messageLayout->addWidget(messageConsole, 1, 0);

        // Установка макета для message
        message->setLayout(messageLayout);

        // Создание контейнера для message
        messageContainer = new QWidget();
        messageContainerLayout = new QGridLayout();
        messageContainerLayout->setContentsMargins(0, 0, 0, 0);
        messageContainerLayout->setSpacing(0);
        messageContainerLayout->addWidget(message);
        messageContainer->setLayout(messageContainerLayout);
        messageContainer->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);

        // Изначально скрываем messageContainer
        messageContainer->hide();
    }

    void setupCollapsedPanel()
    {
        // Создание свернутой панели
        collapsedPanel = new QWidget(centralwindow);
        collapsedPanel->setFixedWidth(40);
        collapsedPanel->setStyleSheet("background-color: #494850; border-bottom-left-radius: 10px;\n"
                                      "border-bottom-right-radius: 0px;");

        // Создание вертикального макета с выравниванием по верхней части
        collapsedPanelLayout = new QVBoxLayout(collapsedPanel);
        collapsedPanelLayout->setContentsMargins(0, 0, 0, 0); // Убираем отступы
        collapsedPanelLayout->setSpacing(0); // Убираем промежутки между кнопками
        collapsedPanelLayout->setAlignment(Qt::AlignTop); // Выравниваем виджеты по верхней части

        // Создание кнопки для разворачивания меню элементов
        leftMenuElements = new QPushButton("", collapsedPanel);
        leftMenuElements->setFixedSize(40, 40);
        QIcon IcoElem("../Static/icons/icoLeftMenu.ico");
        leftMenuElements->setIcon(IcoElem);
        leftMenuElements->setStyleSheet(
                "QPushButton { background: none; border: none; color: #D8D8F6; border-radius: 0; }"
                "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 0; }"
        );
        collapsedPanelLayout->addWidget(leftMenuElements);

        // Создание кнопки для открытия чата
        leftMenuMessage = new QPushButton("", collapsedPanel);
        leftMenuMessage->setFixedSize(40, 40);
        QIcon IcoMes("../Static/icons/icoMes.ico");
        leftMenuMessage->setIcon(IcoMes);
        leftMenuMessage->setStyleSheet(
                "QPushButton { background: none; border: none; color: #D8D8F6; border-radius: 0; }"
                "QPushButton:hover { background-color: rgba(255, 255, 255, 0.1); border-radius: 0; }"
        );
        collapsedPanelLayout->addWidget(leftMenuMessage);
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
        console->setStyleSheet(QString::fromUtf8(R"(
            background-color: #3e3d3d;
            color: #D8D8F6;
            border: 1px solid black;
            border-top-left-radius: 10px;
            border-top-right-radius: 10px;
            border-bottom-left-radius: 0px;
            border-bottom-right-radius: 0px;
        )"));
    }

    void setupWorkWindow()
    {
        // Создание и настройка рабочего окна
        workWindow = new QFrame(centralwindow);
        workWindow->setObjectName("workWindow");
        workWindow->setFrameShape(QFrame::Shape::NoFrame);
        workWindow->setFrameShadow(QFrame::Shadow::Plain);
        workWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        workWindow->resize(674, 460);
    }

    void setupSettingsPanel()
    {
        // Создание панели настроек
        settingsPanel = new QWidget(nullptr, Qt::Popup);
        settingsPanel->setObjectName("settingsPanel");
        settingsPanel->setStyleSheet("background-color: #494850; border: 1px solid #443d3c; border-radius: 5px;");

        // Создание макета
        settingsLayout = new QVBoxLayout(settingsPanel);
        settingsLayout->setContentsMargins(10, 10, 10, 10);
        settingsLayout->setSpacing(10);

        // Добавление компонентов
        componentGrid = new QCheckBox("Grid", settingsPanel);
        componentGrid->setStyleSheet("color: #D8D8F6;background-color: #494850; border: none");
        componentGrid->setChecked(true);
        settingsLayout->addWidget(componentGrid);

        settingsPanel->adjustSize();
    }

    void setupConnections()
    {
        QObject::connect(collapseButton, &QPushButton::clicked, [=]() {
            leftMenuContainer->hide();
            collapsedPanel->show();
        });

        QObject::connect(leftMenuElements, &QPushButton::clicked, [=]() {
            collapsedPanel->hide();
            leftMenuContainer->show();
            messageContainer->hide();
        });

        QObject::connect(leftMenuMessage, &QPushButton::clicked, [=]() {
            collapsedPanel->hide();
            messageContainer->show();
            leftMenuContainer->hide();
        });

        QObject::connect(messageCollapseButton, &QPushButton::clicked, [=]() {
            messageContainer->hide();
            collapsedPanel->show();
        });

        QObject::connect(settings, &QPushButton::clicked, [=]() {
            QPoint pos = settings->mapToGlobal(QPoint(0, settings->height()));
            settingsPanel->move(pos);
            settingsPanel->adjustSize();
            settingsPanel->show();
        });

        QObject::connect(helpButton, &QPushButton::clicked, action_help, &QAction::trigger);
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
        action_help->setText(QCoreApplication::translate("MainWindow", "Help", nullptr));

        // Установка текста элементов левого меню
        const bool __sortingEnabled = leftMenu->isSortingEnabled();
        leftMenu->setSortingEnabled(false);

        itemFigures->setText(0, QCoreApplication::translate("MainWindow", "Figures", nullptr));
        itemRequirements->setText(0, QCoreApplication::translate("MainWindow", "Requirements", nullptr));

        leftMenu->setSortingEnabled(__sortingEnabled);

        // Очистка текста консоли
        console->setText(QCoreApplication::translate("MainWindow", "", nullptr));

        // Установка текста кнопок меню
        projectButton->setText(QCoreApplication::translate("MainWindow", "Project", nullptr));
        collaborationButton->setText(QCoreApplication::translate("MainWindow", "Collaboration", nullptr));
        helpButton->setText(QCoreApplication::translate("MainWindow", "Help", nullptr));

        // Установка текста настроек
        componentGrid->setText(QCoreApplication::translate("MainWindow", "Grid", nullptr));
    }
};

namespace Ui {
    class MainWindow : public Ui_MainWindow {};
}

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H