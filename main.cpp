#include "Arry.h"
#include "BMPfile.h"
#include "painter/objects.h"
#include "painter/paint.h"
#include <cstring>
int main()
{
    Paint screen;
    std::cout << "-------------------------------------OurPaint-------------------------------------" << std::endl;
    std::cout << "This programm will help you to make different shapes, like such as line, point, circle." << std::endl;
    std::cout << "To find out the commands, write \"help\"" << std::endl;
    while (true) {
        std::cout << ">";
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
            screen.undo();
        }
        if (strcmp(command, "redo") == 0){
            screen.redo();
        }
    }
}
