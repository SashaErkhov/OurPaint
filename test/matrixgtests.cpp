//
// Created by Ardrass on 030, 30.07.2024.
//
#include <gtest/gtest.h>
#include "../Matrix/Matrix.h"

TEST(MatrixTest, pseudoMatrixTest) {
    Matrix<double> matrix(2, 3);
    matrix.setElement(0, 0, 3);
    matrix.setElement(0, 1, 2);
    matrix.setElement(0, 2, 2);
    matrix.setElement(1, 0, 2);
    matrix.setElement(1, 1, 3);
    matrix.setElement(1, 2, -2);
    Matrix<double> g = matrix.pseudoInv();
    ASSERT_TRUE(g.getElement(0, 0), 7/45);
    ASSERT_TRUE(g.getElement(0, 1), 2/45);
    ASSERT_TRUE(g.getElement(1, 0), 2/45);
    ASSERT_TRUE(g.getElement(1, 1), 7/45);
    ASSERT_TRUE(g.getElement(2, 0), 2/9);
    ASSERT_TRUE(g.getElement(2, 1), -2/9);
}