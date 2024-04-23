#pragma once
#ifndef MRETOR
#define MRETOR
#include <fstream>
struct ID {
	long long int id;
	ID(long long int i = 0) : id(i) {}
	ID& operator=(const ID& x);
};

bool operator>(const ID& left, const ID& right);
bool operator<(const ID& left, const ID& right);
bool operator==(const ID& left, const ID& right);

std::ifstream& operator>> (std::ifstream& in, ID& x);

struct point {
	double x;
	double y;
};

std::ifstream& operator>> (std::ifstream& in, point& x);

struct section {
	point* beg;
	point* end;
};

struct circle {
	point* center;
	double R;
};

#endif
