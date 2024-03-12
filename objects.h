#pragma once
#ifndef MRETOR
#define MRETOR

struct point{
  ID id;
  double x;
  double y;
};
struct section{
  ID id;
  point *beg;
  point *end;
};
struct circle{
  ID id;
  point *center;
  double R;
};
#endif
