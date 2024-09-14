#include "paint.h"
#include "../Matrix/Matrix.h"
#include <map>

ElementData::ElementData() {
    params = Arry<double>();
}

ID Paint::addRequirement(const RequirementData &rd) {
    c_bmpPainter = BMPpainter();
    ActionsInfo info;

    m_reqD.addElement(rd);
    Arry<IReq*> allRequirements;
    std::map<PARAMID, double*> allParams;
    Arry<double> allParamValues;

    // Сбор всех требований и их параметров
    for (const auto& rd : m_reqD) {
        IReq* requirement = nullptr;

        // 1
        if (rd.req == ET_POINTSECTIONDIST) {
            point* p_it = &(*(m_pointIDs[rd.objects[0]]));
            section* s_it = &(*(m_sectionIDs[rd.objects[1]]));
            requirement = new ReqPointSecDist(p_it, s_it, rd.params[0]);
        }
        // 2
        else if (rd.req == ET_POINTONSECTION) {
            point* p_it = &(*(m_pointIDs[rd.objects[0]]));
            section* s_it = &(*(m_sectionIDs[rd.objects[1]]));
            requirement = new ReqPointOnSec(p_it, s_it);
        }
        // 3
        else if (rd.req == ET_POINTPOINTDIST) {
            point* p1_it = &(*(m_pointIDs[rd.objects[0]]));
            point* p2_it = &(*(m_pointIDs[rd.objects[1]]));
            requirement = new ReqPointPointDist(p1_it, p2_it, rd.params[0]);
        }
        // 4
        else if (rd.req == ET_POINTONPOINT) {
            point* p1_it = &(*(m_pointIDs[rd.objects[0]]));
            point* p2_it = &(*(m_pointIDs[rd.objects[1]]));
            requirement = new ReqPointOnPoint(p1_it, p2_it);
        }
        // 5
        else if (rd.req == ET_SECTIONCIRCLEDIST) {
            circle* c_it = &(*(m_circleIDs[rd.objects[0]]));
            section* s_it = &(*(m_sectionIDs[rd.objects[0]]));
            requirement = new ReqSecCircleDist(s_it, c_it, rd.params[0]);
        }
        // 6
        else if (rd.req == ET_SECTIONONCIRCLE) {
            circle* c_it = &(*(m_circleIDs[rd.objects[0]]));
            section* s_it = &(*(m_sectionIDs[rd.objects[1]]));
            requirement = new ReqSecOnCircle(s_it, c_it);
        }
        // 7
        else if (rd.req == ET_SECTIONINCIRCLE) {
            circle* c_it = &(*(m_circleIDs[rd.objects[0]]));
            section* s_it = &(*(m_sectionIDs[rd.objects[1]]));
            requirement = new ReqSecInCircle(s_it, c_it);
        }
        // 8
        else if (rd.req == ET_SECTIONSECTIONPARALLEL) {
            section* s1_it = &(*(m_sectionIDs[rd.objects[0]]));
            section* s2_it = &(*(m_sectionIDs[rd.objects[1]]));
            requirement = new ReqSecSecParallel(s1_it, s2_it);
        }
        // 9
        else if (rd.req == ET_SECTIONSECTIONPERPENDICULAR) {
            section* s1_it = &(*(m_sectionIDs[rd.objects[0]]));
            section* s2_it = &(*(m_sectionIDs[rd.objects[1]]));
            requirement = new ReqSecSecPerpendicular(s1_it, s2_it);
        }
        // 10
        else if (rd.req == ET_SECTIONSECTIONANGEL) {
            section* s1_it = &(*(m_sectionIDs[rd.objects[0]]));
            section* s2_it = &(*(m_sectionIDs[rd.objects[1]]));
            requirement = new ReqSecSecAngel(s1_it, s2_it, rd.params[0]);
        }


        if (requirement) {
            allRequirements.addElement(requirement);
            Arry<PARAMID> params = requirement->getParams();
            for (size_t i = 0; i < params.getSize(); i++) {
                if (allParams.find(params[i]) == allParams.end()) {
                    allParams[params[i]] = &(*params[i]);
                    allParamValues.addElement(*params[i]);
                }
            }
        }
    }

    // Gauss-Newton algorithm
    double error = 0.0;
    for (const auto& req : allRequirements) {
        error += std::pow(req->getError(), 2);
    }
    double lambda = 5;
    double prevError = std::numeric_limits<double>::max();
    while (error > 10e-2) {

        Matrix<double> jacobiMatrix(allRequirements.getSize(), allParams.size());
        Matrix<double> errors(allRequirements.getSize(), 1);

        // Jacobi matrix
        for (size_t i = 0; i < allRequirements.getSize(); ++i) {
            Arry<PARAMID> params = allRequirements[i]->getParams();
            for (size_t j = 0; j < params.getSize(); ++j) {
                jacobiMatrix.setElement(i, j, allRequirements[i]->getDerivative(params[j]));
            }
            errors.setElement(i, 0, allRequirements[i]->getError());
        }

        Matrix<double> jacobiTra = jacobiMatrix.transpose();
        Matrix<double> normalMatrix = jacobiTra * jacobiMatrix;
        if (error > prevError) {
            lambda *= 2;
        } else {
            lambda /= 2;
        }
        prevError = error;
        for (int i = 0; i < normalMatrix.cols_size(); ++i) {
            normalMatrix.setElement(i, i, normalMatrix.getElement(i, i) + lambda);
        }
        Matrix<double> inverseNormalMatrix = normalMatrix.invMatrix();
        Matrix<double> delta = inverseNormalMatrix * jacobiTra * errors;
        double alpha = 0.01;
        size_t index = 0;
        for (auto& param : allParams) {
            *(param.second) -= alpha * delta.getElement(index++, 0);
        }

        // Check error
        error = 0.0;
        for (const auto& req : allRequirements) {
            error += std::pow(req->getError(), 2);
        }
        if (prevError - error < 10e-6) throw std::runtime_error("UNKNOWN ERROR");
    }

    
    // undo/redo
    for (const auto& req : allRequirements) {
        Arry<PARAMID> params = req->getParams();
        Arry<double> beforeValues, afterValues;
        for (size_t i = 0; i < params.getSize(); ++i) {
            beforeValues.addElement(*params[i]);
            afterValues.addElement(allParamValues[i]);
        }
        info.m_paramsBefore.addElement(beforeValues);
        info.m_paramsAfter.addElement(afterValues);
        s_allFigures = s_allFigures || req->getRectangle();
    }
    // Clear
    for (auto requirement : allRequirements) {
        delete requirement;
    }

    c_undoRedo.add(info);
    return s_maxID.id;
}

