#include "paint.h"
#include "Assoc.h"
#include "for_test.h"

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
