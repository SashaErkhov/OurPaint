#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include "../../Matrix/Matrix.h"
#include "../requirements.h"

class Function {
protected:
    std::vector<PARAMID> params;
public:
    Function() : params() {}

    virtual ~Function() = default;

    virtual bool hasDeriv() = 0;

    virtual bool hasSecondDeriv() = 0;

    virtual double getDeriv(PARAMID) = 0;

    virtual double getSecondDeriv(PARAMID, PARAMID) = 0;

    virtual double operator()() = 0;

    virtual double operator()(std::vector<double> &) = 0; //set params
    virtual size_t getNumVars() = 0;

    virtual std::vector<PARAMID> getParam() = 0;
};

class SumOfSquares : public Function {
    std::vector<Function *> c_functions;
public:
    SumOfSquares() : c_functions(), Function() {}

    explicit SumOfSquares(const std::vector<Function *> &c_functions) : Function(), c_functions(c_functions) {
        for (auto f: c_functions) {
            std::vector<PARAMID> x0 = f->getParam();
            for (auto &i: x0) {
                if (std::find(params.begin(), params.end(), i) == params.end()) {
                    params.push_back(i);
                }
            }
        }
    }

    void addFunction(Function *f) {
        c_functions.push_back(f);
        for (auto x0: f->getParam()) {
            if (std::find(params.begin(), params.end(), x0) == params.end()) {
                params.push_back(x0);
            }
        }
    }

    size_t getNumVars() override {
        size_t i = params.size();
        return i;
    }

    std::vector<PARAMID> getParam() override {
        return params;
    }

    bool hasDeriv() override {
        return true;
    }

    bool hasSecondDeriv() override {
        return false;
    }

    double getDeriv(PARAMID i) override {
        double res = 0;
        for (auto f: c_functions) {
            res += 2 * (*f)() * f->getDeriv(i); //all the function must be not simple
        }
        return res;
    }

    double getSecondDeriv(PARAMID i, PARAMID j) override {
        // TODO: insert return statement
        return 0;
    }

    double operator()() override {
        double res = 0;
        for (auto f: c_functions) {
            res += (*f)() * (*f)();
        }
        return res;
    }

    double operator()(std::vector<double> &x0) override {
        if (x0.size() != getNumVars()) {
            throw std::runtime_error("This vector cant be used to calculate this function");
        }
        for (size_t i = 0; i < params.size(); i++) {
            *params[i] = x0[i];
        }
        return operator()();
    }
};

class FunctionOfReq : public Function {
    IReq *s_req;
public:
    explicit FunctionOfReq(IReq *req) : s_req(req), Function() {
        Arry<double *> p = s_req->getParams();
        for (size_t i = 0; i < p.getSize(); i++) {
            params.push_back(p[i]);
        }

    }

    bool hasDeriv() override {
        return true;
    }

    bool hasSecondDeriv() override {
        return true;
    }

    double getDeriv(PARAMID i) override {
        return s_req->getDerivative(i);
    }

    double getSecondDeriv(PARAMID i, PARAMID j) override {
        if (i == j) {
            return s_req->getSecondDerivative(i);
        }
        return s_req->getMixedDerivative(i, j);
    }

    double operator()() override {
        return s_req->getError();
    }

    double operator()(std::vector<double> &x0) override {
        for (size_t i = 0; i < params.size(); i++) {
            *params[i] = x0[i];
        }
        return s_req->getError();
    }

    size_t getNumVars() override {
        return params.size();
    }

    std::vector<PARAMID> getParam() override {
        return params;
    }
};

#endif // FUNCTION_H