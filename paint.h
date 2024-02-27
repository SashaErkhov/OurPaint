#pragma once
#ifndef PAINT_23
#define PAINT_23

#include "Array.h"
#include "objects.h"

enum Element{ET_POINT, ET_SECTION, ET_CIRCLE};
class Paint
{
    Arry<point> pointar;
    Arry<section> sectionar;
    Arry<circle> circlear;
public:
    void addElement(Element et, const Arry* <& parametr>);
};

#endif