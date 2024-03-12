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