#include "paint.h"
#include "./BMPfile/code/BMPfile.h"
#include "./additions/Arry.h"
#include "./additions/List.h"
#include "objects.h"
#include <fstream>

/*ID Paint::addElement(const ElementData& ed)
{
	if (ed.et == ET_POINT)
	{
		point tmp;
		tmp.x = ed.params.getElement(0);
		tmp.y = ed.params.getElement(1);
		tmp.id;//TODO
		pointar.addElement(tmp);
		return tmp.id;
	}
	if (ed.et == ET_SECTION)
	{
		point tmp1;
		tmp1.x = ed.params.getElement(0);
		tmp1.y = ed.params.getElement(1);
		tmp1.id;//TODO
		pointar.addElement(tmp1);
		point tmp2;
		tmp2.x = ed.params.getElement(2);
		tmp2.y = ed.params.getElement(3);
		tmp2.id;//TODO
		pointar.addElement(tmp2);
		section tmp;
		//tmp.beg=&
		//tmp.end=&
		tmp.id;//TODO



		return tmp.id;
	}
	if (ed.et == ET_CIRCLE)
	{

	}
<<<<<<< HEAD
}*/

void Paint::drawSection(section s, bool isWhite){
    //Алгоритм Брезенхема
    int x0 = s.beg->x;
    int y0 = s.beg->y;
    int x1 = s.end->x;
    int y1 = s.end->y;
    int deltaX = abs(x1 - x0);
    int dirX = x0 < x1 ? 1 : -1;
    int deltaY = -abs(y1 - y0);
    int dirY = y0 < y1 ? 1 : -1;
    int err = deltaX + deltaY;
    int e2;
    for (;;) {
        filename.setPixel(x0, y0, isWhite);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 <= deltaX) { err += deltaX; y0 += dirY; } /* e_xy+e_y < 0 */
        if (e2 >= deltaY) { err += deltaY; x0 += dirX; } /* e_xy+e_x > 0 */
    }
}

void Paint::drawPoint(point p, bool isWhite){
    filename.setPixel(p.x, p.y, isWhite);
}
void Paint::drawCircle(circle c, bool isWhite){
    int x = 0;
    int y = static_cast<int>(c.R);
    int delta = 1 - 2 * y;
    int error = 0;
    while (y >= x) {
        filename.setPixel(c.center->x + x, c.center->y + y, isWhite);
        filename.setPixel(c.center->x + x, c.center->y - y, isWhite);
        filename.setPixel(c.center->x - x, c.center->y + y, isWhite);
        filename.setPixel(c.center->x - x, c.center->y - y, isWhite);
        filename.setPixel(c.center->x + y, c.center->y + x, isWhite);
        filename.setPixel(c.center->x + y, c.center->y - x, isWhite);
        filename.setPixel(c.center->x - y, c.center->y + x, isWhite);
        filename.setPixel(c.center->x - y, c.center->y - x, isWhite);
        error = (delta + y) * 2 - 1;
        if ((delta < 0) && (error <= 0)) {
            delta += 2 * ++x + 1;
            continue;
        }
        if ((delta > 0) && (error > 0)) {
            delta -= 2 * --y + 1;
            continue;
        }
        delta += 2 * (++x - --y);
    }


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
        drawPoint(*point, false);
    }
    for (auto circle = m_circleStorage.begin(); circle < m_circleStorage.end(); circle++){
        drawCircle(*circle, false);
    }
    for (auto section  = m_sectionStorage.begin(); section < m_sectionStorage.end(); section++){
        drawSection(*section, false);
    }
}

void Paint::loadFromFile(const char* filen){
	std::ifstream file;
	file.open(filen);
	if (!(file.is_open())){
		throw "We can't open file";
	}
	size_t size=0;
	size_t count=0;
	file>>size;
	file>>count;
	idxPoint input;
	for(size_t i=0; i<count;++i){//проверка на размер size и count нужна
		file>>input;//нужно создать ввод для таких элементов
		m_pointIndex.addElement(input);
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