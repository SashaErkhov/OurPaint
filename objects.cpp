#include "objects.h"

ID& ID::operator=(const ID& x){
    if(this!=&x){
        id=x.id;
    }
    return *this;
}

bool operator>(const ID& left, const ID& right){
  if(left.id<right.id){
    return false;
  }
  return true;
}