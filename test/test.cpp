#include <gtest/gtest.h>
#include "paint.h"
#include "Assoc.h"

bool operator==(const ElementData& left, const ElementData& right){
  if((left.et==right.et)){
    for(size_t i=0; i<left.params.getSize();++i){
      if(left.params[i]!=right.params[i]){
        return false;
      }
    }
    return true;
  }
  return false;
}

ElementData Paint::getElementInfo(ID id){
    List<point>::iterator poi;
    List<section>::iterator sec;
    List<circle>::iterator cir;
    ElementData ret;
    try{
        poi=m_pointIDs.findByKey(id);
        ret.et=ET_POINT;
        ret.params.addElement((*poi).x);
        ret.params.addElement((*poi).y);
    }catch(std::runtime_error){
      try{
        sec=m_sectionIDs.findByKey(id);
        ret.et=ET_SECTION;
        ret.params.addElement((*((*sec).beg)).x);
        ret.params.addElement((*((*sec).beg)).y);
        ret.params.addElement((*((*sec).end)).x);
        ret.params.addElement((*((*sec).end)).y);
      }catch(std::runtime_error){
        cir=m_circleIDs.findByKey(id);
        ret.et=ET_SECTION;
        ret.params.addElement((*((*cir).center)).x);
        ret.params.addElement((*((*cir).center)).y);
        ret.params.addElement((*cir).R);
      }
    }
    return ret;
}

TEST(PaintTest, AddingElems){
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
  std::cout << ed.et << " " << ed.params.getElement(0) << " " << ed.params.getElement(1) << " " << (ed.et==ET_POINT) << " " << (ed.params.getElement(0)==1.0) << " " << (ed.params.getElement(1)==2.0);
  EXPECT_EQ(ed.et,ET_POINT);
  EXPECT_EQ(ed.params.getElement(0),1.0);
  EXPECT_EQ(ed.params.getElement(1),2.0); 
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