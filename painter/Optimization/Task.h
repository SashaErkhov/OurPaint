#ifndef TASK_H
#define TASK_H
#define type double // TODO

#include "../../Matrix/Matrix.h"
#include "Function.h"
#include <vector>

class Task {
    Function* function;

public:
    Task(Function* function) : function(function) {};

    bool hasGrad() const {};
    bool hasJacob() const {};
    bool hasHess() const {};

    std::vector<type> getGrad() const {};
    Matrix<type> getJacob() const {};
    Matrix<type> getHess() const {};
};

#endif // TASK_H