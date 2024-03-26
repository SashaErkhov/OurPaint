#pragma once
#ifndef PAINT_23
#define PAINT_23
#include "./BMPfile/code/BMPfile.h"
#include "./additions/Arry.h"
#include "objects.h"

enum Element{ET_POINT, ET_SECTION, ET_CIRCLE};

struct ElementData{
    Element et;
    Arry<double> params;
};

class Paint
{
    struct idx{
        ID id;
        List<Point>::iterator it;
    }
    Array<idxPoint> m_pointIndex;
    List<point> m_pointStorage;
    List<section> m_sectionStorage;
    List<circle> m_circleStorage;
public:
    //Добавление элементов с указанием их типа и необходимого набора параметров
    ID addElement(const ElementData& ed);

    // Получение информации об объекте    
    ElementData getElementInfo(ID id);
     

    // Сохранение данных в файл
    void saveToFile(const char* filename);

    // Загрузка данных из файла
    void loadFromFile(const char* filename);

    //Отрисовка текущего состояния
    void paint();

};

#endif
