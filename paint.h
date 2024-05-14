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
enum Requirement {
    ET_POINTONPOINT,
    ET_POINTPOINTDIST,
    ET_POINTONSECTION,
    ET_POINTSECTIONDIST,
    ET_SECTIONCIRCLEDIST,
    ET_SECTIONSECTIONDISTPARALLEL,
    ET_SECTIONSECTIONPERPENDICULAR,
    ET_SECTIONSECTIONANGEL,
    ET_SECTIONSECTIONANGELEQ,
    ET_SECTIONONCIRCLE,
    ET_SECTIONINCIRCLE
};

struct RequirementData {
    Requirement req;
    Arry<ID> objects;
    double params;
    RequirementData();
};
struct ElementData {
    Element et;
    Arry<double> params;
    ElementData();
};

#define PARAMID double*

struct IReq {
    virtual double getError() = 0;
    virtual Arry<PARAMID> getParams() = 0;
    virtual double getDerivative(PARAMID p) = 0;
};
class ReqPointSegDist : public IReq {
    point* m_p;
    section* m_s;
    double d;
public:
    ReqPointSegDist(point* p, section* s, double dist);
    double getError();
    Arry<PARAMID> getParams();
    double getDerivative(PARAMID p);
};
class ReqPointOnPoint: public IReq {
    point* m_p1;
    point* m_p2;
public:
    ReqPointOnPoint(point* p1, point* p2);
    double getError();
    Arry<PARAMID> getParams();
    double getDerivative(PARAMID p);
};
/* EXAMPLE
ReqPointSegDist req;
Arry<PARAMID> params = req.getParams();
Arry<double> paramValue(params.getSize());
Arry<double> derivatives(params.getSize());
// вычислить все частные производные
for(param in params) {
    derivatives[k] = req.getDerivative(param);
}
//изменили текущие состояние объектов
paramValues += (-alpha* derivatives);
// Вычислить ошибку для нового состояния
double e = req.getError();
// В зависимоти от величины ошибки продолжить или остановиться */

//c_ - класс, v_- переменная, s_структура, m_ - контейнеры(списки, массивы и другие)
class Paint {
    Assoc<ID, List<point>::iterator> m_pointIDs;
    Assoc<ID, List<section>::iterator> m_sectionIDs;
    Assoc<ID, List<circle>::iterator> m_circleIDs;
    Assoc<ID, List<RequirementData>::iterator> m_reqIDs;
    ID s_maxID;
    List<RequirementData> m_reqStorage;
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
    
    void paint();
};

#endif