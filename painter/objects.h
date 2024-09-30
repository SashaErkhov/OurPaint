#pragma once
#ifndef MRETOR
#define MRETOR

#include <fstream>
#include "enums.h"

struct ID {
    long long int id;

    ID(long long int i = 0) : id(i) {}

    ID &operator=(const ID &x);
};

bool operator>(const ID &left, const ID &right);

bool operator<(const ID &left, const ID &right);

bool operator==(const ID &left, const ID &right);

std::ifstream &operator>>(std::ifstream &in, ID &x);

struct rectangle {
    double x_1;
    double x_2;
    double y_1;
    double y_2;

    rectangle operator||(const rectangle &r);
};

struct primitive {
    virtual rectangle rect() = 0;
    virtual inline Element type() = 0;
};

struct point : public primitive {
    double x;
    double y;

    rectangle rect() override;
    inline Element type(){
        return ET_POINT;
    }
};

std::ifstream &operator>>(std::ifstream &in, point &x);

struct section : public primitive {
    point *beg;
    point *end;

    rectangle rect() override;
    inline Element type(){
        return ET_SECTION;
    }
};

struct circle : public primitive {
    point *center;
    double R;

    rectangle rect() override;
    inline Element type(){
        return ET_CIRCLE;
    }
};

#endif
