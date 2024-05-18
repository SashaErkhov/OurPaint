#ifndef MATRIX
#define MATRIX

#include <iostream>

///////////////////////////// general purpose functions

class Matrix
{
public:
    Matrix(unsigned int rowsI, unsigned int colsI) : rows(rowsI), cols(colsI) {
        matrix = new double* [rows];
        for (size_t i = 0; i < rows; i++) {
            matrix[i] = new double[cols];
            for (size_t j = 0; j < cols; j++) {
                matrix[i][j] = 0.0;
            }
        }
    }

    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        matrix = new double* [rows];
        for (size_t i = 0; i < rows; i++) {
            matrix[i] = new double[cols];
            for (size_t j = 0; j < cols; j++) {
                matrix[i][j] = other.matrix[i][j];
            }
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            for (size_t i = 0; i < rows; i++) {
                delete[] matrix[i];
            }
            delete[] matrix;

            rows = other.rows;
            cols = other.cols;

            matrix = new double* [rows];
            for (size_t i = 0; i < rows; i++) {
                matrix[i] = new double[cols];
                for (size_t j = 0; j < cols; j++) {
                    matrix[i][j] = other.matrix[i][j];
                }
            }
        }
        return *this;
    }


    ~Matrix()
    {
        for (size_t i = 0; i < rows; i++)
        {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    // Method to set elements in the matrix
    void setElement(unsigned int rowI, unsigned int colI, double value) {
        if (rowI >= rows || colI >= cols) {
            throw std::out_of_range("Index out of range");
        }
        matrix[rowI][colI] = value;
    }

    // Method to get elements from the matrix
    double getElement(unsigned int rowI, unsigned int colI) const {
        if (rowI >= rows || colI >= cols) {
            throw std::out_of_range("Index out of range");
        }
        return matrix[rowI][colI];
    }

    Matrix invMatrix() {
        if (rows != cols) {
            throw std::invalid_argument("Inverse can only be computed for square matrices.");
        }

        double determinant = det(matrix, rows);
        if (determinant == 0) {
            throw std::runtime_error("Matrix is singular and cannot be inverted.");
        }

        double** invMat = new double* [rows];
        for (size_t i = 0; i < rows; ++i) {
            invMat[i] = new double[cols];
        }

        inverseMatrix(matrix, invMat, rows);

        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < rows; j++) {
                result.setElement(i, j, invMat[i][j]);
            }
        }

        freeMatrix(invMat, rows);

        return result;
    }

