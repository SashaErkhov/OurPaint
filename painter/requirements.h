//
// Created by Ardrass on 010, 10.07.2024.
//

#ifndef OURPAINT_REQUIREMENTS_H
#define OURPAINT_REQUIREMENTS_H

#define PARAMID double*
#include "Arry.h"
#include "objects.h"
#include <cmath>
#include "enums.h"
#define h 0.000001

struct RequirementData {
    Requirement req;
    Arry<ID> objects;
    Arry<double> params;
    RequirementData();
};

// Abstract class
struct IReq {
protected:
    Arry<ID> objects;
    Requirement req;
public:
    virtual double getError() = 0;
    virtual Arry<PARAMID> getParams() = 0;
    virtual rectangle getRectangle() = 0;
    virtual double getDerivative(PARAMID p) {
        double origValue = *p;
        *p += h;
        double f1 = getError();
        *p -= 2 * h;
        double f2 = getError();
        *p = origValue;
        return (f1 - f2) / (2 * h);
    }

    virtual double getSecondDerivative(PARAMID p) {
        double origValue = *p;
        *p += h;
        double f1 = getError();
        *p = origValue;
        double f0 = getError();
        *p -= h;
        double f2 = getError();
        *p = origValue;
        return (f1 - 2 * f0 + f2) / (h * h);
    }

    virtual double getMixedDerivative(PARAMID p1, PARAMID p2) {
        double origValue1 = *p1;
        double origValue2 = *p2;
        *p1 += h;
        *p2 += h;
        double f1 = getError();
        *p2 -= 2 * h;
        double f2 = getError();
        *p1 -= 2 * h;
        *p2 += 2 * h;
        double f3 = getError();
        *p2 -= 2 * h;
        double f4 = getError();
        *p1 = origValue1;
        *p2 = origValue2;
        return (f1 - f2 - f3 + f4) / (4 * h * h);
    }
};



// 1
class ReqPointSecDist : public IReq {
    point* m_p;
    section* m_s;
    double d;
public:
    ReqPointSecDist(point* p, section* s, double dist);
    double getError() override;
    Arry<PARAMID> getParams() override;
    rectangle getRectangle() override;
};

// 2
class ReqPointOnSec : public IReq {
    point* m_p;
    section* m_s;
public:
    ReqPointOnSec(point* p, section* s);
    double getError() override;
    Arry<PARAMID> getParams() override;
    rectangle getRectangle() override;
};

// 3
class ReqPointPointDist : public IReq {
    point* m_p1;
    point* m_p2;
    double v_dist;
public:
    ReqPointPointDist(point* p1, point* p2, double dist);
    double getError() override;
    Arry<PARAMID> getParams() override;
    rectangle getRectangle() override;
};

// 4
class ReqPointOnPoint : public IReq {
    point* m_p1;
    point* m_p2;
public:
    ReqPointOnPoint(point* p1, point* p2);
    double getError() override;
    Arry<PARAMID> getParams() override;
    rectangle getRectangle() override;
};

// 5
class ReqSecCircleDist : public IReq {
    section* m_s;
    circle* m_c;
    double v_dist;
public:
    ReqSecCircleDist(section* m_s, circle* m_c, double dist);
    double getError() override;
    Arry<PARAMID> getParams() override;
    rectangle getRectangle() override;
};

// 6
class ReqSecOnCircle : public IReq {
    section* m_s;
    circle* m_c;
public:
    ReqSecOnCircle(section* m_s, circle* m_c);
    double getError() override;
    Arry<PARAMID> getParams() override;
    rectangle getRectangle() override;
};

// 7
class ReqSecInCircle : public IReq {
    section* m_s;
    circle* m_c;
public:
    ReqSecInCircle(section* m_s, circle* m_c);
    double getError() override;
    Arry<PARAMID> getParams() override;
    rectangle getRectangle() override;
};

// 8
class ReqSecSecParallel : public IReq {
    section* m_s1;
    section* m_s2;
public:
    ReqSecSecParallel(section* m_s1, section* m_s2);
    double getError() override;
    Arry<PARAMID> getParams() override;
    rectangle getRectangle() override;
};

// 9
class ReqSecSecPerpendicular : public IReq {
    section* m_s1;
    section* m_s2;
public:
    ReqSecSecPerpendicular(section* m_s1, section* m_s2);
    double getError() override;
    Arry<PARAMID> getParams() override;
    rectangle getRectangle() override;
};

// 10
class ReqSecSecAngel : public IReq {
    section* m_s1;
    section* m_s2;
    double desired_angle;
public:
    ReqSecSecAngel(section* m_s1, section* m_s2, double desired_dist);
    double getError() override;
    Arry<PARAMID> getParams() override;
    rectangle getRectangle() override;
};


#endif //OURPAINT_REQUIREMENTS_H
