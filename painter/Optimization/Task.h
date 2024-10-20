#ifndef TASK_H
#define TASK_H
#define type double* // TODO

#include "../../Matrix/Matrix.h"
#include "Function.h"
#include <vector>

class Task {
    Function* function;

public:
    Task(Function* function) : function(function) {};

    bool hasGrad() const {
        return function->hasDeriv();
    };

    bool hasJacob() const {
        return function->hasDeriv();
    };

    bool hasHess() const {
        return function->hasSecondDeriv();
    };


    std::vector<double> getGrad() const {
        if (!hasGrad()) {
            throw std::runtime_error("Gradient not available");
        }

        size_t dim = function->inputDimension();

        std::vector<double> gradient;

        for (size_t i = 0; i < dim; ++i) {
            gradient.push_back(function->getDeriv(i));
        }

        return gradient;
    };

    Matrix<double> getJacob() const {
        if (!hasJacob()) {
            throw std::runtime_error("Jacobin not available");
        }

        size_t inputDim = function->inputDimension();
        size_t outputDim = function->outputDimension();
        Matrix<double> jacobian(outputDim, inputDim);

        for (size_t i = 0; i < outputDim; ++i) {
            for (size_t j = 0; j < inputDim; ++j) {
                jacobian.setElement(i, j, function->getDeriv(i));  // calc derivative by i function and by j var
            }
        }

        return jacobian;
    };

    Matrix<double> getHess() const {
        if (!hasHess()) {
            throw std::runtime_error("Hessian not available");
        }

        size_t inputDim = function->inputDimension();
        Matrix<double> hessian(inputDim, inputDim);

        for (size_t i = 0; i < inputDim; ++i) {
            for (size_t j = 0; j < inputDim; ++j) {
                hessian.setElement(i, j, function->getSecondDeriv(j));  // calc second derivative by i var and by j var
            }
        }

        return hessian;
    };
};

#endif // TASK_H