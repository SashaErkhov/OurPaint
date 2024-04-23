#include "paint.h"
#include "BMPfile.h"
#include "Arry.h"
#include "List.h"
#include "Assoc.h"
#include "objects.h"
#include <fstream>

ElementData::ElementData(){
    et=ET_OUT;
    params=Arry<double>();
}

ID Paint::addElement(const ElementData& ed) {
    if (ed.et == ET_POINT) {
        point tmp;
        tmp.x = ed.params[0];
        tmp.y = ed.params[1];
        m_pointIDs.addPair(++s_maxID.id,m_pointStorage.addElement(tmp));
        return s_maxID;
    }
    if (ed.et == ET_SECTION) {
        point tmp1;
        tmp1.x = ed.params[0];
        tmp1.y = ed.params[1];        
        m_pointIDs.addPair(++s_maxID.id,m_pointStorage.addElement(tmp1));
        point tmp2;
        tmp2.x = ed.params[2];
        tmp2.y = ed.params[3];
        m_pointIDs.addPair(++s_maxID.id,m_pointStorage.addElement(tmp2));
        section tmp;
        tmp.beg = &tmp1;
        tmp.end = &tmp2;
        m_sectionIDs.addPair(++s_maxID.id,m_sectionStorage.addElement(tmp));
        return s_maxID;
    }
    if (ed.et == ET_CIRCLE) {
        point center;
        center.x = ed.params[0];
        center.y = ed.params[1];
        m_pointIDs.addPair(++s_maxID.id,m_pointStorage.addElement(center));
        circle tmp;
        tmp.center = &center;
        tmp.R = ed.params[2];
        m_circleIDs.addPair(++s_maxID.id,m_circleStorage.addElement(tmp));
        return s_maxID;
    }
    return ID{ -1 };
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

void Paint::exportToBMP(const char* file) {
    paint();
    try {
        c_bmpPainter.saveBMP(file);
    }
    catch (...) {
        throw std::invalid_argument("Can not opened file!");
    }
}

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
	/*double len2 = pow(ed.point1.x - ed.point2.x, 2) + pow(ed.point1.x - ed.point2.x, 2)
	changing.point1.y = sqrt(len2-pow(changing.point1.x -changing.point2.x, 2))+ changing.point1.y*/
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
	/*
	changing.point2.x=changing.point1.x; - чтобы были равны X координаты точек
	*/
}
void Paint::makeMySectionHorizontal(ElementData& changing) {
	if (changing.et != ET_SECTION) {
		throw "The element is not section!";
	}
	//отрезок становится строго горизонтальным2
	/*
	changing.point2.y=changing.point1.y; - чтобы были равны Y координаты точек
	*/
}
void Paint::makeMyCircleEqual(const ElementData& ed, ElementData& changing) {
	if (ed.et != ET_CIRCLE or changing.et != ET_CIRCLE) {
		throw "Some of the elements is not Circle!";
	}
	//окружности становятся одинакового размера
	/*
	changing.radius = ed.radius;
	*/
}


void Paint::changeBMP(const BMPfile& file)
{
    c_bmpPainter = BMPpainter(file);
}

void Paint::changeBMP(const char* filename)
{
    c_bmpPainter = BMPpainter(BMPfile(filename));
}
