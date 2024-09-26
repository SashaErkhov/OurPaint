//
// Created by Tim on 25.09.2024.
//

#ifndef OURPAINT_PAINTER_H
#define OURPAINT_PAINTER_H

#include "objects.h"

class Painter {

public:
    virtual unsigned long long getWeight()=0;
    virtual unsigned long long getHeight()=0;
    virtual void changeSize(const rectangle& allObjects)=0;

    virtual void drawPoint(point pt, bool isWhite = false)=0;
    virtual void drawSection(section sec, bool isWhite = false)=0;
    virtual void drawCircle(circle c, bool isWhite = false)=0;
};


#endif //OURPAINT_PAINTER_H
