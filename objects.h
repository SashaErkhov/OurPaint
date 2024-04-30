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
std::ofstream& operator<< (std::ofstream& out, ID& x);

struct rectangle {
        double x_1;
        double x_2;
        double y_1;
        double y_2;
        rectangle operator||(const rectangle& r);
};

struct point {
	double x;
	double y;
    rectangle rect();
};
std::ifstream& operator>> (std::ifstream& in, point& x);
struct section {
	point* beg;
	point* end;
    rectangle rect();
};

struct circle {
	point* center;
	double R;
    rectangle rect();
};

#endif
