#include "objects.h"
#include <fstream>

ID& ID::operator=(const ID& x) {
    if (this != &x) {
        id = x.id;
    }
    return *this;
}

bool operator>(const ID& left, const ID& right) {
    if (left.id < right.id) {
        return false;
    }
    return true;
}

bool operator<(const ID& left, const ID& right) {
    if (left.id < right.id) {
        return true;
    }
    return false;
}
bool operator==(const ID& left, const ID& right) {
    if (left.id == right.id) {
        return true;
    }
    return false;
}

std::ifstream& operator>> (std::ifstream& in, ID& x) {
    in >> x.id;
    return in;
}

std::ifstream& operator>> (std::ifstream& in, point& x) {
    in >> x.x;
    in >> x.y;
    return in;
}

rectangle point::rect() {
    rectangle r = {x, x, y, y};
    return r;
}
rectangle circle::rect() {
    rectangle r = {center->x - R, center->x + R,center->y - R, center->y + R};
    return r;
}
rectangle section::rect() {
    rectangle r = {beg->x,end->x, beg->y, end->y};
    return r;
}

rectangle rectangle::operator||(const rectangle &r) {
    rectangle r1 = {
        std::min(std::min(x_1, r.x_1),std::min(x_2, r.x_2)),
        std::max(std::max(x_1, r.x_1),std::max(x_2, r.x_2)),
        std::min(std::min(y_1, r.y_1),std::min(y_2, r.y_2)),
        std::max(std::max(y_1, r.y_1),std::max(y_2, r.y_2))
    };
    return r1;
}
