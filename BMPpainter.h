#ifndef OURPAINT_BMPPAINTER_H
#define OURPAINT_BMPPAINTER_H
#include "./BMPfile/code/BMPfile.h"
#include "objects.h"
#include <cmath>

class BMPpainter {
    BMPfile file;
public:
    BMPpainter(unsigned int weight = 1000, unsigned int height = 1000): file(weight, height){}
    BMPpainter(const BMPfile &file);
    BMPpainter(const BMPpainter &other);
    BMPpainter &operator=(const BMPpainter &other);
    void drawPoint(point pt, bool isWhite = false);
    void drawSection(section sec, bool isWhite = false);
    void drawCircle(circle c, bool isWhite = false);
    void saveBMP(const char* filename="print.bmp");
};

#endif //OURPAINT_BMPPAINTER_H
