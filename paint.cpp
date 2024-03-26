#include "paint.h"
#include "./PVM13.02.24/code/BMPfile.h"
#include "./additions/Arry.h"
#include "objects.h"

ID Paint::addElement(const ElementData& ed)
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