private:
    double** matrix;
    unsigned int rows;
    unsigned int cols;

    // comparing strings
    bool compareStrings(const char* str1, const char* str2) {
        size_t i = 0;
        while (str1[i] != '\0' && str2[i] != '\0') {
            if (str1[i] != str2[i]) {
                return false;
            }
            i++;
        }
        return str1[i] == str2[i];
    }

    // Function to free matrix memory
    void freeMatrix(double** matrix, int size) {
        for (size_t i = 0; i < size; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    ///////////////////////////// functions for computing coefficients of the characteristic polynomial

    // Function to multiply two matrices
    double** um(double** a, double** b, int size) {
        double** c = new double* [size];
        for (int i = 0; i < size; i++)
        {
            c[i] = new double[size];
            for (int j = 0; j < size; j++)
            {
                c[i][j] = 0;
                for (int k = 0; k < size; k++) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return c;
    }

    // precomputing matrix to the highest required power and saving it
    void precomputePowers(double** matrix, int size, double*** powers, int maxExp) {
        powers[0] = new double* [size];
        for (int i = 0; i < size; ++i) {
            powers[0][i] = new double[size];
            for (int j = 0; j < size; ++j) {
                powers[0][i][j] = 0.0; // Filling with zeros
            }
            powers[0][i][i] = 1.0; // Setting ones on the diagonal
        }

        powers[1] = new double* [size];
        for (int i = 0; i < size; ++i) {
            powers[1][i] = new double[size];
            for (int j = 0; j < size; ++j) {
                powers[1][i][j] = matrix[i][j]; // Original matrix for power 1
            }
        }

        for (int exp = 2; exp <= maxExp; ++exp) {
            powers[exp] = um(powers[exp - 1], matrix, size);
        }
    }

    // trace of a matrix raised to a power
    double trace(double*** powers, int size, int n) {
        double sum = 0;
        for (int i = 0; i < size; ++i) {
            sum += powers[n][i][i];
        }
        return sum;
    }

    // coefficients of the characteristic polynomial with index k
    double coef(double*** powers, int N, int k, double* coefCache, bool* isComputed) {
        if (k == 0) {
            return 1.0;
        }

        if (isComputed[k]) {
            return coefCache[k];
        }

        double result = -trace(powers, N, k);
        for (int i = 1; i < k; ++i) {
            result -= coef(powers, N, i, coefCache, isComputed) * trace(powers, N, k - i);
        }
        result /= k;

        coefCache[k] = result;
        isComputed[k] = true;



        return result;
    }

    ///////////////////////////// functions for computing the inverse matrix

    // Calculating the inverse matrix
    void inverseMatrix(double** matrix, double** inverse, int size) {
        // Creating an identity matrix
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                inverse[i][j] = (i == j) ? 1.0 : 0.0;
            }
        }

        // Applying Gauss-Jordan elimination
        for (size_t i = 0; i < size; ++i) {
            // Ensuring the element matrix[i][i] is non-zero
            if (matrix[i][i] == 0.0) {
                // Searching for a non-zero element in the current column
                for (size_t j = i + 1; j < size; ++j) {
                    if (matrix[j][i] != 0.0) {
                        // Swapping rows in both matrices
                        for (size_t k = 0; k < size; ++k) {
                            std::swap(matrix[i][k], matrix[j][k]);
                            std::swap(inverse[i][k], inverse[j][k]);
                        }
                        break;
                    }
                }
            }

            // Normalizing the row so that element matrix[i][i] equals 1
            double factor = matrix[i][i];
            for (size_t j = 0; j < size; ++j) {
                matrix[i][j] /= factor;
                inverse[i][j] /= factor;
            }

            // Nullifying all elements in the current column
            for (size_t j = 0; j < size; ++j) {
                if (j != i) {
                    factor = matrix[j][i];
                    for (size_t k = 0; k < size; ++k) {
                        matrix[j][k] -= factor * matrix[i][k];
                        inverse[j][k] -= factor * inverse[i][k];
                    }
                }
            }
        }
    }

    ///////////////////////////// functions for computing the determinant

    // determinant by Gauss method
    double det(double** matrix, int s) {
        // Creating a copy of the matrix
        double** tempMatrix = new double* [s];
        for (size_t i = 0; i < s; ++i) {
            tempMatrix[i] = new double[s];
            for (size_t j = 0; j < s; ++j) {
                tempMatrix[i][j] = matrix[i][j];
            }
        }

        double d = 1;
        double eps = 1e-10;
        for (size_t i = 0; i < s; ++i) {
            if (tempMatrix[i][i] < eps && tempMatrix[i][i] > -eps) {
                int k;
                for (k = i + 1; k < s; ++k) {
                    if (tempMatrix[k][i] != 0) {
                        // Swapping rows
                        for (size_t j = 0; j < s; ++j) {
                            double temp = tempMatrix[i][j];
                            tempMatrix[i][j] = tempMatrix[k][j];
                            tempMatrix[k][j] = temp;
                        }
                        d *= -1;
                        break;
                    }
                }
                if (k == s) {
                    freeMatrix(tempMatrix, s);
                    // all zero
                    return 0;
                }
            }
            // Reducing matrix to upper triangular form
            for (size_t k = i + 1; k < s; ++k) {
                double factor = tempMatrix[k][i] / tempMatrix[i][i];
                for (size_t j = i; j < s; ++j) {
                    tempMatrix[k][j] -= factor * tempMatrix[i][j];
                }
            }
            d *= tempMatrix[i][i];
        }

        freeMatrix(tempMatrix, s);

        return d;
    }
};

#endif // MATRIX