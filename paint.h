#pragma once
#ifndef PAINT_23
#define PAINT_23

#include "BMPfile.h"
#include "Arry.h"
#include "List.h"
#include "objects.h"
#include <cmath>
#include "BMPpainter.h"
#include "Assoc.h"

enum Element {
    ET_POINT, ET_SECTION, ET_CIRCLE
};

struct ElementData {
    Element et;
    Arry<double> params;
};

class Paint {
    Assoc<ID, List<point>::iterator> pointIDs;
    Assoc<ID, List<section>::iterator> sectionIDs;
    Assoc<ID, List<circle>::iterator> circleIDs;

    ID maxID;

    List<point> pointList;
    List<section> sectionList;
    List<circle> circleList;

    BMPpainter bmpPainter;

    /*
    * TODO
    * All methods (addElement, getElementInfo, etc.) must be adapted to work with the new data structure,
    * utilizing Assoc methods for adding, searching, and deleting elements.
    */

public:
    // Добавление элементов с указанием их типа и необходимого набора параметров
    ID addElement(const ElementData &ed);

    // Получение информации об объекте    
    ElementData getElementInfo(ID id);

    // Сохранение данных в файл
    void saveToFile(const char *filename);

    // Экспорт в BMP файл
    void exportToBMP(const char *file);

    // Загрузка данных из файла
    void loadFromFile(const char *file);

    // Задает фон(изменяет filename файл)
    void changeBMP(const char* file);

    void paint();
};

#endif