ID Paint::addElement(const ElementData &ed) {
    ActionsInfo info;
    if (ed.et == ET_POINT) {
        point tmp;
        tmp.x = ed.params[0];
        tmp.y = ed.params[1];
        s_allFigures = s_allFigures || tmp.rect();
        m_pointIDs[++s_maxID.id] = m_pointStorage.addElement(tmp);
        info.m_objects.addElement(s_maxID);
        info.m_paramsAfter.addElement(ed.params);
        c_undoRedo.add(info);
        return s_maxID;
    }
    if (ed.et == ET_SECTION) {
        point tmp1;
        tmp1.x = ed.params[0];
        tmp1.y = ed.params[1];
        auto beg = m_pointStorage.addElement(tmp1);
        m_pointIDs[++s_maxID.id] = beg;
        Arry<double> params1;
        params1.addElement(ed.params[0]);
        params1.addElement(ed.params[1]);
        info.m_objects.addElement(s_maxID);
        info.m_paramsAfter.addElement(params1);
        point tmp2;
        tmp2.x = ed.params[2];
        tmp2.y = ed.params[3];
        auto end = m_pointStorage.addElement(tmp2);
        m_pointIDs[++s_maxID.id] = end;
        Arry<double> params2;
        params2.addElement(ed.params[2]);
        params2.addElement(ed.params[3]);
        info.m_objects.addElement(s_maxID);
        info.m_paramsAfter.addElement(params2);
        section tmp;
        tmp.beg = &(*beg);
        tmp.end = &(*end);
        s_allFigures = s_allFigures || tmp.rect();
        m_sectionIDs[++s_maxID.id] = m_sectionStorage.addElement(tmp);
        info.m_objects.addElement(s_maxID);
        info.m_paramsAfter.addElement(ed.params);
        c_undoRedo.add(info);
        return s_maxID;
    }
    if (ed.et == ET_CIRCLE) {
        point center;
        center.x = ed.params[0];
        center.y = ed.params[1];
        Arry<double> params1;
        params1.addElement(ed.params[0]);
        params1.addElement(ed.params[1]);
        info.m_paramsAfter.addElement(params1);
        auto cent = m_pointStorage.addElement(center);
        m_pointIDs[++s_maxID.id] = cent;
        info.m_objects.addElement(s_maxID);
        circle tmp;
        tmp.center = &(*cent);
        tmp.R = ed.params[2];
        s_allFigures = s_allFigures || tmp.rect();
        m_circleIDs[++s_maxID.id] = m_circleStorage.addElement(tmp);
        info.m_objects.addElement(s_maxID);
        params1.addElement(ed.params[2]);
        info.m_paramsAfter.addElement(params1);
        c_undoRedo.add(info);
        return s_maxID;
    }
    return ID{-1};
}

