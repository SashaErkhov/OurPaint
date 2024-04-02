//
// Created by Eugene Bychkov on 02.04.2024.
//

#ifndef OURPAINT_BMPPAINTER_H
#define OURPAINT_BMPPAINTER_H
#include "./BMPfile/code/BMPfile.h"
#include "objects.h"
#include <cmath>
class BMPpainter {
    BMPfile filename;
public:
    BMPpainter(const char* filename);
    void drawPoint(point pt, bool isWhite=false);
    void drawSection(section sec, bool isWhite=false);
    void drawCircle(circle c, bool isWhite=false);
};


#endif //OURPAINT_BMPPAINTER_H
