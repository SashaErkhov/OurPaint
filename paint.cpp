#include "paint.h"
#include "./BMPfile/code/BMPfile.h"
#include "./additions/Arry.h"
#include "./additions/List.h"
#include "objects.h"
#include <fstream>
ID Paint::addElement(const ElementData &ed) {
    if (ed.et == ET_POINT) {
        point tmp;
        tmp.x = ed.params[0];
        tmp.y = ed.params[1];
        m_pointStorage.addElement(tmp);
        ++maxID.id;
        return maxID;
    }
    if (ed.et == ET_SECTION) {
        point tmp1;
        tmp1.x = ed.params[0];
        tmp1.y = ed.params[1];
        ++maxID.id;
        m_pointStorage.addElement(tmp1);
        point tmp2;
        tmp2.x = ed.params[2];
        tmp2.y = ed.params[3];
        ++maxID.id;
        m_pointStorage.addElement(tmp2);
        section tmp;
        tmp.beg = &tmp1;
        tmp.end = &tmp2;
        m_sectionStorage.addElement(tmp);
        ++maxID.id;
        return maxID;
    }
    if (ed.et == ET_CIRCLE) {
        point center;
        center.x = ed.params[0];
        center.y = ed.params[1];
        ++maxID.id;
        m_pointStorage.addElement(center);
        circle tmp;
        tmp.center = &center;
        tmp.R = ed.params[2];
        return maxID;
    }
    return ID{-1};
}


// Реализация part2, часть qucksort
int Paint::part2(int left, int right) {
	idxPoint pivot = m_pointIndex[(left + right) / 2];
	while (left <= right) {
		while (m_pointIndex[left].id < pivot.id) left++;
		while (m_pointIndex[right].id > pivot.id) right--;
		if (left <= right) {
			std::swap(m_pointIndex[left], m_pointIndex[right]);
			left++;
			right--;
		}
	}
	return left;
}

// Реализация part1, часть qucksort
void Paint::part1(int start, int end) {
	if (start >= end) return;
	int rightStart = part2(start, end);
	part1(start, rightStart - 1);
	part1(rightStart, end);
}

// Реализация quickSort
void Paint::quickSort() {
	part1(0, m_pointIndex.size() - 1);
}
void Paint::paint(){
    for (auto point = m_pointStorage.begin(); point < m_pointStorage.end(); point++){
        filename.drawPoint(*point, false);
    }
    for (auto circle = m_circleStorage.begin(); circle < m_circleStorage.end(); circle++){
        filename.drawCircle(*circle, false);
    }
    for (auto section  = m_sectionStorage.begin(); section < m_sectionStorage.end(); section++){
        filename.drawSection(*section, false);
    }
}

void Paint::loadFromFile(const char* filen){
	std::ifstream file;
	file.open(filen);
	if (!(file.is_open())){
		throw "We can't open file";
	}
	file>>size;
	point need;
	for(size_t i=0; i<size; ++i){
		file>>need;//нужно создать ввод для таких элементов
		m_pointStorage.addElement(need);
	}
	file>>size;
	section work;
	for(size_t i=0; i<size; ++i){
		file>>work;//нужно создать ввод для таких элементов
		m_sectionStorage.addElement(work);
	}
	file>>size;
	circle worker;
	for(size_t i=0; i<size; ++i){
		file>>worker;//нужно создать ввод для таких элементов
		m_circleStorage.addElement(worker);
	}
}

//Element constract

ElementData::ElementData(point& pt){
	et=ET_POINT;
	params.addElement(pt.x);
	params.addElement(pt.y);
}

ElementData::ElementData(section& sec){
	et=ET_SECTION;
	params.addElement(sec.beg->x);
	params.addElement(sec.beg->y);
	params.addElement(sec.end->x);
	params.addElement(sec.end->y);
}

ElementData::ElementData(circle& cir){
	et=ET_CIRCLE;
	params.addElement(cir.center->x);
	params.addElement(cir.center->y);
	params.addElement(cir.R);
}

void Paint::exportToBMP(const char *file) {
    paint();
    try {
        filename.filename.saveBmp(file);
    } catch (...) {
        throw std::invalid_argument("Can not opened file!");
    }
}

void Paint::changeBMP(const char *file) {
    filename = BMPpainter(file);
}
