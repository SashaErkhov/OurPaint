#pragma once
#ifndef MRETOR
#define MRETOR
struct ID{
  int id;
};
struct point{
  double x;
  double y;
  point(double xx=0, double yy=0): x(xx), y(yy){}
};
struct section{
  point *beg;
  point *end;
  section(point *b=nullptr, point *e=nullptr): beg(b), end(e){}
};
struct circle{
  point *center;
  double R;
  circle(point *c=nullptr, double r=0): center(c), R(r){}
};
#endif
