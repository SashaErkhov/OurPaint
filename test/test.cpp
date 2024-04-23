#include <gtest/gtest.h>
#include "paint.h"

TEST(PaintTest, AddingElems){
  Paint paint;
  // Заполним параметры добавляемого элемента
  Arry<double> params;
  params.addElement(1.0);
  params.addElement(2.0);
  // Добавим элемент
  ID pointid = paint.addElement(ET_POINT,params);
  // Проверка???  
  ElementData ed = paint.getElementInfo(pointid);
  ASSERT_EQ(ed.et,ET_POINT);
  ASSERT_EQ(ed.params.getElement(0),1.0);
  ASSERT_EQ(ed.params.getElement(1),2.0); 
}

TEST(PaintTest, save_and_load){
  Paint paint;
  ElementData in;
  in.et=ET_POINT;
  in.params.addElement(3.0);
  in.params.addElement(2.0);
  paint.addElement(in);
  in.et=ET_POINT;
  in.params.addElement(8.0);
  in.params.addElement(7.0);
  paint.addElement(in);
  in.et=ET_SECTION;
  in.params.addElement(87.0);
  in.params.addElement(77.0);
  in.params.addElement(78.0);
  in.params.addElement(87.0);
  paint.addElement(in);
}
