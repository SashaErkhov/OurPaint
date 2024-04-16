#include "paint.h"
#include "BMPfile.h"
#include "Arry.h"
#include "List.h"
#include "objects.h"
#include <fstream>


ID Paint::addElement(const ElementData& ed) {
    if (ed.et == ET_POINT) {
        point tmp;
        tmp.x = ed.params[0];
        tmp.y = ed.params[1];
        m_pointStorage.addElement(tmp);
        ++s_maxID.id;
        return s_maxID;
    }
    if (ed.et == ET_SECTION) {
        point tmp1;
        tmp1.x = ed.params[0];
        tmp1.y = ed.params[1];
        ++s_maxID.id;
        m_pointStorage.addElement(tmp1);
        point tmp2;
        tmp2.x = ed.params[2];
        tmp2.y = ed.params[3];
        ++s_maxID.id;
        m_pointStorage.addElement(tmp2);
        section tmp;
        tmp.beg = &tmp1;
        tmp.end = &tmp2;
        m_sectionStorage.addElement(tmp);
        ++s_maxID.id;
        return s_maxID;
    }
    if (ed.et == ET_CIRCLE) {
        point center;
        center.x = ed.params[0];
        center.y = ed.params[1];
        ++s_maxID.id;
        m_pointStorage.addElement(center);
        circle tmp;
        tmp.center = &center;
        tmp.R = ed.params[2];
        return s_maxID;
    }
    return ID{ -1 };
}

void Paint::paint() {
    for (auto point = m_pointStorage.begin(); point != m_pointStorage.end(); ++point) {
        filename.drawPoint(*point, false);
    }
    for (auto circle = m_circleStorage.begin(); circle != m_circleStorage.end(); ++circle) {
        filename.drawCircle(*circle, false);
    }
    for (auto section = m_sectionStorage.begin(); section != m_sectionStorage.end(); ++section) {
        filename.drawSection(*section, false);
    }
}

void Paint::saveToFile(const char* file) {
    std::ofstream fout;
    fout.open(file);
    if (!(fout.is_open())) {
        throw "We can't open file";
    }
    fout << m_pointStorage.getSize();
    for (int i = 0; i < s_maxID; ++i) {
        try {
            fout << m_pointStorage.findByKey(i)<<" ";
        }
        catch (...) {
            continue;
        }
    }
    fout << "\n";
    fout << m_sectionStorage.getSize();
    for (int i = 0; i < s_maxID; ++i) {
        try {
            fout << m_sectorStorage.findByKey(i) << " ";
        }
        catch (...) {
            continue;
        }
    }
    fout << "\n";
    fout << m_circleStorage.getSize();
    for (int i = 0; i < s_maxID; ++i) {
        try {
            fout << m_circleStorage.findByKey(i) << " ";
        }
        catch (...) {
            continue;
        }
    }
    fout.close()
}


void Paint::loadFromFile(const char* filename) {
    std::ifstream files;
    files.open(filename);
    if (!(files.is_open())) {
        throw "We can't open file";
    }
    m_pointIndex = Arry<idxPoint>(0);
    m_sectionIndex = Arry<idxSection>(0);
    m_circleIndex = Arry<idxCircle>(0);
    size_t size = 0;
    files >> size;
    point need;
    ID id;
    s_maxID = 0;
    idxPoint point_idx;
    List<point>::iterator point_iter;
    m_pointStorage = List<point>(0);
    for (size_t i = 0; i < size; ++i) {
        files >> id;
        if (id > s_maxID) {
            s_maxID = id;
        }
        files >> need;//нужно создать ввод для таких элементов
        point_iter =m_pointStorage.addElement(need);
        point_idx.id = id;
        point_idx.it = point_iter;
        m_pointIndex.addElement(point_idx);
    }
    files >> size;
    section work;
    idxSection section_idx;
    List<section>::iterator section_iter;
    m_sectionStorage = List<section>(0);
    ID beg_section;
    ID end_section;
    bool end_index_beg = false;
    bool end_index_end = false;
    for (size_t i = 0; i < size; ++i) {
        files >> id;
        if (id > s_maxID) {
            s_maxID = id;
        }
        files >> beg_section;
        files >> end_section;
        end_index_beg = false;
        end_index_end = false;
        for (size_t j = 0; j < m_pointIndex.getSize() && (!(end_index_beg) || !(end_index_end)); ++j) {
            if (m_pointIndex.getElement(j).id == beg_section) {
                work.beg = &(*m_pointIndex.getElement(j).it);
                end_index_beg = true;
            }
            if (m_pointIndex.getElement(j).id == end_section) {
                work.end = &(*m_pointIndex.getElement(j).it);
                end_index_end = true;
            }
        }
        section_iter = m_sectionStorage.addElement(work);
        section_idx.id = id;
        section_idx.it = section_iter;
        m_sectionIndex.addElement(section_idx);
    }
    files >> size;
    circle worker;
    idxCircle circle_idx;
    List<circle>::iterator circle_iter;
    m_circleStorage = List<circle>(0);
    ID center;
    bool end_index_center = false;
    for (size_t i = 0; i < size; ++i) {
        files >> id;
        if (id > s_maxID) {
            s_maxID = id;
        }
        files >> center;
        end_index_center = false;
        for (size_t j = 0; j < m_pointIndex.getSize() && !(end_index_center); ++j) {
            if (m_pointIndex.getElement(j).id == center) {
                worker.center = &(*m_pointIndex.getElement(j).it);
                end_index_center = true;
            }
        }
        files >> worker.R;
        circle_iter = m_circleStorage.addElement(worker);
        circle_idx.id = id;
        circle_idx.it = circle_iter;
        m_circleIndex.addElement(circle_idx);
    }
}

void Paint::exportToBMP(const char* file) {
    paint();
    try {
        filename.saveBMP(file);
    }
    catch (...) {
        throw std::invalid_argument("Can not opened file!");
    }
}

void Paint::changeBMP(const char* file) {
    filename = file;
}

// TODO: Implement this function to fulfill its intended functionality.
ElementData Paint::getElementInfo(ID id)
{
    return ElementData();
}

// TODO: Implement this function to fulfill its intended functionality.
void Paint::saveToFile(const char* filename)
{
    // ...
}
