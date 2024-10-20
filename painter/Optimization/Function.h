#ifndef FUNCTION_H
#define FUNCTION_H
#define type double* // TODO

class Function {
public:
    virtual ~Function() = default;
    virtual bool hasDeriv() const = 0;
    virtual bool hasSecondDeriv() const = 0;
    virtual double getDeriv(size_t varIndex) = 0;
    virtual double getSecondDeriv(size_t varIndex) = 0;
    virtual size_t inputDimension() const = 0;
    virtual size_t outputDimension() const = 0;
    virtual type operator()(type) = 0;
};

#endif // FUNCTION_H