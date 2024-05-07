#include "Arry.h"
#include "BMPfile.h"
#include "objects.h"
#include "paint.h"
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
            std::cout << "clear \t- \tClears the screen" << std::endl;
            std::cout << "export file_name.bmp \t- \tExports the image to file_name.bmp" << std::endl;
            std::cout << "save file_name.db \t- \tload the information about objects to file_name.db like a table" << std::endl;
            std::cout << "load <bmp/db> file_name.<bmp/db> \t- \tload the information about objects from file_name.db if db or"
                "just load the image from file_name.bmp" << std::endl;
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
                screen.saveToFile(fileName);
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
 //                   screen.loadFromFile(fileName);
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
    }
}
