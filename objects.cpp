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

bool operator<(const ID& left, const ID& right){
  if(left.id<right.id){
    return true;
  }
  return false;
}

bool operator==(const ID& left, const ID& right){
    if(left.id==right.id){
        return true;
    }
    return false;
}

std::ifstream& operator>> (std::ifstream& in, ID& x){
    in>>x.id;
    return in;
}

std::ifstream& operator>> (std::ifstream& in, point& x){
    in>>x.x;
    in>>x.y;
    return in;
}