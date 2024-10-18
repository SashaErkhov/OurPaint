#include "help.h"

Help::Help(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::Help)
{
    ui->setupUi(this);

    // Install event filter on the topBar to enable dragging
    ui->topBar->installEventFilter(this);

    // Connect buttons to slots
    connect(ui->commandsButton, &QPushButton::clicked, this, &Help::showCommands);
    connect(ui->requirementsButton, &QPushButton::clicked, this, &Help::showRequirements);
    connect(ui->aboutButton, &QPushButton::clicked, this, &Help::showAbout);

    // Initially show commands content
    showCommands();
}

Help::~Help()
{
    delete ui;
}

bool Help::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->topBar) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                dragPosition = mouseEvent->globalPos() - frameGeometry().topLeft();
                return true;
            }
        } else if (event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->buttons() & Qt::LeftButton) {
                move(mouseEvent->globalPos() - dragPosition);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void Help::showCommands()
{
    ui->helpContentLabel->setText(tr("point x y \t\t-\t Draw point at (x,y)\n\n"
                                     "section x1 y1 x2 y2 \t-\t Draw section between two points (x1,y1) and\t\t\t\t\t (x2,y2)\n\n"
                                     "cirle x y r \t\t-\t Draw circle with radius r and center at (x,y)\n\n"
                                     "getelement ID  \t\t-\t Get info about element with ID\n\n"
                                     "clear \t\t\t-\t Clears the screen\n\n"
                                     "addreq <instruction> <objects> <param> - \tdo something with objects \n\n"));
}

void Help::showRequirements()
{
    ui->helpContentLabel->setText(tr("Types of instructions:\n"
                                     "'1'  - Take 2 args: one point, and one section and distant that you want between them on <param>.\n"
                                     "'2'  - Take point and section and set it on each other(<param> can be empty)\n"
                                     "'3'  - Take 2 point(they may be on figure) and set distance between them on <param>\n"
                                     "'4'  - Take 2 point(they may be on figure) and set it on each other(<param> can be empty)\n"
                                     "'5'  - Take circle and section and set distance between them on <param>\n"
                                     "'6'  - Take circle and section and set section to be on circle (<param> can be empty)\n"
                                     "'7'  - Take circle and section, set the section completely inside circle (<param> can be empty)\n"
                                     "'8'  - Take 2 sections, set distance and parallel alignment between them on <param>\n"
                                     "'9'  - Take 2 sections, set them perpendicular to each other (<param> can be empty)\n"
                                     "'10' - Take 2 sections, set a specific angle between them on <param>"));
}

void Help::showAbout()
{
    ui->helpContentLabel->setText(tr("About project:\n\n"
                                     "OurPaint is a computer-aided design (CAD) system on the two-dimensional plane developed using the Qt framework."
                                     "It allows users to create and edit geometric primitives such as lines, points, and circles."
                                     "The program supports the ability to work collaboratively, making it suitable for team projects."));}