//
// Created by Ardrass on 010, 10.07.2024.
//

#include "requirements.h"

RequirementData::RequirementData(){
    objects = Arry<ID>();
    params = Arry<double>();
}
double ReqPointSegDist::getError() {
    double A = m_s->end->y -m_s->beg->y;
    double B = m_s->end->x -m_s->beg->x;
    double C = m_s->end->x * m_s->beg->y - m_s->beg->x * m_s->end->y;
    double e = std::abs(A * m_p->x - B * m_p->y + C) / sqrt(A*A+B*B) - d;
    return std::abs(e);
}

Arry<PARAMID> ReqPointSegDist::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_p->x));
    res.addElement(&(m_p->y));
    res.addElement(&(m_s->beg->x));
    res.addElement(&(m_s->beg->y));
    res.addElement(&(m_s->end->x));
    res.addElement(&(m_s->end->y));
    return res;
}

ReqPointSegDist::ReqPointSegDist(point *p, section *s, double dist) {
    m_p = p;
    m_s = s;
    d = dist;

}

double ReqPointSegDist::getDerivative(PARAMID param) {
    double x0 = m_p->x;
    double y0 = m_p->y;

    double x1 = m_s->beg->x;
    double y1 = m_s->beg->y;

    double x2 = m_s->end->x;
    double y2 = m_s->end->y;

    if (param == &m_p->x) { // x0
        double num = -y1 + y2;
        double den = std::hypot(-x1+x2,-y1+y2);
        return num / den;
    }
    else if (param == &m_p->y) { // y0
        double num = x1 - x2;
        double den = std::hypot(-x1+x2,-y1+y2);

        return num / den;
    }
    else if (param == &m_s->beg->x) { // x1
        double num1 = (-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = y0 - y2;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->beg->y) { // y1
        double num1 = (-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = -x0 + x2;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->end->x) { // x2
        double num1 = -(-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = -y0 + y1;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->end->y) { // y2
        double num1 = -(-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = x0 - x1;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }

    return 0;
};

ReqPointOnPoint::ReqPointOnPoint(point* p1, point* p2){
    m_p1 = p1;
    m_p2 = p2;
}
double ReqPointOnPoint::getError(){
    return sqrt((m_p2->x-m_p1->x)*(m_p2->x-m_p1->x) + (m_p2->y-m_p1->y)*(m_p2->y-m_p1->y));
}
Arry<PARAMID> ReqPointOnPoint::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_p1->x));
    res.addElement(&(m_p1->y));
    res.addElement(&(m_p2->x));
    res.addElement(&(m_p2->y));
    return res;
}
double ReqPointOnPoint::getDerivative(PARAMID p) {
    double dx = m_p1->x - m_p2->x;
    double dy = m_p1->y - m_p2->y;
    double d = getError();
    if (p == &(m_p1->x)) {
        return dx / d;
    } else if (p == &(m_p1->y)) {
        return dy / d;
    } else if (p == &(m_p2->x)) {
        return -dx / d;
    } else if (p == &(m_p2->y)) {
        return -dy / d;
    } else {
        return 0;
    }
}

ReqPointPointDist::ReqPointPointDist(point *p1, point *p2, double dist) {
    m_p1 = p1;
    m_p2 = p2;
    v_dist = dist;
}

double ReqPointPointDist::getError() {
    return std::abs(sqrt((m_p2->x-m_p1->x)*(m_p2->x-m_p1->x) + (m_p2->y-m_p1->y)*(m_p2->y-m_p1->y)) - v_dist);
}

Arry<PARAMID> ReqPointPointDist::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_p1->x));
    res.addElement(&(m_p1->y));
    res.addElement(&(m_p2->x));
    res.addElement(&(m_p2->y));
    return res;
}

double ReqPointPointDist::getDerivative(double *p) {
    double dx = m_p1->x - m_p2->x;
    double dy = m_p1->y - m_p2->y;
    double d = getError();
    if (p == &(m_p1->x)) {
        return dx / d;
    } else if (p == &(m_p1->y)) {
        return dy / d;
    } else if (p == &(m_p2->x)) {
        return -dx / d;
    } else if (p == &(m_p2->y)) {
        return -dy / d;
    } else {
        return 0;
    }
}

ReqPointOnSec::ReqPointOnSec(point *p, section *s) {
    m_p = p;
    m_s = s;
}

double ReqPointOnSec::getError() {
    double A = m_s->end->y - m_s->beg->y;
    double B = m_s->end->x - m_s->beg->x;
    double C = m_s->end->x * m_s->beg->y - m_s->beg->x * m_s->end->y;
    double e = std::abs(A * m_p->x - B * m_p->y + C) / sqrt(A*A+B*B);
    return e;
}

Arry<PARAMID> ReqPointOnSec::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_p->x));
    res.addElement(&(m_p->y));
    res.addElement(&(m_s->beg->x));
    res.addElement(&(m_s->beg->y));
    res.addElement(&(m_s->end->x));
    res.addElement(&(m_s->end->y));
    return res;
}

