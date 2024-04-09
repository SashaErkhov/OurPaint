#pragma once
#ifndef MRETOR
#define MRETOR
struct ID{
  int id;
  ID(int i=0): id(i){}
  ID& operator=(const ID& x);
};

bool operator>(const ID& left, const ID& right);

struct point{
  double x;
  double y;
};
struct section{
  point *beg;
  point *end;
};
struct circle{
  point *center;
  double R;
};
#endif
