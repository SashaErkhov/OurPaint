#include <gtest/gtest.h>
#include "paint.h"
#include "Assoc.h"
#include "for_test.h"

TEST(PaintTest, AddingElems){
  try{
  Paint paint;
  // Заполним параметры добавляемого элемента
  ElementData in;
  in.et=ET_POINT;
  in.params.addElement(1.0);
  in.params.addElement(2.0);
  // Добавим элемент
  ID pointid = paint.addElement(in);
  // Проверка???  
  ElementData ed = paint.getElementInfo(pointid);
  //std::cout << ed.et << " " << ed.params.getElement(0) << " " << ed.params.getElement(1) << " " << (ed.et==ET_POINT) << " " << (ed.params.getElement(0)==1.0) << " " << (ed.params.getElement(1)==2.0);
  EXPECT_EQ(ed.et,ET_POINT);
  EXPECT_EQ(ed.params.getElement(0),1.0);
  EXPECT_EQ(ed.params.getElement(1),2.0);
  std::cout << "end first test";
  } catch(...){
    std::cout << "Unknown error";
  }
}

TEST(PaintTest, save_and_load_full){
  Assoc<ID, ElementData> for_end;
  Paint paint;
  ElementData in;
  in.et=ET_POINT;
  in.params.addElement(3.0);
  in.params.addElement(2.0);
  for_end.addPair(paint.addElement(in), in);
  in=ElementData();
  in.et=ET_POINT;
  in.params.addElement(8.0);
  in.params.addElement(7.0);
  for_end.addPair(paint.addElement(in), in);
  in=ElementData();
  in.et=ET_SECTION;
  in.params.addElement(87.0);
  in.params.addElement(77.0);
  in.params.addElement(78.0);
  in.params.addElement(87.0);
  ID section_id=paint.addElement(in);
  for_end.addPair(section_id, in);
  in=ElementData();
  in.et=ET_SECTION;
  in.params.addElement(57.0);
  in.params.addElement(75.0);
  in.params.addElement(9.0);
  in.params.addElement(1.0);
  section_id=paint.addElement(in);
  for_end.addPair(section_id, in);
  in=ElementData();
  in.et=ET_CIRCLE;
  in.params.addElement(83.0);
  in.params.addElement(37.0);
  in.params.addElement(38.0);
  ID circle_id=paint.addElement(in);
  for_end.addPair(circle_id, in);
  in=ElementData();
  in.et=ET_CIRCLE;
  in.params.addElement(54.0);
  in.params.addElement(45.0);
  in.params.addElement(4.0);
  circle_id=paint.addElement(in);
  for_end.addPair(circle_id, in);
  paint.saveToFile("piculi.pt");
  Paint road;
  road.loadFromFile("piculi.pt");
  for(auto pos=for_end.begin(); pos!=for_end.end(); ++pos){
    EXPECT_EQ((*pos).second, road.getElementInfo((*pos).first));
  }
  std::cout << "end first test";
}

TEST(PaintTest, save_and_load_without_one){
  Assoc<ID, ElementData> for_end;
  Paint paint;
  ElementData in;
  in.et=ET_POINT;
  in.params.addElement(3.0);
  in.params.addElement(2.0);
  for_end.addPair(paint.addElement(in), in);
  in=ElementData();
  in.et=ET_POINT;
  in.params.addElement(8.0);
  in.params.addElement(7.0);
  for_end.addPair(paint.addElement(in), in);
  in=ElementData();
  in.et=ET_CIRCLE;
  in.params.addElement(83.0);
  in.params.addElement(37.0);
  in.params.addElement(38.0);
  ID circle_id=paint.addElement(in);
  for_end.addPair(circle_id, in);
  in=ElementData();
  in.et=ET_CIRCLE;
  in.params.addElement(54.0);
  in.params.addElement(45.0);
  in.params.addElement(4.0);
  circle_id=paint.addElement(in);
  for_end.addPair(circle_id, in);
  paint.saveToFile("opretor.pt");
  Paint road;
  road.loadFromFile("opretor.pt");
  for(auto pos=for_end.begin(); pos!=for_end.end(); ++pos){
    EXPECT_EQ((*pos).second, road.getElementInfo((*pos).first));
  }
}

TEST(PaintTest, save_and_load_without_all){
  Assoc<ID, ElementData> for_end;
  Paint paint;
  ElementData in;
  in=ElementData();
  paint.saveToFile("ywtor.pt");
  Paint road;
  road.loadFromFile("ywtor.pt");
  EXPECT_EQ(for_end.begin(), for_end.end());
}