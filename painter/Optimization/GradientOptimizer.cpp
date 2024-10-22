//
// Created by Eugene Bychkov on 20.10.2024.
//
#include "GradientOptimizer.h"

void GradientOptimizer::solve() {
    double step = 0.0001;
    while (true) {
        std::vector<double> gradient = c_task->getGrad();
        std::vector<double> temp = c_task->getParameter();
        for (int i = 0; i < temp.size(); i++) {
            temp[i] -= step * gradient[i];
        }
        if (c_task->setParams(temp) < eps) {
            isOptimized = true;
            break;
        }
    }
}
