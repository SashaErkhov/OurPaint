#include <gtest/gtest.h>
#include "../paint.h"

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
    RequirementData r;
    r.req = ET_POINTSECTIONDIST;
    r.objects.addElement(pt);
    r.objects.addElement(pt);
    r.params = 10;
    screen.addRequirement(r);
    ElementData e = screen.getElementInfo(sect);
    EXPECT_NE(e.params[0], 10);
    EXPECT_NE(e.params[1], 10);
}