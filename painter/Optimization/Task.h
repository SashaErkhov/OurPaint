#ifndef TASK_H
#define TASK_H

#include "../../Matrix/Matrix.h"
#include "Function.h"
#include <vector>

class Task {
    Function* function;

public:
    Task(Function* function) : function(function) {};

    bool hasGrad() const {
        if (function->hasDeriv()){
            return true;
        }
        return false;
    };
    bool hasJacob() const {
        if (function->hasDeriv()){
            return true;
        }
        return false;
    };
    bool hasHess() const {
        if (function->hasSecondDeriv()) {
            return true;
        }
        return false;
    };

    std::vector<double> getGrad() const {
        if (!hasGrad()) {
            throw std::runtime_error("Task has no gradient");
        }
        std::vector<double> grad;
        for (auto& a : function->getParam()) {
            grad.push_back(function->getDeriv(a));
        }
        return grad;
    };
    Matrix<double> getJacob() const {};
    Matrix<double> getHess() const {};
    std::vector<double> getParameter() const {
        std::vector<double> params;
        for (auto& a : function->getParam()) {
            params.push_back(*a);
        }
        return params;
    };
    double getDim() const {
        return (*function)();
    };
    double setParams(std::vector<double> params) {
        return (*function)(params);
    };
};

#endif // TASK_H