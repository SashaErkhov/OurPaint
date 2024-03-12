#pragma once
#ifndef PAINT_23
#define PAINT_23
#include "./PVM13.02.24/code/BMPfile.h"
#include "./additions/Array.h"
#include "objects.h"

enum Element{ET_POINT, ET_SECTION, ET_CIRCLE};
class Paint
{
    Arry<point> pointar;
    Arry<section> sectionar;
    Arry<circle> circlear;
public:
    //Добавление элементов с указанием их типа и необходимого набора параментов
    void addElement(Element et, const Arry<double>& params);
    //Отрисовка текущего состояния
    void paint();

};

#endif
