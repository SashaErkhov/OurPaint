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

    ElementData();
};
//c_ - класс, v_- переменная, s_структура, m_ - контейнеры(списки, массивы и другие)
class Paint {
    Assoc<ID, List<point>::iterator> m_pointIDs;
    Assoc<ID, List<section>::iterator> m_sectionIDs;
    Assoc<ID, List<circle>::iterator> m_circleIDs;

    ID s_maxID;

    List<point> m_pointStorage;
    List<section> m_sectionStorage;
    List<circle> m_circleStorage;

    BMPpainter c_bmpPainter;

    /*
    * TODO
    * All methods (addElement, getElementInfo, etc.) must be adapted to work with the new data structure,
    * utilizing Assoc methods for adding, searching, and deleting elements.
    */

public:
    Paint() : s_maxID(0), c_bmpPainter(), m_pointIDs(), m_sectionIDs(), m_circleIDs(), m_pointStorage(),
              m_sectionStorage(), m_circleStorage() {}//default constructor
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
    void changeBMP(const BMPfile& file);
    void changeBMP(const char* filename);

    void paint();
};

#endif
