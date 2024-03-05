#include "Array.h"
#include "BMPfile.h"
#include "objects.h"
#include "paint.h"

int main()
{
	BMPfile first(40, 40);
	first.saveBmp("bmp.bmp");
	return 0;
}