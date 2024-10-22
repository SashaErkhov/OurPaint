//
// Created by Eugene Bychkov on 20.10.2024.
//

#ifndef OURPAINT_GRADIENTOPTIMIZER_H
#define OURPAINT_GRADIENTOPTIMIZER_H
#include "Optimazer.h"
#include "Task.h"
class GradientOptimizer : public Optimizer{
    Task* c_task;
    bool isOptimized;
    public:
        GradientOptimizer(Task* task): c_task(task), isOptimized(false){}
        void solve() override;
        inline bool isSolved() {
            return isOptimized;
        }
};
#endif //OURPAINT_GRADIENTOPTIMIZER_H
