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

rectangle ReqPointSecDist::getRectangle() {
    return m_s->rect() || m_p->rect();
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

rectangle ReqPointOnSec::getRectangle() {
    return m_s->rect() || m_p ->rect();
}


// 3
ReqPointPointDist::ReqPointPointDist(point* p1, point* p2, double dist) {
    m_p1 = p1;
    m_p2 = p2;
    v_dist = dist;
}
double ReqPointPointDist::getError() {
    return std::abs(hypot(m_p1->x - m_p2->x, m_p1->y - m_p2->y) - v_dist);
}
Arry<PARAMID> ReqPointPointDist::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_p1->x));
    res.addElement(&(m_p1->y));
    res.addElement(&(m_p2->x));
    res.addElement(&(m_p2->y));
    return res;
}

rectangle ReqPointPointDist::getRectangle() {
    return m_p1->rect() || m_p2->rect();
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

rectangle ReqPointOnPoint::getRectangle() {
    return m_p2->rect() || m_p1->rect();
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
    res.addElement(&(m_c->R));
    res.addElement(&(m_s->beg->x));
    res.addElement(&(m_s->beg->y));
    res.addElement(&(m_s->end->x));
    res.addElement(&(m_s->end->y));
    return res;
}

rectangle ReqSecCircleDist::getRectangle() {
    return m_s->rect() || m_c->rect();
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
    res.addElement(&(m_c->R));
    res.addElement(&(m_s->beg->x));
    res.addElement(&(m_s->beg->y));
    res.addElement(&(m_s->end->x));
    res.addElement(&(m_s->end->y));
    return res;
}

rectangle ReqSecOnCircle::getRectangle() {
    return m_s->rect() || m_c->rect();
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

    double dist1 = std::hypot(x1 - x0, y1 - y0);
    double dist2 = std::hypot(x2 - x0, y2 - y0);

    return std::max(dist1 - r, dist2 - r);
}
Arry<PARAMID> ReqSecInCircle::getParams() {
    Arry<PARAMID> res;
    res.addElement(&(m_c->center->x));
    res.addElement(&(m_c->center->y));
    res.addElement(&(m_c->R));
    res.addElement(&(m_s->beg->x));
    res.addElement(&(m_s->beg->y));
    res.addElement(&(m_s->end->x));
    res.addElement(&(m_s->end->y));
    return res;
}

rectangle ReqSecInCircle::getRectangle() {
    return m_s->rect() || m_c->rect();
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

rectangle ReqSecSecParallel::getRectangle() {
    return m_s1->rect() || m_s2->rect();
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

rectangle ReqSecSecPerpendicular::getRectangle() {
    return m_s1->rect() || m_s2->rect();
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
    double mag_v = std::hypot(v1, v2);
    double mag_w = std::hypot(w1, w2);
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

rectangle ReqSecSecAngel::getRectangle() {
    return m_s1->rect() || m_s2->rect();
}
