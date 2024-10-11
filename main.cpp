#include "Arry.h"
#include "BMPfile.h"
#include "painter/objects.h"
#include "painter/paint.h"
#include "painter/QTPainter.h"
#include "GUI/mainwindow.h"
#include <memory>
#include <QApplication>
#include <QTranslator>
#include <QSplashScreen>
#include <QMainWindow>
#include <QTimer>
#include <QPixmap>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    /*   QPixmap splashPixmap("C:\\Users\\Tim\\CLionProjects\\OurPaint\\Static\\logo\\eye.png");
    QPixmap scaledPixmap = splashPixmap.scaled(splashPixmap.width() * 10, splashPixmap.height() * 10, Qt::KeepAspectRatio);
       QSplashScreen splash(splashPixmap);
       splash.setWindowFlags(Qt::SplashScreen | Qt::WindowStaysOnTopHint);
       splash.show();*/

    MainWindow w;

    auto painter = std::make_unique<QTPainter>(w.getUi(), w.getWorkWindow());
    painter->setParent(w.getWorkWindow());
    painter->show();
    Paint screen(painter.get());

    /*  QTimer::singleShot(1000, [&]() {
          splash.finish(&w);

      });*/
    w.show();


    QObject::connect(&w, &MainWindow::EnterPressed, [&w, &screen, &painter](const QString &command) {
        QStringList commandParts = command.split(' ');

        if (commandParts[0] == "point" && commandParts.size() == 3) {
            bool xOk, yOk;
            double x = commandParts[1].toDouble(&xOk);
            double y = commandParts[2].toDouble(&yOk);

            if (xOk && yOk) {
                ElementData point;
                point.et = ET_POINT;
                point.params.addElement(x);
                point.params.addElement(y);
                ID id = screen.addElement(point);
               // w.Print_LeftMenu(id.id, "Point", {x, y});
                screen.paint();
                painter->draw();
                w.setSave(false);
            }

        } else if (commandParts[0] == "circle" && commandParts.size() == 4) {
            bool xOk, yOk, rOk;
            double x = commandParts[1].toDouble(&xOk);
            double y = commandParts[2].toDouble(&yOk);
            double r = commandParts[3].toDouble(&rOk);

            if (xOk && yOk && rOk) {

                ElementData circle;
                circle.et = ET_CIRCLE;
                circle.params.addElement(x);
                circle.params.addElement(y);
                circle.params.addElement(r);
                ID id = screen.addElement(circle);
               // w.Print_LeftMenu(id.id-1, "Point", {x, y});
              //  w.Print_LeftMenu(id.id, "Circle", {x, y, r});
                screen.paint();
                painter->draw();
                w.setSave(false);
            }

        } else if (commandParts[0] == "section" && commandParts.size() == 5) {
            bool xOk, yOk, zOk, rOk;
            double x = commandParts[1].toDouble(&xOk);
            double y = commandParts[2].toDouble(&yOk);
            double z = commandParts[3].toDouble(&zOk);
            double r = commandParts[4].toDouble(&rOk);
            if (xOk && yOk && zOk && rOk) {
                ElementData section;
                section.et = ET_SECTION;
                section.params.addElement(x);
                section.params.addElement(y);
                section.params.addElement(z);
                section.params.addElement(r);
                ID id = screen.addElement(section);
               // w.Print_LeftMenu(id.id-2, "Point", {x, y});
                //w.Print_LeftMenu(id.id-1, "Point", {z, r});
                //w.Print_LeftMenu(id.id, "Section", {x, y, z, r});
                screen.paint();
                painter->draw();
                w.setSave(false);
            }

        }  else if (commandParts[0] == "exit") {
            w.close();
        } else if (commandParts[0] == "clear") {
            w.setSave(true);
            painter->clear();
            w.Print_LeftMenu(0, "Clear", {});
            screen.clear();
        }else if (commandParts[0] == "addreq" && commandParts.size() >3) {
            int req = commandParts[1].toInt();
            ID obj1=commandParts[2].toInt();
            ID obj2=commandParts[3].toInt();
            double parameters=commandParts[4].toDouble();
            RequirementData reqData;
            Requirement type;

            switch  (req)  {
                case 1:
                    type = ET_POINTSECTIONDIST;
                    reqData.req = type;
                    reqData.objects.addElement(obj1);
                    reqData.objects.addElement(obj2);
                    reqData.params.addElement(parameters);
                    screen.addRequirement(reqData);
                    painter->clear();
                    screen.paint();
                    painter->draw();
                    w.setSave(false);
                    break;
                case 2:
                    type = ET_POINTONSECTION;
                    reqData.req = type;
                    reqData.objects.addElement(obj1);
                    reqData.objects.addElement(obj2);
                    screen.addRequirement(reqData);
                    painter->clear();
                    screen.paint();
                    painter->draw();
                    w.setSave(false);
                    break;
                case 3:
                    type = ET_POINTPOINTDIST;
                    reqData.req = type;
                    reqData.objects.addElement(obj1);
                    reqData.objects.addElement(obj2);
                    reqData.params.addElement(parameters);
                    screen.addRequirement(reqData);
                    painter->clear();
                    screen.paint();
                    painter->draw();
                    w.setSave(false);
                    break;
                case 4:
                    type = ET_POINTONPOINT;
                    reqData.req = type;
                    reqData.objects.addElement(obj1);
                    reqData.objects.addElement(obj2);
                    screen.addRequirement(reqData);
                    painter->clear();
                    screen.paint();
                    painter->draw();
                    w.setSave(false);
                    break;
                case 5:
                    type = ET_SECTIONCIRCLEDIST;
                    reqData.req = type;
                    reqData.objects.addElement(obj1);
                    reqData.objects.addElement(obj2);
                    reqData.params.addElement(parameters);
                    screen.addRequirement(reqData);
                    painter->clear();
                    screen.paint();
                    painter->draw();
                    w.setSave(false);
                    break;
                case 6:
                    type = ET_SECTIONONCIRCLE;
                    reqData.req = type;
                    reqData.objects.addElement(obj1);
                    reqData.objects.addElement(obj2);
                    screen.addRequirement(reqData);
                    painter->clear();
                    screen.paint();
                    painter->draw();
                    w.setSave(false);
                    break;
                case 7:
                    type = ET_SECTIONINCIRCLE;
                    reqData.req = type;
                    reqData.objects.addElement(obj1);
                    reqData.objects.addElement(obj2);
                    screen.addRequirement(reqData);
                    painter->clear();
                    screen.paint();
                    painter->draw();
                    w.setSave(false);
                    break;
                case 8:
                    type = ET_SECTIONSECTIONPARALLEL;
                    reqData.req = type;
                    reqData.objects.addElement(obj1);
                    reqData.objects.addElement(obj2);
                    screen.addRequirement(reqData);
                    painter->clear();
                    screen.paint();
                    painter->draw();
                    w.setSave(false);
                    break;
                case 9:
                    type = ET_SECTIONSECTIONPERPENDICULAR;
                    reqData.req = type;
                    reqData.objects.addElement(obj1);
                    reqData.objects.addElement(obj2);
                    screen.addRequirement(reqData);
                    painter->clear();
                    screen.paint();
                    painter->draw();
                    w.setSave(false);
                    break;
                case 10:
                    type = ET_SECTIONSECTIONANGEL;
                    reqData.req = type;
                    reqData.objects.addElement(obj1);
                    reqData.objects.addElement(obj2);
                    reqData.params.addElement(parameters);
                    screen.addRequirement(reqData);
                    painter->clear();
                    screen.paint();
                    painter->draw();
                    w.setSave(false);
                    break;
                default:
                  //  std::cout << "Unknown requirement. Please read types of instructions by help command" << std::endl;
                    break;
            }
        }

        w.Print_LeftMenu(0, "Clear", {});
        std::vector<std::pair<ID, ElementData>> elements = screen.getAllElementsInfo();
        for (auto element: elements) {
            if (element.second.et == ET_POINT) {
                double x = element.second.params.getElement(0);
                double y = element.second.params.getElement(1);
                w.Print_LeftMenu(element.first.id, "Point", {x, y});
            } else if (element.second.et == ET_CIRCLE) {
                double x = element.second.params.getElement(0);
                double y = element.second.params.getElement(1);
                double r = element.second.params.getElement(2);
                w.Print_LeftMenu(element.first.id, "Circle", {x, y, r});
            } else if (element.second.et == ET_SECTION) {
                double x1 = element.second.params.getElement(0);
                double y1 = element.second.params.getElement(1);
                double x2 = element.second.params.getElement(2);
                double y2 = element.second.params.getElement(3);
                w.Print_LeftMenu(element.first.id, "Section", {x1, y1, x2, y2});
            }
        }

    });


    QObject::connect(&w, &MainWindow::projectSaved, [&screen, &w](const QString &fileName) {
        std::string File = fileName.toStdString();
        screen.saveToFile(File.c_str());
    });

    QObject::connect(&w, &MainWindow::LoadFile, [&screen, &painter, &w](const QString &fileName) {
        painter->clear();
        w.Print_LeftMenu(0, "Clear", {});
        std::string File = fileName.toStdString();
        screen.loadFromFile(File.c_str());
        screen.paint();
        std::vector<std::pair<ID, ElementData>> elements = screen.getAllElementsInfo();
        painter->draw();
        for (auto element: elements) {
            if (element.second.et == ET_POINT) {
                double x = element.second.params.getElement(0);
                double y = element.second.params.getElement(1);
                w.Print_LeftMenu(element.first.id, "Point", {x, y});
            } else if (element.second.et == ET_CIRCLE) {
                double x = element.second.params.getElement(0);
                double y = element.second.params.getElement(1);
                double r = element.second.params.getElement(2);
                w.Print_LeftMenu(element.first.id, "Circle", {x, y, r});
            } else if (element.second.et == ET_SECTION) {
                double x1 = element.second.params.getElement(0);
                double y1 = element.second.params.getElement(1);
                double x2 = element.second.params.getElement(2);
                double y2 = element.second.params.getElement(3);
                w.Print_LeftMenu(element.first.id, "Section", {x1, y1, x2, y2});
            }
        }
    });



    /*while (true) {
        char command[64];
        std::cin >> command;
        if (strcmp(command, "q") == 0) {
            std::cout << "Are you sure you want to exit without saving your work? Y/N" << std::endl;
            std::cin >> command;
            if (strcmp(command, "Y") == 0) {
                break;
            }
            if (strcmp(command, "N") == 0) {
                continue;
            }
        }
        if (strcmp(command, "help") == 0) {
            std::cout << "---------------------Commands:---------------------" << std::endl;
            std::cout << "point x y \t-\t Draw point at (x,y)" << std::endl;
            std::cout << "section x1 y1 x2 y2 \t-\t Draw section between two points (x1,y1) and (x2,y2)" << std::endl;
            std::cout << "cirle x y r \t-\t Draw circle with radius r and center at (x,y)" << std::endl;
            std::cout << "getelement ID  \t-\t Get info about element with ID" << std::endl;
            std::cout << "clear \t- \tClears the screen" << std::endl;
            std::cout << "export file_name.bmp \t- \tExports the image to file_name.bmp" << std::endl;
            std::cout << "save file_name.db \t- \tload the information about objects to file_name.db like a table" << std::endl;
            std::cout << "load <bmp/db> file_name.<bmp/db> \t- \tload the information about objects from file_name.db if db or"
                "just load the image from file_name.bmp" << std::endl;
            std::cout << "addreq <instruction> <objects> <param> \t- \tdo something with objects \n"
                "                 Types of instructions:\n"
                "                 '1'  - Take 2 args: one point, and one section and distant that you want between them on <param>.\n"
                "                 '2'  - Take point and section and set it on each other(<param> can be empty)\n"
                "                 '3'  - Take 2 point(they may be on figure) and set distance between them on <param>\n"
                "                 '4'  - Take 2 point(they may be on figure) and set it on each other(<param> can be empty)\n"
                "                 '5'  - Take circle and section and set distance between them on <param>\n"
                "                 '6'  - Take circle and section and set section to be on circle (<param> can be empty)\n"
                "                 '7'  - Take circle and section, set the section completely inside circle (<param> can be empty)\n"
                "                 '8'  - Take 2 sections, set distance and parallel alignment between them on <param>\n"
                "                 '9'  - Take 2 sections, set them perpendicular to each other (<param> can be empty)\n"
                "                 '10' - Take 2 sections, set a specific angle between them on <param>\n"
                << std::endl;
            std::cout << "q \t- \tQuit" << std::endl;
            std::cout << "help \t- \tHelp that show you commands to use" << std::endl;
        }
        if (strcmp(command, "point") == 0) {
            double x, y;
            std::cin >> x >> y;
            ElementData point;
            point.et = ET_POINT;
            point.params.addElement(x);
            point.params.addElement(y);
            ID id = screen.addElement(point);
            std::cout << "Point (" << x << ", " << y << ") added with id " << id.id << std::endl;
        }
        if (strcmp(command, "circle") == 0) {
            double x, y, r;
            std::cin >> x >> y >> r;
            ElementData circle;
            circle.et = ET_CIRCLE;
            circle.params.addElement(x);
            circle.params.addElement(y);
            circle.params.addElement(r);
            ID id = screen.addElement(circle);
            std::cout << "Circle with center on(" << x << ", " << y << ") and radius " << r << " added with id " << id.id << std::endl;
        }
        if (strcmp(command, "section") == 0) {
            double x1, y1, x2, y2;
            std::cin >> x1 >> y1 >> x2 >> y2;
            ElementData section;
            section.et = ET_SECTION;
            section.params.addElement(x1);
            section.params.addElement(y1);
            section.params.addElement(x2);
            section.params.addElement(y2);
            ID id = screen.addElement(section);
            std::cout << "Section between points (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << ") added with id " << id.id << std::endl;
        }
        if (strcmp(command, "export") == 0) {
            char fileName[256];
            std::cin >> fileName;
            try {
                screen.exportToBMP(fileName);
                std::cout << "Exported to " << fileName << " SUCCESS!" << std::endl;
            }
            catch (std::exception& e) {
                std::cout << "Exported to " << fileName << " FAILED (file not found)" << std::endl;
            }
        }
        if (strcmp(command, "save") == 0) {
            char fileName[256];
            std::cin >> fileName;
            try {
//                screen.saveToFile(fileName);
                std::cout << "Saved to " << fileName << " SUCCESS!" << std::endl;
            }
            catch (std::exception& e) {
                std::cout << "Saved to " << fileName << " FAILED (file not found)" << std::endl;
            }
        }
        if (strcmp(command, "load") == 0) {
            char fileResolution[4];
            char fileName[256];
            std::cin >> fileResolution >> fileName;
            try {
                if (strcmp(fileResolution, "db") == 0) {
 // unsecure                  screen.loadFromFile(fileName);
                }
                if (strcmp(fileResolution, "bmp") == 0) {
                    screen.changeBMP(fileName);
                }
                std::cout << "Loaded from " << fileName << " SUCCESS!" << std::endl;
            }
            catch (...) {
                std::cout << "Loaded from " << fileName << " FAILED!" << std::endl;
            }
        }
        if (strcmp(command, "getelement")==0){
            ID id;
            std::cin >> id.id;
            ElementData element = screen.getElementInfo(id);
            switch   (element.et)   {
                case ET_SECTION:
                    std::cout   <<  "TYPE: SECTION" <<std::endl;
                    std::cout << "START POINT: (" << element.params.getElement(0)  <<  ",  "  << element.params.getElement(1)  <<  ")"  << std::endl;
                    std::cout  <<  "END POINT:  ("  << element.params.getElement(2)  <<  ",   "  << element.params.getElement(3)  <<  ")"  << std::endl;
                    break;
                case ET_POINT:
                    std::cout  << "TYPE: POINT" << std::endl;
                    std::cout   << "X: " << element.params.getElement(0) << std::endl;
                    std::cout   <<  "Y:  "  << element.params.getElement(1)  << std::endl;
                    break;
                case ET_CIRCLE:
                    std::cout   <<  "TYPE: CIRCLE"  << std::endl;
                    std::cout << "CENTER: (" << element.params.getElement(0)  << ",  "  << element.params.getElement(1)  << ")"  << std::endl;
                    std::cout   <<   "RADIUS:   "   << element.params.getElement(2)   << std::endl;
                    break;
            }
        }
        if (strcmp(command, "addreq")  ==  0)  {
            int req;
            RequirementData reqData;
            ID obj1;
            ID obj2;
            Requirement type;
            double parameters;
            std::cin  >> req;
            switch  (req)  {
            case 1:
                type = ET_POINTSECTIONDIST;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                std::cin >> parameters;
                reqData.req = type;
                reqData.objects.addElement(obj1);
                reqData.objects.addElement(obj2);
                reqData.params.addElement(parameters);
                screen.addRequirement(reqData);
                break;
            case 2:
                type = ET_POINTONSECTION;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.addElement(obj1);
                reqData.objects.addElement(obj2);
                screen.addRequirement(reqData);
                break;
            case 3:
                type = ET_POINTPOINTDIST;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                std::cin >> parameters;
                reqData.req = type;
                reqData.objects.addElement(obj1);
                reqData.objects.addElement(obj2);
                reqData.params.addElement(parameters);
                screen.addRequirement(reqData);
                break;
            case 4:
                type = ET_POINTONPOINT;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.addElement(obj1);
                reqData.objects.addElement(obj2);
                screen.addRequirement(reqData);
                break;
            case 5:
                type = ET_SECTIONCIRCLEDIST;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                std::cin >> parameters;
                reqData.req = type;
                reqData.objects.addElement(obj1);
                reqData.objects.addElement(obj2);
                reqData.params.addElement(parameters);
                screen.addRequirement(reqData);
                break;
            case 6:
                type = ET_SECTIONONCIRCLE;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.addElement(obj1);
                reqData.objects.addElement(obj2);
                screen.addRequirement(reqData);
                break;
            case 7:
                type = ET_SECTIONINCIRCLE;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.addElement(obj1);
                reqData.objects.addElement(obj2);
                screen.addRequirement(reqData);
                break;
            case 8:
                type = ET_SECTIONSECTIONPARALLEL;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.addElement(obj1);
                reqData.objects.addElement(obj2);
                screen.addRequirement(reqData);
                break;
            case 9:
                type = ET_SECTIONSECTIONPERPENDICULAR;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                reqData.req = type;
                reqData.objects.addElement(obj1);
                reqData.objects.addElement(obj2);
                screen.addRequirement(reqData);
                break;
            case 10:
                type = ET_SECTIONSECTIONANGEL;
                std::cin >> obj1.id;
                std::cin >> obj2.id;
                std::cin >> parameters;
                reqData.req = type;
                reqData.objects.addElement(obj1);
                reqData.objects.addElement(obj2);
                reqData.params.addElement(parameters);
                screen.addRequirement(reqData);
                break;
            default:
                std::cout << "Unknown requirement. Please read types of instructions by help command" << std::endl;
                break;
            }
        }
        if (strcmp(command, "remr") == 0){
            ID req;
            std::cin >> req.id;
            screen.deleteRequirement(req);
        }
        if (strcmp(command, "undo") == 0){
            try {
                screen.undo();
            }catch (std::exception& e){
                std::cout << e.what() << std::endl;
            }
        }
        if (strcmp(command, "redo") == 0){
            try{
            screen.redo();
            }catch (std::exception& e){
                std::cout << e.what() << std::endl;
            }
        }
    }*/

    return a.exec();
}