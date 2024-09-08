#include <gtest/gtest.h>
#include "../painter/paint.h"


TEST(PaintTest, PointSecDist) {
    Paint screen;
    ElementData data1;
    data1.et = ET_POINT;
    data1.params[0] = 10;
    data1.params[1] = 10;
    ElementData data_s;
    data_s.et = ET_SECTION;
    data_s.params[0] = -10;
    data_s.params[1] = 20;
    data_s.params[2] = 20;
    data_s.params[3] = 30;
    screen.addElement(data1);
    screen.addElement(data_s);
    RequirementData req;
    req.req = ET_POINTSECTIONDIST;
    req.objects[0] = ID(1);
    req.objects[1] = ID(4);
    req.params[0] = 20;
    screen.addRequirement(req);
    double xp = screen.getElementInfo(ID(1)).params[0];
    double yp = screen.getElementInfo(ID(1)).params[1];
    double xo_s = screen.getElementInfo(ID(4)).params[0];
    double yo_s = screen.getElementInfo(ID(4)).params[1];
    double xs_s = screen.getElementInfo(ID(4)).params[2];
    double ys_s = screen.getElementInfo(ID(4)).params[3];
    EXPECT_EQ(abs((yo_s - ys_s)*xp - (xo_s - xs_s)*yp +xo_s*ys_s - yo_s*xs_s)/sqrt((xo_s-xs_s)*(xo_s-xs_s) +(yo_s-ys_s)*(yo_s-ys_s)), 20);
}
TEST(PaintTest, DoublePointSecDist) {
Paint screen;
ElementData data1;
data1.et = ET_POINT;
data1.params[0] = 10;
data1.params[1] = 10;
ElementData data1;
data1.et = ET_POINT;
data1.params[0] = 0;
data1.params[1] = 20;
ElementData data_s;
data_s.et = ET_SECTION;
data_s.params[0] = -10;
data_s.params[1] = 20;
data_s.params[2] = 20;
data_s.params[3] = 30;
screen.addElement(data1);
screen.addElement(data_s);
RequirementData req1;
req1.req = ET_POINTSECTIONDIST;
req1.objects[0] = ID(1);
req1.objects[1] = ID(5);
req1.params[0] = 20;
screen.addRequirement(req1);
RequirementData req2;
req2.req = ET_POINTSECTIONDIST;
req2.objects[0] = ID(2);
req2.objects[1] = ID(5);
req2.params[0] = 20;
screen.addRequirement(req1);
double xp = screen.getElementInfo(ID(1)).params[0];
double yp = screen.getElementInfo(ID(1)).params[1];
double xo_s = screen.getElementInfo(ID(4)).params[0];
double yo_s = screen.getElementInfo(ID(4)).params[1];
double xs_s = screen.getElementInfo(ID(4)).params[2];
double ys_s = screen.getElementInfo(ID(4)).params[3];
EXPECT_EQ(abs((yo_s - ys_s)*xp - (xo_s - xs_s)*yp +xo_s*ys_s - yo_s*xs_s)/sqrt((xo_s-xs_s)*(xo_s-xs_s) +(yo_s-ys_s)*(yo_s-ys_s)), 20);
}