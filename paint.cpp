#include "paint.h"
#include "BMPfile.h"
#include "Arry.h"
#include "List.h"
#include "Assoc.h"
#include "objects.h"
#include <fstream>

ElementData::ElementData(){
    params = Arry<double>();
}
RequirementData::RequirementData(){
    objects = Arry<ID>();
    params = 0;
}
ID Paint::addRequirement(const RequirementData &rd) {
    if (rd.req == ET_POINTSECTIONDIST){
        point* p_it = nullptr;
        section *s_it = nullptr;
        try {
            p_it =  &(*(m_pointIDs.findByKey(rd.objects[0])));
        }
        catch (...){
            s_it =  &(*(m_sectionIDs.findByKey(rd.objects[0])));
        }
        if (p_it != nullptr){
            try {
                s_it =  &(*(m_sectionIDs.findByKey(rd.objects[1])));
            }
            catch (...){
                throw std::invalid_argument("No such section or point");
            }
        }else if (s_it!= nullptr){
            try {
                p_it =  &(*(m_pointIDs.findByKey(rd.objects[1])));
            }
            catch (...){
                throw std::invalid_argument("No such point or section");
            }
        }
        ReqPointSegDist requirement(p_it, s_it, rd.params);
        Arry<PARAMID> params = requirement.getParams();
        Arry<double> paramValues(params.getSize());
        paramValues.addElement((*(m_pointIDs.findByKey(rd.objects[0]))).x);
        paramValues.addElement((*(m_pointIDs.findByKey(rd.objects[0]))).y);
        paramValues.addElement((*(m_sectionIDs.findByKey(rd.objects[1]))).beg->x);
        paramValues.addElement((*(m_sectionIDs.findByKey(rd.objects[1]))).beg->y);
        paramValues.addElement((*(m_sectionIDs.findByKey(rd.objects[1]))).end->x);
        paramValues.addElement((*(m_sectionIDs.findByKey(rd.objects[1]))).end->y);
        Arry<double> derivatives(params.getSize());
        int k = 0;
        for (auto it = params.begin(); it != params.end(); ++it, ++k) {
            derivatives[k] = requirement.getDerivative(*it);
        }
        double alpha = 10e-10;
        double e = requirement.getError();
        while (e > 10e-10){
            alpha = e/(1+e);
            for(int i = 0; i < paramValues.getSize(); ++i){
                paramValues[i] += derivatives[i] * alpha;
            }
            (*(m_pointIDs.findByKey(rd.objects[0]))).x = paramValues[0];
            (*(m_pointIDs.findByKey(rd.objects[0]))).y = paramValues[1];
            (*(m_sectionIDs.findByKey(rd.objects[1]))).beg->x = paramValues[2];
            (*(m_sectionIDs.findByKey(rd.objects[1]))).beg->y = paramValues[3];
            (*(m_sectionIDs.findByKey(rd.objects[1]))).end->x = paramValues[4];
            (*(m_sectionIDs.findByKey(rd.objects[1]))).end->y = paramValues[5];
            e = requirement.getError();
        }
    }
    return ID{ -1 };
}
ID Paint::addElement(const ElementData& ed) {
    if (ed.et == ET_POINT) {
        point tmp;
        tmp.x = ed.params[0];
        tmp.y = ed.params[1];
        m_pointIDs.addPair(++s_maxID.id, m_pointStorage.addElement(tmp));
        return s_maxID;
    }
    if (ed.et == ET_SECTION) {
        point tmp1;
        tmp1.x = ed.params[0];
        tmp1.y = ed.params[1];
        auto beg = m_pointStorage.addElement(tmp1);
        m_pointIDs.addPair(++s_maxID.id, beg);
        point tmp2;
        tmp2.x = ed.params[2];
        tmp2.y = ed.params[3];
        auto end = m_pointStorage.addElement(tmp2);
        m_pointIDs.addPair(++s_maxID.id,end);
        section tmp;
        tmp.beg = &(*beg);
        tmp.end = &(*end);
        m_sectionIDs.addPair(++s_maxID.id,m_sectionStorage.addElement(tmp));
        return s_maxID;
    }
    if (ed.et == ET_CIRCLE) {
        point center;
        center.x = ed.params[0];
        center.y = ed.params[1];
        auto cent = m_pointStorage.addElement(center);
        m_pointIDs.addPair(++s_maxID.id, cent);
        circle tmp;
        tmp.center = &(*cent);
        tmp.R = ed.params[2];
        m_circleIDs.addPair(++s_maxID.id, m_circleStorage.addElement(tmp));
        return s_maxID;
    }
    return ID{ -1 };
}

