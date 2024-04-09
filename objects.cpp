#include "objects.h"

bool operator>(const ID& left, const ID& right){
  if(left.id<right.id){
    return false;
  }
  return true;
}