double ReqPointOnSec::getDerivative(PARAMID param) {
    double x0 = m_p->x;
    double y0 = m_p->y;

    double x1 = m_s->beg->x;
    double y1 = m_s->beg->y;

    double x2 = m_s->end->x;
    double y2 = m_s->end->y;

    if (param == &m_p->x) { // x0
        double num = -y1 + y2;
        double den = std::hypot(-x1+x2,-y1+y2);
        return num / den;
    }
    else if (param == &m_p->y) { // y0
        double num = x1 - x2;
        double den = std::hypot(-x1+x2,-y1+y2);

        return num / den;
    }
    else if (param == &m_s->beg->x) { // x1
        double num1 = (-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = y0 - y2;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->beg->y) { // y1
        double num1 = (-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = -x0 + x2;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->end->x) { // x2
        double num1 = -(-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = -y0 + y1;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->end->y) { // y2
        double num1 = -(-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = x0 - x1;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }

    return 0;}

ReqSecCircleDist::ReqSecCircleDist(section *s, circle *c, double dist) {
    m_s = s;
    m_c = c;
    v_dist = dist;
}
double ReqSecCircleDist::getError() {
    double A = m_s->end->y - m_s->beg->y;
    double B = m_s->end->x - m_s->beg->x;
    double C = m_s->end->x * m_s->beg->y - m_s->beg->x * m_s->end->y;
    double e = std::abs(A * m_c->center->x - B * m_c->center->y + C) / sqrt(A*A+B*B) - (m_c->R + v_dist);
    return std::abs(e);
}

Arry<PARAMID> ReqSecCircleDist::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_c->center->x));
    res.addElement(&(m_c->center->y));
    res.addElement(&(m_s->beg->x));
    res.addElement(&(m_s->beg->y));
    res.addElement(&(m_s->end->x));
    res.addElement(&(m_s->end->y));
    return res;
}

double ReqSecCircleDist::getDerivative(PARAMID p) {
    double x0 = m_c->center->x;
    double y0 = m_c->center->y;

    double x1 = m_s->beg->x;
    double y1 = m_s->beg->y;

    double x2 = m_s->end->x;
    double y2 = m_s->end->y;

    if (p == &m_c->center->x) { // x0
        double num = -y1 + y2;
        double den = std::hypot(-x1+x2,-y1+y2);
        return num / den;
    }
    else if (p == &m_c->center->y) { // y0
        double num = x1 - x2;
        double den = std::hypot(-x1+x2,-y1+y2);

        return num / den;
    }
    else if (p == &m_s->beg->x) { // x1
        double num1 = (-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = y0 - y2;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->beg->y) { // y1
        double num1 = (-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = -x0 + x2;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->end->x) { // x2
        double num1 = -(-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = -y0 + y1;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->end->y) { // y2
        double num1 = -(-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = x0 - x1;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }

    return 0;
}


ReqSecOnCircle::ReqSecOnCircle(section *s, circle *c) {
    m_s = s;
    m_c = c;
}

double ReqSecOnCircle::getError() {
    double A = m_s->end->y - m_s->beg->y;
    double B = m_s->end->x - m_s->beg->x;
    double C = m_s->end->x * m_s->beg->y - m_s->beg->x * m_s->end->y;
    double e = std::abs(A * m_c->center->x - B * m_c->center->y + C) / sqrt(A*A+B*B) - (m_c->R);
    return std::abs(e);
}

Arry<PARAMID> ReqSecOnCircle::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_c->center->x));
    res.addElement(&(m_c->center->y));
    res.addElement(&(m_s->beg->x));
    res.addElement(&(m_s->beg->y));
    res.addElement(&(m_s->end->x));
    res.addElement(&(m_s->end->y));
    return res;
}

double ReqSecOnCircle::getDerivative(double *p) {
    double x0 = m_c->center->x;
    double y0 = m_c->center->y;

    double x1 = m_s->beg->x;
    double y1 = m_s->beg->y;

    double x2 = m_s->end->x;
    double y2 = m_s->end->y;


    if (p == &m_c->center->x) { // x0
        double num = -y1 + y2;
        double den = std::hypot(-x1+x2,-y1+y2);
        return num / den;
    }
    else if (p == &m_c->center->y) { // y0
        double num = x1 - x2;
        double den = std::hypot(-x1+x2,-y1+y2);

        return num / den;
    }
    else if (p == &m_s->beg->x) { // x1
        double num1 = (-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = y0 - y2;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->beg->y) { // y1
        double num1 = (-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = -x0 + x2;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->end->x) { // x2
        double num1 = -(-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = -y0 + y1;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->end->y) { // y2
        double num1 = -(-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = pow(std::hypot(-x1+x2,-y1+y2), 3);
        double num2 = x0 - x1;
        double den2 = std::hypot(-x1+x2,-y1+y2);

        return num1 / den1 + num2 / den2;
    }

    return 0;
}
