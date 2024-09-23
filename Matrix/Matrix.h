#ifndef MATRIX
#define MATRIX

#include <iostream>
#include <stdexcept>
#include <cmath>
template <typename T>
class Matrix
{
public:
    Matrix(unsigned int rowsI, unsigned int colsI) : rows(rowsI), cols(colsI) {
        matrix = new T * [rows];
        for (size_t i = 0; i < rows; i++) {
            matrix[i] = new T[cols];
            for (size_t j = 0; j < cols; j++) {
                matrix[i][j] = 0.0;
            }
        }
    }

    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        matrix = new T * [rows];
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

            matrix = new T * [rows];
            for (size_t i = 0; i < rows; i++) {
                matrix[i] = new T[cols];
                for (size_t j = 0; j < cols; j++) {
                    matrix[i][j] = other.matrix[i][j];
                }
            }
        }
        return *this;
    }

    //~Matrix() {
    //    for (size_t i = 0; i < rows; i++) {
    //        delete[] matrix[i];
    //    }
    //    delete[] matrix;
    //}

    Matrix transpose() const {
        Matrix transposed(cols, rows);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                transposed.matrix[j][i] = matrix[i][j];
            }
        }
        return transposed;
    }
    void setIdentity() {
        if (rows != cols) {
            throw std::runtime_error("Matrix must be square to set as identity");
        }
        for (unsigned int i = 0; i < rows; ++i) {
            for (unsigned int j = 0; j < cols; ++j) {
                if (i == j) {
                    matrix[i][j] = static_cast<T>(1);
                } else {
                    matrix[i][j] = static_cast<T>(0);
                }
            }
        }
    }

    Matrix pseudoInv(unsigned int maxIterations = 1000, T tolerance = 1e-10) const {
        Matrix A = *this;
        Matrix A_T = A.transpose();
        Matrix A_TA = A_T * A;
        Matrix I(A_TA.rows_size(), A_TA.cols_size());
        for (size_t i = 0; i < I.rows_size(); ++i) {
            I.setElement(i, i, 1.0);
        }

        Matrix Y = A_TA;
        Matrix Z = I;
        T alpha = 1.0 / A.norm();
        Y *= alpha;
        Z *= alpha;

        for (unsigned int k = 0; k < maxIterations; ++k) {
            Matrix Y_new = Y * (I * 2.0 - A * Z * Y);
            Matrix Z_new = (I * 2.0 - A * Z * Y) * Z;
            if ((Y_new - Y).norm() < tolerance && (Z_new - Z).norm() < tolerance) {
                break;
            }
            Y = Y_new;
            Z = Z_new;
        }

        return Z * A_T;
    }
    Matrix& operator*=(const Matrix& other) {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrices must have compatible dimensions for multiplication.");
        }

        Matrix newMatrix(rows, other.cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < other.cols; j++) {
                newMatrix.matrix[i][j] = 0;
                for (size_t k = 0; k < cols; k++) {
                    newMatrix.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
                }
            }
        }

        for (size_t i = 0; i < rows; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;

        matrix = newMatrix.matrix;
        newMatrix.matrix = nullptr;

        rows = newMatrix.rows;
        cols = newMatrix.cols;

        return *this;
    }
    Matrix& operator-(){
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                matrix[i][j] = -matrix[i][j];
            }
        }
        return *this;
    }
    Matrix& operator*=(T scalar) {
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                matrix[i][j] *= scalar;
            }
        }
        return *this;
    }

    Matrix& operator/=(T scalar) {
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                matrix[i][j] /= scalar;
            }
        }
        return *this;
    }

    Matrix& operator+=(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have the same size for addition.");
        }
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                matrix[i][j] += other.matrix[i][j];
            }
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrices must have the same size for subtraction.");
        }
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                matrix[i][j] -= other.matrix[i][j];
            }
        }
        return *this;
    }

    [[nodiscard]] inline unsigned int rows_size() const { return rows; }
    [[nodiscard]] inline unsigned int cols_size() const { return cols; }

    T norm() const {
        T sum = 0;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                sum += matrix[i][j] * matrix[i][j];
            }
        }
        return std::sqrt(sum);
    }

    void setElement(unsigned int rowI, unsigned int colI, T value) {
        if (rowI >= rows || colI >= cols) {
            throw std::out_of_range("Index out of range");
        }
        matrix[rowI][colI] = value;
    }

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

        T** invMat = new T * [rows];
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
    inline void print() const {
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    T** matrix;
    unsigned int rows;
    unsigned int cols;

    void freeMatrix(T** matrix, int size) {
        for (size_t i = 0; i < size; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    void inverseMatrix(T** matrix, T** inverse, int size) {
        // �������� ����� �������� �������, ����� �� �������� ��
        T** tempMatrix = new T * [size];
        for (size_t i = 0; i < size; ++i) {
            tempMatrix[i] = new T[size];
            for (size_t j = 0; j < size; ++j) {
                tempMatrix[i][j] = matrix[i][j];
            }
        }

        // �������� ��������� �������
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                inverse[i][j] = (i == j) ? 1.0 : 0.0;
            }
        }

        // ���������� ������ ������-�������
        for (size_t i = 0; i < size; ++i) {
            // ����������� ���������� �������� �� ���������
            if (tempMatrix[i][i] == 0.0) {
                for (size_t j = i + 1; j < size; ++j) {
                    if (tempMatrix[j][i] != 0.0) {
                        for (size_t k = 0; k < size; ++k) {
                            std::swap(tempMatrix[i][k], tempMatrix[j][k]);
                            std::swap(inverse[i][k], inverse[j][k]);
                        }
                        break;
                    }
                }
            }

            // ������������ ������ ���, ����� ������������ ������� ���� ����� 1
            T factor = tempMatrix[i][i];
            for (size_t j = 0; j < size; ++j) {
                tempMatrix[i][j] /= factor;
                inverse[i][j] /= factor;
            }

            // ��������� ���� ��������� � ������� �������, ����� �������������
            for (size_t j = 0; j < size; ++j) {
                if (j != i) {
                    factor = tempMatrix[j][i];
                    for (size_t k = 0; k < size; ++k) {
                        tempMatrix[j][k] -= factor * tempMatrix[i][k];
                        inverse[j][k] -= factor * inverse[i][k];
                    }
                }
            }
        }

        // ������� ��������� �������
        freeMatrix(tempMatrix, size);
    }

    T det(T** matrix, int s) {
        T** tempMatrix = new T * [s];
        for (size_t i = 0; i < s; ++i) {
            tempMatrix[i] = new T[s];
            for (size_t j = 0; j < s; ++j) {
                tempMatrix[i][j] = matrix[i][j];
            }
        }

        T d = 1;
        T eps = 10e-20;
        for (size_t i = 0; i < s; ++i) {
            if (std::abs(tempMatrix[i][i]) < eps) {
                for (size_t k = i + 1; k < s; ++k) {
                    if (std::abs(tempMatrix[k][i]) > eps) {
                        for (size_t j = 0; j < s; ++j) {
                            std::swap(tempMatrix[i][j], tempMatrix[k][j]);
                        }
                        d *= -1;
                        break;
                    }
                }
            }
            if (std::abs(tempMatrix[i][i]) < eps) {
                freeMatrix(tempMatrix, s);
                return 0;
            }
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
Matrix<T> operator*(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
    Matrix<T> result(matrix1);
    return result *= matrix2;
}

template <typename T>
Matrix<T> operator+(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
    if (matrix1.rows_size() != matrix2.rows_size() ||
        matrix1.cols_size() != matrix2.cols_size()) {
        throw std::invalid_argument("Matrices must have the same size for addition.");
    }
    Matrix<T> result(matrix1);
    return result += matrix2;
}

template <typename T>
Matrix<T> operator-(const Matrix<T>& matrix1, const Matrix<T>& matrix2) {
    if (matrix1.rows_size() != matrix2.rows_size() ||
        matrix1.cols_size() != matrix2.cols_size()) {
        throw std::invalid_argument("Matrices must have the same size for subtraction.");
    }
    Matrix<T> result(matrix1);
    return result -= matrix2;
}
template <typename T>
Matrix<T> operator*(const Matrix<T>& matrix, const T& scalar){
    Matrix<T> tmp = matrix;
    return tmp *= scalar;
}
template <typename T>
Matrix<T> operator/(const Matrix<T>& matrix, const T& scalar){
    Matrix<T> tmp = matrix;
    return tmp /= scalar;
}
#endif // MATRIX
