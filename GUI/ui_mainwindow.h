/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include <QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave_project_to;
    QAction *actionImport_project;
    QAction *actionExport_bmp;
    QAction *actionOpen_bmp;
    QAction *actionOpen_server;
    QAction *actionJoin_server;
    QAction *actionJoin_local_server;
    QAction *actionExit_from_session;
    QAction *action_help;
    QWidget *centralwindow;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QTreeWidget *leftMenu;
    QLineEdit *console;
    QFrame *workWindow;
    QMenuBar *upMenu;
    QMenu *menuCollaboration;
    QMenu *menuProject;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 576);
        MainWindow->setMinimumSize(QSize(640, 360));
        MainWindow->setMaximumSize(QSize(1920, 1080));
        MainWindow->setStyleSheet(QString::fromUtf8("background: \"#978897\""));
        actionSave_project_to = new QAction(MainWindow);
        actionSave_project_to->setObjectName("actionSave_project_to");
        QFont font;
        font.setPointSize(9);
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
        centralwindow = new QWidget(MainWindow);
        centralwindow->setObjectName("centralwindow");
        gridLayout_2 = new QGridLayout(centralwindow);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        leftMenu = new QTreeWidget(centralwindow);
        QFont font1;
        font1.setKerning(true);
        font1.setStyleStrategy(QFont::PreferDefault);
        leftMenu->headerItem()->setText(0, QString());
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(0, font1);
        leftMenu->setHeaderItem(__qtreewidgetitem);
        QFont font2;
        font2.setPointSize(12);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(leftMenu);
        __qtreewidgetitem1->setFont(0, font2);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem(leftMenu);
        __qtreewidgetitem2->setFont(0, font2);
        leftMenu->setObjectName("leftMenu");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leftMenu->sizePolicy().hasHeightForWidth());
        leftMenu->setSizePolicy(sizePolicy);
        leftMenu->setMinimumSize(QSize(50, 320));
        leftMenu->setStyleSheet(QString::fromUtf8("background: \"#494850\""));

        gridLayout->addWidget(leftMenu, 0, 0, 2, 1, Qt::AlignmentFlag::AlignLeft);

        console = new QLineEdit(centralwindow);
        console->setObjectName("console");
        console->setMinimumSize(QSize(0, 40));
        console->setFont(font2);
        console->setCursor(QCursor(Qt::CursorShape::IBeamCursor));
        console->setAutoFillBackground(false);
        console->setStyleSheet(QString::fromUtf8("background:\"#3e3d3d\""));

        gridLayout->addWidget(console, 1, 1, 1, 1);

        workWindow = new QFrame(centralwindow);
        workWindow->setObjectName("workWindow");
        workWindow->setMinimumSize(QSize(320, 180));
        workWindow->setMaximumSize(QSize(1920, 1080));
        workWindow->setStyleSheet(QString::fromUtf8("background: \"#ffffff\""));
        workWindow->setFrameShape(QFrame::Shape::StyledPanel);
        workWindow->setFrameShadow(QFrame::Shadow::Raised);


        gridLayout->addWidget(workWindow, 0, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwindow);
        upMenu = new QMenuBar(MainWindow);
        upMenu->setObjectName("upMenu");
        upMenu->setGeometry(QRect(0, 0, 1024, 25));
        upMenu->setFont(font);
        menuCollaboration = new QMenu(upMenu);
        menuCollaboration->setObjectName("menuCollaboration");
        menuCollaboration->setFont(font);
        menuProject = new QMenu(upMenu);
        menuProject->setObjectName("menuProject");
        menuHelp = new QMenu(upMenu);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(upMenu);

        upMenu->addAction(menuCollaboration->menuAction());
        upMenu->addAction(menuProject->menuAction());
        upMenu->addAction(menuHelp->menuAction());
        menuCollaboration->addAction(actionSave_project_to);
        menuCollaboration->addAction(actionImport_project);
        menuCollaboration->addAction(actionExport_bmp);
        menuCollaboration->addAction(actionOpen_bmp);
        menuProject->addAction(actionOpen_server);
        menuProject->addAction(actionJoin_server);
        menuProject->addAction(actionJoin_local_server);
        menuProject->addSeparator();
        menuProject->addAction(actionExit_from_session);
        menuHelp->addAction(action_help);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
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


        const bool __sortingEnabled = leftMenu->isSortingEnabled();
        leftMenu->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem = leftMenu->topLevelItem(0);
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "Figures", nullptr));
        QTreeWidgetItem *___qtreewidgetitem1 = leftMenu->topLevelItem(1);
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("MainWindow", "Requirements", nullptr));
        leftMenu->setSortingEnabled(__sortingEnabled);

        console->setText(QCoreApplication::translate("MainWindow", "", nullptr));
        menuCollaboration->setTitle(QCoreApplication::translate("MainWindow", "Project", nullptr));
        menuProject->setTitle(QCoreApplication::translate("MainWindow", "Collaboration", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    }

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
