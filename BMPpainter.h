#ifndef OURPAINT_BMPPAINTER_H
#define OURPAINT_BMPPAINTER_H
#include "./BMPfile/code/BMPfile.h"
#include "objects.h"
#include <cmath>

class BMPpainter {
    BMPfile filename;
public:
    BMPpainter();
    BMPpainter(const char* filename);
    void drawPoint(point pt, bool isWhite = false);
    void drawSection(section sec, bool isWhite = false);
    void drawCircle(circle c, bool isWhite = false);
    void saveBMP(const char* filename);
};

#endif //OURPAINT_BMPPAINTER_H
