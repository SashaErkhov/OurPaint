#pragma once
#ifndef PAINT_23
#define PAINT_23

#define PARAMID double*

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

struct IReq {
    virtual double getError() = 0;
    virtual Arry<PARAMID> getParams() = 0;
    virtual double getDerivative(PARAMID) = 0;
};


class ReqPointSegDist :public IReq {
    point* m_pt;
    section* m_sect;

public:
    virtual double getError();
    virtual Arry<PARAMID> getParams() {
        Arry<PARAMID> pars;
        pars.addElement(&m_pt->x);
        pars.addElement(&m_pt->y);
        pars.addElement(&m_sect->beg->x);
        pars.addElement(&m_sect->beg->y);
        pars.addElement(&m_sect->end->x);
        pars.addElement(&m_sect->end->y);
        return pars;;
    };
    virtual double getDerivative(PARAMID param) {
        double x0 = m_pt->x;
        double y0 = m_pt->y;

        double x1 = m_sect->beg->x;
        double y1 = m_sect->beg->y;

        double x2 = m_sect->end->x;
        double y2 = m_sect->end->y;

        if (param == &m_pt->x) { // x0
            double num = (-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
            double den = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2))
                * sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2));

            return num / den;
        }
        else if (param == &m_pt->y) { // y0
            double num = (x1 - x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
            double den = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2))
                * sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

            return num / den;
        }
        else if (param == &m_sect->beg->x) { // x1
            double num1 = (-x1 + x2) * sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2));
            double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
            double num2 = (y0 - y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
            double den2 = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 +
                x0 * (-y1 + y2)), 2)) * sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

            return num1 / den1 + num2 / den2;
        }
        else if (param == &m_sect->beg->x) { // y1
            double num1 = (-y1 + y2) * sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2));
            double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
            double num2 = (-x0 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
            double den2 = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 +
                x0 * (-y1 + y2)), 2)) * sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

            return num1 / den1 + num2 / den2;
        }
        else if (param == &m_sect->end->x) { // x2
            double num1 = -(-x1 + x2) * sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2));
            double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
            double num2 = (-y0 + y1) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
            double den2 = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 +
                x0 * (-y1 + y2)), 2)) * sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

            return num1 / den1 + num2 / den2;
        }
        else if (param == &m_sect->end->x) { // y2
            double num1 = -(-y1 + y2) * sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2));
            double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
            double num2 = (x0 - x1) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
            double den2 = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 +
                x0 * (-y1 + y2)), 2)) * sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

            return num1 / den1 + num2 / den2;
        }

        return 0;
    };
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

    bool operator==(const Paint& left, const Paint& right);
};

#endif