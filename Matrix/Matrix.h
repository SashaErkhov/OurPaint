#ifndef MATRIX
#define MATRIX

#include <iostream>

///////////////////////////// general purpose functions
template <typename T>
class Matrix
{
public:
    Matrix(unsigned int rowsI, unsigned int colsI) : rows(rowsI), cols(colsI) {
        matrix = new T* [rows];
        for (size_t i = 0; i < rows; i++) {
            matrix[i] = new T[cols];
            for (size_t j = 0; j < cols; j++) {
                matrix[i][j] = 0.0;
            }
        }
    }
    Matrix &operator*=(const Matrix &other) {
        if (rows == 1 && cols == 1){
            Matrix newMatrix(other.rows, other.cols);
            for (int i = 0; i < other.rows; i++) {
                for (int j = 0; j < other.cols; j++) {
                    newMatrix.matrix[i][j] = matrix[0][0] * other.matrix[i][j];
                }
            }
            matrix = newMatrix.matrix;
            rows = other.rows;
            cols = other.cols;
            return *this;
        }
        if (other.rows == 1 && other.cols == 1){
            for (int i = 0; i < other.rows; i++) {
                for (int j = 0; j < other.cols; j++) {
                    matrix[i][j] *= other.matrix[0][0];
                }
            }
            return *this;
        }
        if (cols != other.rows) {
            throw std::invalid_argument("Matrices must have the same size");
        }
        Matrix newMatrix(rows, other.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                newMatrix.matrix[i][j] = 0;
                for (int k = 0; k < other.rows; k++) {
                    newMatrix.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
                }
            }
        }
        matrix = newMatrix.matrix;
        rows = newMatrix.rows;
        cols = newMatrix.cols;
        return *this;
    }
    //some operators for computing
    Matrix &operator+=(const Matrix &other) {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have the same size");
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] += other.matrix[i][j];
            }
        }
        return *this;
    }

    Matrix &operator-=(const Matrix &other) {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have the same size");
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] -= other.matrix[i][j];
            }
        }
        return *this;
    }
    //------------------------------------------------------------------------------

    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        matrix = new T* [rows];
        for (size_t i = 0; i < rows; i++) {
            matrix[i] = new T[cols];
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

            matrix = new T* [rows];
            for (size_t i = 0; i < rows; i++) {
                matrix[i] = new T[cols];
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
    [[nodiscard]] inline unsigned int rows_size()  const  {return rows;}
    [[nodiscard]] inline unsigned int cols_size()  const   {return cols;}
    // Method to set elements in the matrix
    void setElement(unsigned int rowI, unsigned int colI, T value) {
        if (rowI >= rows || colI >= cols) {
            throw std::out_of_range("Index out of range");
        }
        matrix[rowI][colI] = value;
    }

    // Method to get elements from the matrix
    T getElement(unsigned int rowI, unsigned int colI) const {
        if (rowI >= rows || colI >= cols) {
            throw std::out_of_range("Index out of range");
        }
        return matrix[rowI][colI];
    }

    Matrix invMatrix() {
        if (rows != cols) {
            throw std::invalid_argument("Inverse can only be computed for square matrices.");
        }

        T determinant = det(matrix, rows);
        if (determinant == 0) {
            throw std::runtime_error("Matrix is singular and cannot be inverted.");
        }

        T** invMat = new T* [rows];
        for (size_t i = 0; i < rows; ++i) {
            invMat[i] = new T[cols];
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
    T** matrix;
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
    void freeMatrix(T** matrix, int size) {
        for (size_t i = 0; i < size; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    ///////////////////////////// functions for computing coefficients of the characteristic polynomial

    // Function to multiply two matrices
    T** um(T** a, T** b, int size) {
        T** c = new T* [size];
        for (int i = 0; i < size; i++)
        {
            c[i] = new T[size];
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
    void precomputePowers(T** matrix, int size, T*** powers, int maxExp) {
        powers[0] = new T* [size];
        for (int i = 0; i < size; ++i) {
            powers[0][i] = new T[size];
            for (int j = 0; j < size; ++j) {
                powers[0][i][j] = 0.0; // Filling with zeros
            }
            powers[0][i][i] = 1.0; // Setting ones on the diagonal
        }

        powers[1] = new T* [size];
        for (int i = 0; i < size; ++i) {
            powers[1][i] = new T[size];
            for (int j = 0; j < size; ++j) {
                powers[1][i][j] = matrix[i][j]; // Original matrix for power 1
            }
        }

        for (int exp = 2; exp <= maxExp; ++exp) {
            powers[exp] = um(powers[exp - 1], matrix, size);
        }
    }

    // trace of a matrix raised to a power
    T trace(T*** powers, int size, int n) {
        T sum = 0;
        for (int i = 0; i < size; ++i) {
            sum += powers[n][i][i];
        }
        return sum;
    }

    // coefficients of the characteristic polynomial with index k
    T coef(T*** powers, int N, int k, T* coefCache, bool* isComputed) {
        if (k == 0) {
            return 1.0;
        }

        if (isComputed[k]) {
            return coefCache[k];
        }

        T result = -trace(powers, N, k);
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
    void inverseMatrix(T** matrix, T** inverse, int size) {
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
            T factor = matrix[i][i];
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
    T det(T** matrix, int s) {
        // Creating a copy of the matrix
        T** tempMatrix = new T* [s];
        for (size_t i = 0; i < s; ++i) {
            tempMatrix[i] = new T[s];
            for (size_t j = 0; j < s; ++j) {
                tempMatrix[i][j] = matrix[i][j];
            }
        }

        T d = 1;
        T eps = 1e-10;
        for (size_t i = 0; i < s; ++i) {
            if (tempMatrix[i][i] < eps && tempMatrix[i][i] > -eps) {
                int k;
                for (k = i + 1; k < s; ++k) {
                    if (tempMatrix[k][i] != 0) {
                        // Swapping rows
                        for (size_t j = 0; j < s; ++j) {
                            T temp = tempMatrix[i][j];
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
                T factor = tempMatrix[k][i] / tempMatrix[i][i];
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
template <typename T>
Matrix<T> operator*(const Matrix<T> &matrix1, const Matrix<T> &matrix2) {
    Matrix result(matrix1);
    return result *= matrix2;
}

template <typename T>
Matrix<T> operator+(const Matrix<T> &matrix1, const Matrix<T> &matrix2) {
    if (matrix1.rows_size() != matrix2.rows_size() ||
        matrix1.cols_size() != matrix2.cols_size()) {
        throw std::invalid_argument("Matrices have different sizes");
    }
    Matrix result(matrix1);
    return result += matrix2;
}
template <typename T>
Matrix<T> operator-(const Matrix<T> &matrix1, const Matrix<T> &matrix2) {
    if (matrix1.rows_size() != matrix2.rows_size() ||
        matrix1.cols_size() != matrix2.cols_size()) {
        throw std::invalid_argument("Matrices have different sizes");
    }
    Matrix result(matrix1);
    return result -= matrix2;
}

#endif // MATRIX