ElementData Paint::getElementInfo(ID id) {
    ElementData result;

    try {
        auto p = m_pointIDs.findByKey(id);
        result.et = ET_POINT;
        result.params.addElement((*p).x);
        result.params.addElement((*p).y);
    }
    catch (const std::runtime_error&) {
        try {
            auto sec = m_sectionIDs.findByKey(id);
            result.et = ET_SECTION;
            result.params.addElement((*sec).beg->x);
            result.params.addElement((*sec).beg->y);
            result.params.addElement((*sec).end->x);
            result.params.addElement((*sec).end->y);
        }
        catch (const std::runtime_error&) {
            auto circ = m_circleIDs.findByKey(id);
            result.et = ET_CIRCLE;
            result.params.addElement((*circ).center->x);
            result.params.addElement((*circ).center->y);
            result.params.addElement((*circ).R);
        }
    }

    return result;
}

void Paint::paint() {
    for (auto point = m_pointStorage.begin(); point != m_pointStorage.end(); ++point) {
        c_bmpPainter.drawPoint(*point, false);
    }
    for (auto circle = m_circleStorage.begin(); circle != m_circleStorage.end(); ++circle) {
        c_bmpPainter.drawCircle(*circle, false);
    }
    for (auto section = m_sectionStorage.begin(); section != m_sectionStorage.end(); ++section) {
        c_bmpPainter.drawSection(*section, false);
    }
}
/*
void Paint::saveToFile(const char* file) {
    std::ofstream fout;
    fout.open(file);
    if (!(fout.is_open())) {
        throw "We can't open file";
    }
    fout << m_pointStorage.getSize();
    point prm;
    for(auto pos=m_pointIDs.begin(); pos!=m_pointIDs.end(); ++pos){
        fout<<(*pos).first.id();
        prm=*((*pos).second);
        fout<<prm.x;
        fout<<prm.y;
    }
    fout << "\n";
    fout << m_sectionStorage.getSize();
    section sct;
    bool end=false;
    for (auto pos=m_sectionIDs.begin(); pos!=m_sectionIDs.end(); ++pos) {
        end=false;
        fout<<(*pos).first().id();
        sct=*((*pos).second());
        for(auto pos=m_pointIDs.begin(); pos!=m_pointIDs.end() && !(end); ++pos){
            if(&(*((*pos).second))==sct.beg){
                fout<<(*pos).first.id();
                end=true;
            }
        }
        for(auto pos=m_pointIDs.begin(); pos!=m_pointIDs.end() && !(end); ++pos){
            if(&(*((*pos).second))==sct.end){
                fout<<(*pos).first.id();
                end=true;
            }
        }
    }
    fout << "\n";
    fout << m_circleStorage.getSize();
    circle crc;
    bool end=false;
    for (auto pos=m_sectionIDs.begin(); pos!=m_sectionIDs.end(); ++pos) {
        end=false;
        fout<<(*pos).first.id();
        crc=*((*pos).second);
        for(auto pos=m_pointIDs.begin(); pos!=m_pointIDs.end() && !(end); ++pos){
            if(&(*((*pos).second))==crc.center){
                fout<<(*pos).first.id();
                end=true;
            }
        }
        fout<<crc.R;
    }
    fout.close();
}


void Paint::loadFromFile(const char* filename) {
    std::ifstream files;
    files.open(filename);
    if (!(files.is_open())) {
        throw "We can't open file";
    }
    m_pointIDs = Assoc<ID, List<point>::iterator>();
    m_sectionIDs = Assoc<ID, List<section>::iterator>();
    m_circleIDs = Assoc<ID, List<circle>::iterator>();
    size_t size=0;
    files >> size;
    point need;
    ID id;
    s_maxID = 0;
    List<point>::iterator point_iter;
    m_pointStorage = List<point>(0);
    for (size_t i = 0; i < size; ++i) {
        files >> id;
        if (id > s_maxID) {
            s_maxID = id;
        }
        files >> need;//нужно создать ввод для таких элементов
        point_iter =m_pointStorage.addElement(need);
        m_pointIDs.addPair(id, point_iter);
    }
    files >> size;
    section work;
    List<section>::iterator section_iter;
    m_sectionStorage = List<section>(0);
    ID beg_section;
    ID end_section;
    for (size_t i = 0; i < size; ++i) {
        files >> id;
        if (id > s_maxID) {
            s_maxID = id;
        }
        files >> beg_section;
        files >> end_section;
        work.beg=&(*m_pointIDs.findByKey(beg_section));
        work.end=&(*m_pointIDs.findByKey(end_section));
        section_iter = m_sectionStorage.addElement(work);
        m_sectionIDs.addPair(id, section_iter);
    }
    files >> size;
    circle worker;
    List<circle>::iterator circle_iter;
    m_circleStorage = List<circle>(0);
    ID center;
    for (size_t i = 0; i < size; ++i) {
        files >> id;
        if (id > s_maxID) {
            s_maxID = id;
        }
        files >> center;
        worker.center=&(*m_pointIDs.findByKey(center));
        files >> worker.R;
        circle_iter = m_circleStorage.addElement(worker);
        m_circleIDs.addPair(id, circle_iter);
    }
}
*/

