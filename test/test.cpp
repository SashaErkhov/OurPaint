#include <gtest/gtest.h>
#include "../painter/paint.h"

TEST(PaintTest, AddingElems){
  Paint paint;
  // Заполним параметры добавляемого элемента
  ElementData ed;
  ed.et = ET_POINT;
  ed.params.addElement(1.0);
  ed.params.addElement(2.0);
  // Добавим элемент
  ID pointid = paint.addElement(ed);
  // Проверка???  
  ElementData e = paint.getElementInfo(pointid);
  ASSERT_EQ(ed.et, ET_POINT);
  ASSERT_EQ(ed.params.getElement(0),1.0);
  ASSERT_EQ(ed.params.getElement(1),2.0); 
}
/*
TEST(PaintTest, save_and_load){
  Paint paint;
  ElementData in;
  in.et=ET_POINT;
  in.params.addElement(3.0);
  in.params.addElement(2.0);
  paint.addElement(in);
  in=ElementData();
  in.et=ET_POINT;
  in.params.addElement(8.0);
  in.params.addElement(7.0);
  paint.addElement(in);
  in=ElementData();
  in.et=ET_SECTION;
  in.params.addElement(87.0);
  in.params.addElement(77.0);
  in.params.addElement(78.0);
  in.params.addElement(87.0);
  paint.addElement(in);
  in=ElementData();
  in.et=ET_SECTION;
  in.params.addElement(57.0);
  in.params.addElement(75.0);
  in.params.addElement(9.0);
  in.params.addElement(1.0);
  paint.addElement(in);
  in=ElementData();
  in.et=ET_CIRCLE;
  in.params.addElement(83.0);
  in.params.addElement(37.0);
  in.params.addElement(38.0);
  paint.addElement(in);
  in=ElementData();
  in.et=ET_CIRCLE;
  in.params.addElement(54.0);
  in.params.addElement(45.0);
  in.params.addElement(4.0);
  paint.addElement(in);
  paint.saveToFile("piculi.pt");
  Paint road;
  road.loadFromFile("piculi.pt");
  EXPECT_EQ(road, paint);
}*/
TEST(SimpleTests, GetElementInfo){
    Paint screen;
    ElementData p;
    p.et = ET_POINT;
    p.params.addElement(10);
    p.params.addElement(10);
    ID pt = screen.addElement(p);
    ElementData e = screen.getElementInfo(pt);
    EXPECT_EQ(e.params[0], 10);
    EXPECT_EQ(e.params[1], 10);
    ElementData s;
    s.et = ET_SECTION;
    s.params.addElement(100);
    s.params.addElement(100);
    s.params.addElement(200);
    s.params.addElement(200);
    ID sect = screen.addElement(s);
    ElementData s1 = screen.getElementInfo(pt);
    EXPECT_EQ(s1.params[0], 100);
    EXPECT_EQ(s1.params[1], 100);
    EXPECT_EQ(s1.params[2], 200);
    EXPECT_EQ(s1.params[3], 200);
    ElementData c;
    c.et = ET_CIRCLE;
    c.params.addElement(10);
    c.params.addElement(10);
    c.params.addElement(10);
    ID circ = screen.addElement(c);
    ElementData m = screen.getElementInfo(circ);
    EXPECT_EQ(m.params[0], 10);
    EXPECT_EQ(m.params[1], 10);
    EXPECT_EQ(m.params[2], 10);
}
TEST(SimpleTests, PointSegDistTest){
    Paint screen;
    ElementData p;
    p.et = ET_POINT;
    p.params.addElement(10);
    p.params.addElement(10);
    ID pt = screen.addElement(p);
    ElementData s;
    s.et = ET_SECTION;
    s.params.addElement(100);
    s.params.addElement(100);
    s.params.addElement(200);
    s.params.addElement(200);
    ID sect = screen.addElement(s);
    ElementData e = screen.getElementInfo(pt);
    EXPECT_EQ(e.params[0], 10);
    EXPECT_EQ(e.params[1], 10);
    e = screen.getElementInfo(sect);
    EXPECT_EQ(e.params[0], 100);
    EXPECT_EQ(e.params[1], 100);
    EXPECT_EQ(e.params[2], 200);
    EXPECT_EQ(e.params[3], 200);
    RequirementData r;
    r.req = ET_POINTSECTIONDIST;
    r.objects.addElement(pt);
    r.objects.addElement(sect);
    r.params = 10;
    screen.addRequirement(r);
    ElementData ex = screen.getElementInfo(sect);
    EXPECT_NE(ex.params[0], 10);
    EXPECT_NE(ex.params[1], 10);
}