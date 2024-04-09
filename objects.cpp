#include "objects.h"
#include <fstream>

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

std::ifstream& operator>> (std::ifstream& in, ID& x){
    in>>x.id;
    return in;
}