void Paint::exportToBMP(const char* file) {
    paint();
    try {
        c_bmpPainter.saveBMP(file);
    }
    catch (...) {
        throw std::invalid_argument("Can not opened file!");
    }
}
/*
void Paint::makeMySectionOrt(const ElementData& ed, ElementData& changing){
    if (ed.et != ET_SECTION or changing.et != ET_SECTION) {
        throw "Some of the elements is not section!";
    }
    //изменяем changing так, чтобы он был ортогонален с ed(cкорее всего, поворотом одной из точек)
}
void Paint::makeMySectionEqual(const ElementData& ed, ElementData& changing) {
    if (ed.et != ET_SECTION or changing.et != ET_SECTION) {
        throw "Some of the elements is not section!";
    }
    //изменяем changing так, чтобы его длина была равна длине ed (cкорее всего, поворотом одной из точек)
    //например поменяем Y второй точки
    double len2 = pow(ed.point1.x - ed.point2.x, 2) + pow(ed.point1.x - ed.point2.x, 2)
    changing.point1.y = sqrt(len2-pow(changing.point1.x -changing.point2.x, 2))+ changing.point1.y
}
void Paint::makeMySectionParallel(const ElementData& ed, ElementData& changing) {
    if (ed.et != ET_SECTION or changing.et != ET_SECTION) {
        throw "Some of the elements is not section!";
    }
    //изменяем changing так, чтобы он стал параллелен ed
}
void Paint::makeMySectionVertical(ElementData& changing) {
    if (changing.et != ET_SECTION) {
        throw "The element is not section!";
    }
    //отрезок становится строго вертикальным
    changing.point2.x=changing.point1.x; - чтобы были равны X координаты точек
}
void Paint::makeMySectionHorizontal(ElementData& changing) {
    if (changing.et != ET_SECTION) {
        throw "The element is not section!";
    }
    //отрезок становится строго горизонтальным2
    changing.point2.y=changing.point1.y; - чтобы были равны Y координаты точек
}
void Paint::makeMyCircleEqual(const ElementData& ed, ElementData& changing) {
    if (ed.et != ET_CIRCLE or changing.et != ET_CIRCLE) {
        throw "Some of the elements is not Circle!";
    }
    //окружности становятся одинакового размера
    changing.radius = ed.radius;
}
*/


void Paint::changeBMP(const BMPfile& file)
{
    c_bmpPainter = BMPpainter(file);
}

void Paint::changeBMP(const char* filename)
{
    c_bmpPainter = BMPpainter(BMPfile(filename));
}

double ReqPointSegDist::getError() {
    if (m_s->beg->x == m_s->end->x) {
        return std::abs(m_p->x - m_s->beg->x) - d;
    }
    if (m_s->beg->y == m_s->end->y) {
        return std::abs(m_p->y - m_s->beg->y) - d;
    }
    double A = -(m_s->end->x -m_s->beg->x) / (m_s->end->y - m_s->beg->y);
    double B = 1;
    double C = -(A * m_s->beg->x + B * m_s->beg->y);
    double e = (A * m_p->x + B * m_p->y + C)/ sqrt(A*A+B*B) - d;
    return e;
}

Arry<PARAMID> ReqPointSegDist::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_p->x));
    res.addElement(&(m_p->y));
    res.addElement(&(m_s->beg->x));
    res.addElement(&(m_s->beg->y));
    res.addElement(&(m_s->end->x));
    res.addElement(&(m_s->end->y));
    return res;
}

ReqPointSegDist::ReqPointSegDist(point *p, section *s, double dist) {
    m_p = p;
    m_s = s;
    d = dist;
}

double ReqPointSegDist::getDerivative(PARAMID param) {
    double x0 = m_p->x;
    double y0 = m_p->y;

    double x1 = m_s->beg->x;
    double y1 = m_s->beg->y;

    double x2 = m_s->end->x;
    double y2 = m_s->end->y;

    if (param == &m_p->x) { // x0
        double num = (-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2))
            * sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2));

        return num / den;
    }
    else if (param == &m_p->y) { // y0
        double num = (x1 - x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2))
            * sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num / den;
    }
    else if (param == &m_s->beg->x) { // x1
        double num1 = (-x1 + x2) * sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = (y0 - y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den2 = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 +
            x0 * (-y1 + y2)), 2)) * sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->beg->x) { // y1
        double num1 = (-y1 + y2) * sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = (-x0 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den2 = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 +
            x0 * (-y1 + y2)), 2)) * sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->end->x) { // x2
        double num1 = -(-x1 + x2) * sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = (-y0 + y1) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den2 = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 +
            x0 * (-y1 + y2)), 2)) * sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->end->x) { // y2
        double num1 = -(-y1 + y2) * sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2)), 2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = (x0 - x1) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den2 = sqrt(pow((-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 +
            x0 * (-y1 + y2)), 2)) * sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }

    return 0;
};

