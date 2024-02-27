#pragma once
#ifndef MRETOR
#define MRETOR
struct point{
  double x;
  double y;
};
struct section{
  point *beg;
  point *end;
};
struct circle{
  point center;
  double R;
};
#endif