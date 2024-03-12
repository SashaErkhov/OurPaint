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
