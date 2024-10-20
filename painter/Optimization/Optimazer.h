#ifndef OPTIMAZER_H
#define OPTIMAZER_H

class Optimizer {
public:
    virtual ~Optimizer() = default;
    virtual void solve() = 0;
};

#endif // OPTIMAZER_H