ElementData Paint::getElementInfo(ID id) {
    ElementData result;

    try {
        auto p = m_pointIDs[id];
        result.et = ET_POINT;
        result.params.addElement((*p).x);
        result.params.addElement((*p).y);
    }
    catch (const std::runtime_error &) {
        try {
            auto sec = m_sectionIDs[id];
            result.et = ET_SECTION;
            result.params.addElement((*sec).beg->x);
            result.params.addElement((*sec).beg->y);
            result.params.addElement((*sec).end->x);
            result.params.addElement((*sec).end->y);
        }
        catch (const std::runtime_error &) {
            auto circ = m_circleIDs[id];
            result.et = ET_CIRCLE;
            result.params.addElement((*circ).center->x);
            result.params.addElement((*circ).center->y);
            result.params.addElement((*circ).R);
        }
    }

    return result;
}

RequirementData Paint::getRequirementInfo(ID id) {
    RequirementData result;
    auto req = m_reqIDs[id];
    Arry<PARAMID> paramIDs = (*req)->getParams();
    Arry<double> params;
    for (auto paramID = paramIDs.begin(); paramID != paramIDs.end(); ++paramID) {
        params.addElement(*(*paramID));
    }
    result.params = params;
    return result;
}

void Paint::paint() {
    c_bmpPainter.changeSize(s_allFigures);
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

void Paint::exportToBMP(const char *file) {
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


void Paint::changeBMP(const BMPfile &file) {
    c_bmpPainter = BMPpainter(file);
}

void Paint::changeBMP(const char *filename) {
    c_bmpPainter = BMPpainter(BMPfile(filename));
}


void Paint::deleteRequirement(ID req) {
    try {
        m_reqStorage.remove(m_reqIDs[req]);
    } catch (...) {
        std::cout << "no requirement with ID " << req.id << std::endl;
    }
}

void Paint::undo() {
    ActionsInfo info = c_undoRedo.undo();
    point *p = nullptr;
    section *s = nullptr;
    circle *c = nullptr;
    if (info.m_paramsBefore.getSize() == 0) {
        for (int i = 0; i < info.m_objects.getSize(); ++i) {
            if (m_pointIDs.contains(info.m_objects[i])) {
                m_pointStorage.remove(m_pointIDs[info.m_objects[i]]);
            }else if (m_sectionIDs.contains(info.m_objects[i])) {
                m_sectionStorage.remove(m_sectionIDs[info.m_objects[i]]);
            }else if (m_circleIDs.contains(info.m_objects[i])){
                m_circleStorage.remove(m_circleIDs[info.m_objects[i]]);
            } else {
                std::cout << "No ID to undo" << std::endl;
                break;
            }
        }
        return;
    }
    for (int i = 0; i < info.m_objects.getSize(); ++i) {
        try {
            p = &(*m_pointIDs[info.m_objects[i]]);
            p->x = info.m_paramsBefore[i][0];
            p->y = info.m_paramsBefore[i][1];
        } catch (...) {
            try {
                s = &(*m_sectionIDs[info.m_objects[i]]);
                s->beg->x = info.m_paramsBefore[i][0];
                s->beg->y = info.m_paramsBefore[i][1];
                s->end->x = info.m_paramsBefore[i][2];
                s->end->y = info.m_paramsBefore[i][3];
            } catch (...) {
                try {
                    c = &(*m_circleIDs[info.m_objects[i]]);
                    c->center->x = info.m_paramsBefore[i][0];
                    c->center->y = info.m_paramsBefore[i][1];
                } catch (...) {
                    std::cout << "No ID to undo" << std::endl;
                    break;
                }
            }
        }
    }
}

void Paint::redo() {
    ActionsInfo info = c_undoRedo.redo();
    point *p = nullptr;
    section *s = nullptr;
    circle *c = nullptr;
    if (info.m_paramsBefore.getSize() == 0) {
        if (info.m_objects.getSize() == 3){
            point beg;
            beg.x = info.m_paramsAfter[0][0];
            beg.y = info.m_paramsAfter[0][1];
            auto p1 = m_pointStorage.addElement(beg);
            m_pointIDs[info.m_objects[0]] = p1;
            point end;
            end.x = info.m_paramsAfter[1][0];
            end.y = info.m_paramsAfter[1][1];
            auto p2 = m_pointStorage.addElement(end);
            m_pointIDs[info.m_objects[1]] = p2;
            section sec;
            sec.beg = &(*(p1));
            sec.end = &(*(p2));
            m_sectionIDs[info.m_objects[2]] = m_sectionStorage.addElement(sec);
            s_allFigures = s_allFigures || sec.rect();
        }else if (info.m_objects.getSize() == 2){
            point center;
            center.x = info.m_paramsAfter[0][0];
            center.y = info.m_paramsAfter[0][1];
            circle circ;
            auto p1 = m_pointStorage.addElement(center);
            m_pointIDs[info.m_objects[0]] = p1;
            circ.center = &(*(p1));
            circ.R = info.m_paramsAfter[1][2];
            m_circleIDs[info.m_objects[1]] = m_circleStorage.addElement(circ);
            s_allFigures = s_allFigures || circ.rect();
        } else if (info.m_objects.getSize() == 1){
            point pt;
            pt.x = info.m_paramsAfter[0][0];
            pt.y = info.m_paramsAfter[0][1];
            m_pointIDs[info.m_objects[0]] = m_pointStorage.addElement(pt);
            s_allFigures = s_allFigures || pt.rect();
        }
        return;
    }
    for (int i = 0; i < info.m_objects.getSize(); ++i) {
        try {
            p = &(*m_pointIDs[info.m_objects[i]]);
            p->x = info.m_paramsAfter[i][0];
            p->y = info.m_paramsAfter[i][1];
        } catch (...) {
            try {
                s = &(*m_sectionIDs[info.m_objects[i]]);
                s->beg->x = info.m_paramsAfter[i][0];
                s->beg->y = info.m_paramsAfter[i][1];
                s->end->x = info.m_paramsAfter[i][2];
                s->end->y = info.m_paramsAfter[i][3];
            } catch (...) {
                try {
                    c = &(*m_circleIDs[info.m_objects[i]]);
                    c->center->x = info.m_paramsAfter[i][0];
                    c->center->y = info.m_paramsAfter[i][1];
                } catch (...) {
                    std::cout << "No ID to redo" << std::endl;
                    break;
                }
            }
        }
    }
}
