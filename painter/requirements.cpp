#include "requirements.h"

RequirementData::RequirementData() {
    objects = Arry<ID>();
    params = Arry<double>();
}



// 1
double ReqPointSecDist::getError() {
    double A = m_s->end->y - m_s->beg->y;
    double B = m_s->end->x - m_s->beg->x;
    double C = m_s->end->x * m_s->beg->y - m_s->beg->x * m_s->end->y;
    double e = std::abs(A * m_p->x - B * m_p->y + C) / sqrt(A * A + B * B) - d;
    return std::abs(e);
}
Arry<PARAMID> ReqPointSecDist::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_p->x));
    res.addElement(&(m_p->y));
    res.addElement(&(m_s->beg->x));
    res.addElement(&(m_s->beg->y));
    res.addElement(&(m_s->end->x));
    res.addElement(&(m_s->end->y));
    return res;
}
ReqPointSecDist::ReqPointSecDist(point* p, section* s, double dist) {
    m_p = p;
    m_s = s;
    d = dist;

}
double ReqPointSecDist::getDerivative(PARAMID param) {
    double x0 = m_p->x;
    double y0 = m_p->y;

    double x1 = m_s->beg->x;
    double y1 = m_s->beg->y;

    double x2 = m_s->end->x;
    double y2 = m_s->end->y;

    if (param == &m_p->x) { // x0
        double num = -y1 + y2;
        double den = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num / den;
    }
    else if (param == &m_p->y) { // y0
        double num = x1 - x2;
        double den = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num / den;
    }
    else if (param == &m_s->beg->x) { // x1
        double num1 = (-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = y0 - y2;
        double den2 = sqrt(((-x1 + x2), 2) + ((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->beg->y) { // y1
        double num1 = (-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = -x0 + x2;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->end->x) { // x2
        double num1 = -(-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = -y0 + y1;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->end->y) { // y2
        double num1 = -(-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = x0 - x1;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }

    return 0;
};



// 2
ReqPointOnSec::ReqPointOnSec(point* p, section* s) {
    m_p = p;
    m_s = s;
}
double ReqPointOnSec::getError() {
    double A = m_s->end->y - m_s->beg->y;
    double B = m_s->end->x - m_s->beg->x;
    double C = m_s->end->x * m_s->beg->y - m_s->beg->x * m_s->end->y;
    double e = std::abs(A * m_p->x - B * m_p->y + C) / sqrt(A * A + B * B);
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
        double den = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num / den;
    }
    else if (param == &m_p->y) { // y0
        double num = x1 - x2;
        double den = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num / den;
    }
    else if (param == &m_s->beg->x) { // x1
        double num1 = (-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = y0 - y2;
        double den2 = sqrt(((-x1 + x2), 2) + ((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->beg->y) { // y1
        double num1 = (-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = -x0 + x2;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->end->x) { // x2
        double num1 = -(-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = -y0 + y1;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (param == &m_s->end->y) { // y2
        double num1 = -(-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = x0 - x1;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }

    return 0;
}



// 3
ReqPointPointDist::ReqPointPointDist(point* p1, point* p2, double dist) {
    m_p1 = p1;
    m_p2 = p2;
    v_dist = dist;
}
double ReqPointPointDist::getError() {
    return std::abs(sqrt((m_p2->x - m_p1->x) * (m_p2->x - m_p1->x) + (m_p2->y - m_p1->y) * (m_p2->y - m_p1->y)) - v_dist);
}
Arry<PARAMID> ReqPointPointDist::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_p1->x));
    res.addElement(&(m_p1->y));
    res.addElement(&(m_p2->x));
    res.addElement(&(m_p2->y));
    return res;
}
double ReqPointPointDist::getDerivative(double* p) {
    double dx = m_p1->x - m_p2->x;
    double dy = m_p1->y - m_p2->y;
    double d = getError();
    if (p == &(m_p1->x)) {
        return dx / d;
    }
    else if (p == &(m_p1->y)) {
        return dy / d;
    }
    else if (p == &(m_p2->x)) {
        return -dx / d;
    }
    else if (p == &(m_p2->y)) {
        return -dy / d;
    }
    else {
        return 0;
    }
}



// 4
ReqPointOnPoint::ReqPointOnPoint(point* p1, point* p2) {
    m_p1 = p1;
    m_p2 = p2;
}
double ReqPointOnPoint::getError() {
    return sqrt((m_p2->x - m_p1->x) * (m_p2->x - m_p1->x) + (m_p2->y - m_p1->y) * (m_p2->y - m_p1->y));
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
    }
    else if (p == &(m_p1->y)) {
        return dy / d;
    }
    else if (p == &(m_p2->x)) {
        return -dx / d;
    }
    else if (p == &(m_p2->y)) {
        return -dy / d;
    }
    else {
        return 0;
    }
}



// 5
ReqSecCircleDist::ReqSecCircleDist(section* s, circle* c, double dist) {
    m_s = s;
    m_c = c;
    v_dist = dist;
}
double ReqSecCircleDist::getError() {
    double A = m_s->end->y - m_s->beg->y;
    double B = m_s->end->x - m_s->beg->x;
    double C = m_s->end->x * m_s->beg->y - m_s->beg->x * m_s->end->y;
    double e = std::abs(A * m_c->center->x - B * m_c->center->y + C) / sqrt(A * A + B * B) - (m_c->R + v_dist);
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
        double den = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num / den;
    }
    else if (p == &m_c->center->y) { // y0
        double num = x1 - x2;
        double den = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num / den;
    }
    else if (p == &m_s->beg->x) { // x1
        double num1 = (-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = y0 - y2;
        double den2 = sqrt(((-x1 + x2), 2) + ((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->beg->y) { // y1
        double num1 = (-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = -x0 + x2;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->end->x) { // x2
        double num1 = -(-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = -y0 + y1;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->end->y) { // y2
        double num1 = -(-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = x0 - x1;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }

    return 0;
}



// 6
ReqSecOnCircle::ReqSecOnCircle(section* s, circle* c) {
    m_s = s;
    m_c = c;
}
double ReqSecOnCircle::getError() {
    double A = m_s->end->y - m_s->beg->y;
    double B = m_s->end->x - m_s->beg->x;
    double C = m_s->end->x * m_s->beg->y - m_s->beg->x * m_s->end->y;
    double e = std::abs(A * m_c->center->x - B * m_c->center->y + C) / sqrt(A * A + B * B) - (m_c->R);
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
double ReqSecOnCircle::getDerivative(double* p) {
    double x0 = m_c->center->x;
    double y0 = m_c->center->y;

    double x1 = m_s->beg->x;
    double y1 = m_s->beg->y;

    double x2 = m_s->end->x;
    double y2 = m_s->end->y;

    if (p == &m_c->center->x) { // x0
        double num = -y1 + y2;
        double den = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num / den;
    }
    else if (p == &m_c->center->y) { // y0
        double num = x1 - x2;
        double den = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num / den;
    }
    else if (p == &m_s->beg->x) { // x1
        double num1 = (-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = y0 - y2;
        double den2 = sqrt(((-x1 + x2), 2) + ((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->beg->y) { // y1
        double num1 = (-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = -x0 + x2;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->end->x) { // x2
        double num1 = -(-x1 + x2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = -y0 + y1;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }
    else if (p == &m_s->end->y) { // y2
        double num1 = -(-y1 + y2) * (-((-x1 + x2) * y0) + x2 * y1 - x1 * y2 + x0 * (-y1 + y2));
        double den1 = sqrt(pow((pow((-x1 + x2), 2) + pow((-y1 + y2), 2)), 3));
        double num2 = x0 - x1;
        double den2 = sqrt(pow((-x1 + x2), 2) + pow((-y1 + y2), 2));

        return num1 / den1 + num2 / den2;
    }

    return 0;
}



// 7
ReqSecInCircle::ReqSecInCircle(section* s, circle* c) {
    m_s = s;
    m_c = c;
}
double ReqSecInCircle::getError() {
    double x0 = m_c->center->x;
    double y0 = m_c->center->y;

    double r = m_c->R - 1;

    double x1 = m_s->beg->x;
    double y1 = m_s->beg->y;

    double x2 = m_s->end->x;
    double y2 = m_s->end->y;

    double dist1 = sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2));
    double dist2 = sqrt(pow((x2 - x0), 2) + pow((y2 - y0), 2));

    return std::max(dist1 - r, dist2 - r);
}
Arry<PARAMID> ReqSecInCircle::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_s->beg->x));
    res.addElement(&(m_s->beg->y));
    res.addElement(&(m_s->end->x));
    res.addElement(&(m_s->end->y));
    return res;
}
double ReqSecInCircle::getDerivative(PARAMID p) {
    double x0 = m_c->center->x;
    double y0 = m_c->center->y;

    double r = m_c->R - 1;

    double x1 = m_s->beg->x;
    double y1 = m_s->beg->y;

    double x2 = m_s->end->x;
    double y2 = m_s->end->y;

    double dist1 = sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2));
    double dist2 = sqrt(pow((x2 - x0), 2) + pow((y2 - y0), 2));

    double grad_x1;
    double grad_y1;
    double grad_x2;
    double grad_y2;

    if (dist1 > r) {
        grad_x1 = (x1 - x0) / dist1;
        grad_y1 = (y1 - y0) / dist1;
    }
    else {
        grad_x1 = 0;
        grad_y1 = 0;
    }

    if (dist2 > r) {
        grad_x2 = (x2 - x0) / dist2;
        grad_y2 = (y2 - y0) / dist2;
    }
    else {
        grad_x2 = 0;
        grad_y2 = 0;
    }

    if (p == &m_s->beg->x) return grad_x1;
    else if (p == &m_s->beg->y) return grad_y1;
    else if (p == &m_s->end->x) return grad_x2;
    else if (p == &m_s->end->y) return grad_y2;

    return 0.0;
}



// 8
ReqSecSecParallel::ReqSecSecParallel(section* s1, section* s2) {
    m_s1 = s1;
    m_s2 = s2;
}
// ad-bc. Normal vectors (x2-x1, y2-y1) and (x4-x3, y4-y3)
double ReqSecSecParallel::getError() {
    double v1 = m_s1->end->x - m_s1->beg->x;
    double v2 = m_s1->end->y - m_s1->beg->y;
    double w1 = m_s2->end->x - m_s2->beg->x;
    double w2 = m_s2->end->y - m_s2->beg->y;
    return std::abs(v1 * w2 - v2 * w1);
}
Arry<PARAMID> ReqSecSecParallel::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_s1->beg->x));
    res.addElement(&(m_s1->beg->y));
    res.addElement(&(m_s1->end->x));
    res.addElement(&(m_s1->end->y));
    res.addElement(&(m_s2->beg->x));
    res.addElement(&(m_s2->beg->y));
    res.addElement(&(m_s2->end->x));
    res.addElement(&(m_s2->end->y));
    return res;
}
//  (x2-x1)*(y4-y3)-(y2-y1)*(x4-x3)
double ReqSecSecParallel::getDerivative(PARAMID p) {
    double x1 = m_s1->beg->x;
    double y1 = m_s1->beg->y;
    double x2 = m_s1->end->x;
    double y2 = m_s1->end->y;

    double x3 = m_s2->beg->x;
    double y3 = m_s2->beg->y;
    double x4 = m_s2->end->x;
    double y4 = m_s2->end->y;

    double v1 = x2 - x1;
    double v2 = y2 - y1;
    double w1 = x4 - x3;
    double w2 = y4 - y3;

    double sign = (v1 * w2 - w1 * v2 >= 0) ? 1 : -1;

    if (p == &(m_s1->beg->x)) return -w2 * sign;
    else if (p == &(m_s1->beg->y)) return w1 * sign;
    else if (p == &(m_s1->end->x)) return w2 * sign;
    else if (p == &(m_s1->end->y)) return -w1 * sign;
    else if (p == &(m_s2->beg->x)) return v2 * sign;
    else if (p == &(m_s2->beg->y)) return -v1 * sign;
    else if (p == &(m_s2->end->x)) return -v2 * sign;
    else if (p == &(m_s2->end->y)) return v1 * sign;

    return 0.0;
}



// 9
ReqSecSecPerpendicular::ReqSecSecPerpendicular(section* s1, section* s2) {
    m_s1 = s1;
    m_s2 = s2;
}
double ReqSecSecPerpendicular::getError() {
    double v1 = m_s1->end->x - m_s1->beg->x;
    double v2 = m_s1->end->y - m_s1->beg->y;
    double w1 = m_s2->end->x - m_s2->beg->x;
    double w2 = m_s2->end->y - m_s2->beg->y;
    return std::abs(v1 * w1 + v2 * w2);
}
Arry<PARAMID> ReqSecSecPerpendicular::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_s1->beg->x));
    res.addElement(&(m_s1->beg->y));
    res.addElement(&(m_s1->end->x));
    res.addElement(&(m_s1->end->y));
    res.addElement(&(m_s2->beg->x));
    res.addElement(&(m_s2->beg->y));
    res.addElement(&(m_s2->end->x));
    res.addElement(&(m_s2->end->y));
    return res;
}
double ReqSecSecPerpendicular::getDerivative(PARAMID p) {
    double x1 = m_s1->beg->x;
    double y1 = m_s1->beg->y;
    double x2 = m_s1->end->x;
    double y2 = m_s1->end->y;

    double x3 = m_s2->beg->x;
    double y3 = m_s2->beg->y;
    double x4 = m_s2->end->x;
    double y4 = m_s2->end->y;

    double v1 = x2 - x1;
    double v2 = y2 - y1;
    double w1 = x4 - x3;
    double w2 = y4 - y3;

    if (p == &(m_s1->beg->x)) return -w1;
    else if (p == &(m_s1->beg->y)) return -w2;
    else if (p == &(m_s1->end->x)) return w1;
    else if (p == &(m_s1->end->y)) return w2;
    else if (p == &(m_s2->beg->x)) return -v1;
    else if (p == &(m_s2->beg->y)) return -v2;
    else if (p == &(m_s2->end->x)) return v1;
    else if (p == &(m_s2->end->y)) return v2;

    return 0.0;
}



// 10
ReqSecSecAngel::ReqSecSecAngel(section* s1, section* s2, double angle) {
    m_s1 = s1;
    m_s2 = s2;
    desired_angle = angle;
}
double ReqSecSecAngel::getError() {
    double v1 = m_s1->end->x - m_s1->beg->x;
    double v2 = m_s1->end->y - m_s1->beg->y;
    double w1 = m_s2->end->x - m_s2->beg->x;
    double w2 = m_s2->end->y - m_s2->beg->y;

    double dot_product = v1 * w1 + v2 * w2;
    double mag_v = sqrt(v1 * v1 + v2 * v2);
    double mag_w = sqrt(w1 * w1 + w2 * w2);
    double cos_angle = dot_product / (mag_v * mag_w);
    double angle = (acos(cos_angle) / 3.141592) * 180;

    return std::abs(angle - desired_angle);
}
Arry<PARAMID> ReqSecSecAngel::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_s1->beg->x));
    res.addElement(&(m_s1->beg->y));
    res.addElement(&(m_s1->end->x));
    res.addElement(&(m_s1->end->y));
    res.addElement(&(m_s2->beg->x));
    res.addElement(&(m_s2->beg->y));
    res.addElement(&(m_s2->end->x));
    res.addElement(&(m_s2->end->y));
    return res;
}
double ReqSecSecAngel::getDerivative(PARAMID p) {
    double x1 = m_s1->beg->x;
    double y1 = m_s1->beg->y;
    double x2 = m_s1->end->x;
    double y2 = m_s1->end->y;

    double x3 = m_s2->beg->x;
    double y3 = m_s2->beg->y;
    double x4 = m_s2->end->x;
    double y4 = m_s2->end->y;

    if (p == &(m_s1->beg->x)) {
        return -(((x3 - x4) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2))) +
            ((-x1 + x2) * ((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4))) /
            (pow(pow(-x1 + x2, 2) + pow(-y1 + y2, 2), 1.5) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))) /
            sqrt(1 - pow(((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4)), 2) /
                ((pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * (pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))));
    }
    else if (p == &(m_s1->beg->y)) {
        return -(((y3 - y4) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2))) +
            ((-y1 + y2) * ((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4))) /
            (pow(pow(-x1 + x2, 2) + pow(-y1 + y2, 2), 1.5) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))) /
            sqrt(1 - pow(((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4)), 2) /
                ((pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * (pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))));
    }
    else if (p == &(m_s1->end->x)) {
        return -(((-x3 + x4) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2))) -
            ((-x1 + x2) * ((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4))) /
            (pow(pow(-x1 + x2, 2) + pow(-y1 + y2, 2), 1.5) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))) /
            sqrt(1 - pow(((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4)), 2) /
                ((pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * (pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))));
    }
    else if (p == &(m_s1->end->y)) {
        return -(((-y3 + y4) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2))) -
            ((-y1 + y2) * ((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4))) /
            (pow(pow(-x1 + x2, 2) + pow(-y1 + y2, 2), 1.5) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))) /
            sqrt(1 - pow(((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4)), 2) /
                ((pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * (pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))));
    }
    else if (p == &(m_s2->beg->x)) {
        return -((((-x3 + x4) * ((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4))) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * pow(pow(-x3 + x4, 2) + pow(-y3 + y4, 2), 1.5)) +
            (x1 - x2) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))) /
            sqrt(1 - pow(((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4)), 2) /
                ((pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * (pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))));
    }
    else if (p == &(m_s2->beg->y)) {
        return -((((-y3 + y4) * ((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4))) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * pow(pow(-x3 + x4, 2) + pow(-y3 + y4, 2), 1.5)) +
            (y1 - y2) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))) /
            sqrt(1 - pow(((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4)), 2) /
                ((pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * (pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))));
    }
    else if (p == &(m_s2->end->x)) {
        return -((-(((-x3 + x4) * ((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4))) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * pow(pow(-x3 + x4, 2) + pow(-y3 + y4, 2), 1.5))) +
            (-x1 + x2) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))) /
            sqrt(1 - pow(((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4)), 2) /
                ((pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * (pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))));
    }
    else if (p == &(m_s2->end->y)) {
        return -((-(((-y3 + y4) * ((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4))) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * pow(pow(-x3 + x4, 2) + pow(-y3 + y4, 2), 1.5))) +
            (-y1 + y2) /
            (sqrt(pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * sqrt(pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))) /
            sqrt(1 - pow(((-x1 + x2) * (-x3 + x4) + (-y1 + y2) * (-y3 + y4)), 2) /
                ((pow(-x1 + x2, 2) + pow(-y1 + y2, 2)) * (pow(-x3 + x4, 2) + pow(-y3 + y4, 2)))));
    }

    return 0.0;
}
