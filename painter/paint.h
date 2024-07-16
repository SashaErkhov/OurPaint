#pragma once
#ifndef PAINT_23
#define PAINT_23

#include "BMPfile.h"
#include "List.h"
#include "objects.h"
#include "BMPpainter.h"
#include "Assoc.h"
#include "../Matrix/Matrix.h"
#include "enums.h"
#include "requirements.h"
#include "UndoRedo.h"
struct ElementData {
    Element et;
    Arry<double> params;
    ElementData();
};
struct RequirementInfo{
    Requirement s_req;
    ID s_object1;
    ID s_object2;
    Arry<double> m_paramsBefore;
    Arry<double> m_paramsAfter;
};

//c_ - класс, v_- переменная, s_структура, m_ - контейнеры(списки, массивы и другие) f_ - приватный метод класса
class Paint {
    UndoRedo<RequirementInfo> c_undoRedo;
    Assoc<ID, List<point>::iterator> m_pointIDs;
    Assoc<ID, List<section>::iterator> m_sectionIDs;
    Assoc<ID, List<circle>::iterator> m_circleIDs;
    Assoc<ID, List<IReq*>::iterator> m_reqIDs;
    ID s_maxID;
    List<IReq*> m_reqStorage;
    List<point> m_pointStorage;
    List<section> m_sectionStorage;
    List<circle> m_circleStorage;
    BMPpainter c_bmpPainter;
    rectangle s_allFigures;
    /*
    * TODO
    * All methods (addElement, getElementInfo, etc.) must be adapted to work with the new data structure,
    * utilizing Assoc methods for adding, searching, and deleting elements.
    */

public:
    Paint() : s_maxID(0), c_bmpPainter(), m_pointIDs(), m_sectionIDs(), m_circleIDs(), m_pointStorage(),
        m_sectionStorage(), m_circleStorage() {
        s_allFigures.x_1 = -10;
        s_allFigures.x_2 = 10;
        s_allFigures.y_1 = -10;
        s_allFigures.y_2 = 10;
    } //default constructor
        
    // Добавление элементов с указанием их типа и необходимого набора параметров
    ID addElement(const ElementData &ed);
    
    // Добавление требованием
    ID addRequirement(const RequirementData &rd);

    // Получение информации об объекте    
    ElementData getElementInfo(ID id);
    
    RequirementData getRequirementInfo(ID id);

    // Сохранение данных в файл
    void saveToFile(const char *filename);
    
    // Экспорт в BMP файл
    void exportToBMP(const char *file);
    
    // Загрузка данных из файла
    void loadFromFile(const char *file);
    
    // Задает фон(изменяет filename файл)
    void changeBMP(const BMPfile& file);
    void changeBMP(const char* filename);

    void deleteElement(ID elem);
    void deleteRequirement(ID req);

    void undoReq();
    void redoReq();

    void paint();
};

#endif