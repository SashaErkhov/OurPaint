#ifndef FUNCTION_H
#define FUNCTION_H
#define type double // TODO

class Function {
public:
    virtual ~Function() = default;
    virtual bool hasDeriv() = 0;
    virtual type getDeriv(type) = 0;
    virtual type operator()(type) = 0;
};

#endif // FUNCTION_H