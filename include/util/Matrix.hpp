#if !defined(UTIL_MATRIX_HPP)
#define UTIL_MATRIX_HPP

/*
 * Models a matrix of any given size as a template with configurable 
 * content.
 * 
 * Specifies all basic calculations (addition, subtraction, multiplication,
 * determinant, inverse, minor, adjoint)
 * 
 * @author Tuukka Haapasalo
 * @created 2004-07-27
 */

#include "util/Log.hpp"
#include "util/String.hpp"
#include "util/NumberUtils.hpp"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdarg>

#ifdef minor
#undef minor
#endif // minor
namespace util {

template<class T>
class Matrix {
public:
    /* Creates a dummy, empty matrix */
    Matrix() :
        rows(0), cols(0), space(0) {
    }
    /* Creates an empty matrix of a given size */
    inline Matrix(size_t const rows, size_t const cols);
    /* Creates a matrix of given size and sets the values given as parameters */
    inline Matrix(size_t const rows, size_t const cols,
            T const first, ...);
    /* Creates a copy of this matrix */
    inline Matrix(Matrix const & other);
    /* Deletes the matrix */
    inline ~Matrix();

    /* Returns the value at the given matrix position */
    T at(size_t const row, size_t const col) const {
        if (col >= cols || row >= rows)
            ERROR("Matrix index out of bounds (" << row << ", " << col << ")");
        return space[row][col];
    }
    /* Sets the value at the given matrix position */
    void set(size_t const row, size_t const col, T const value) {
        if (col >= cols || row >= rows)
            ERROR("Matrix index out of bounds (" << row << ", " << col << ")");
        space[row][col] = value;
    }

    /* Sets a row of values at once */
    inline void setRow(size_t const row, ...);
    /* Sets all the values of the whole matrix at once */
    inline void setArray(int const dummy, ...);

    /* Checks whether the matrix is a square matrix */
    inline bool isSquare() const {
        return rows == cols;
    }

    /* Generates an identity matrix */
    inline static Matrix identity(size_t const size);

    /* 
     * Returns the minor of this matrix (a matrix with the given 
     * row & col removed) 
     */
    inline Matrix minor(size_t const row, size_t const col) const;

    /* Calculates the determinant of the matrix */
    inline T determinant() const;

    /* Returns the transpose of this matrix */
    inline Matrix transpose() const;

    /* Calculates the adjoint to this matrix */
    inline Matrix adjoint() const;

    /* Returns a textual representation of the matrix */
    inline String toString() const;

    /* Checks whether an inverse matrix exists for this matrix */
    bool canInvert() const {
        return determinant() != 0;
    }

    /* Calculates the inverse of this matrix */
    inline Matrix inverse() const;

    /* Assignment operator */
    inline Matrix & operator=(Matrix const & other);

    /* Equality comparison */
    inline bool operator==(Matrix const & other);
    bool operator!=(Matrix const & other) {
        return !(*this == other);
    }

    /* Multiplication */
    inline Matrix operator*(Matrix const & other) const;
    inline Matrix operator*(T const value) const;
    inline Matrix & operator*=(Matrix const & other);
    inline Matrix & operator*=(T const value);

    /* Addition */
    inline Matrix operator+(Matrix const & other) const;
    inline Matrix & operator+=(Matrix const & other);

    /* Subtraction */
    inline Matrix operator-(Matrix const & other) const;
    inline Matrix & operator-=(Matrix const & other);

    /* Returns a pointer to a row-major array of the matrix values */
    inline T * getArray() const;

    /* Returns the height of the matrix (rows) */
    T height() const {
        return rows;
    }
    /* Returns the width of the matrix (cols) */
    T width() const {
        return cols;
    }

private:
    /* Allocates space for the matrix */
    inline void allocate();
    /* Deallocates space for the matrix */
    inline void deallocate();
    /* Clears the matrix to zero (or copies values from a given matrix) */
    inline void clear(Matrix const * const other = 0);

private:
    T **space;
    size_t rows;
    size_t cols;

};

/*
 * Creates the matrix
 */
template<class T>
Matrix<T>::Matrix(size_t const rows_, size_t const cols_) :
    space(0), rows(rows_), cols(cols_) {
    allocate();
    clear();
}

/*
 * Creates and initializes the matrix
 */
template<class T>
Matrix<T>::Matrix(size_t const rows_, size_t const cols_,
        T const first, ...) :
    space(0), rows(rows_), cols(cols_) {
    allocate();
    va_list ap;

    va_start(ap, first);
    for (size_t h = 0; h < rows; h++) {
        for (size_t w = 0; w < cols; w++) {
            if (h == 0 && w == 0) {
                set(h, w, first);
            } else {
                set(h, w, va_arg(ap, T));
            }
        }
    }
    va_end(ap);
}

/*
 * Copies a matrix
 */
template<class T>
Matrix<T>::Matrix(Matrix const & other) :
    space(0), rows(other.rows), cols(other.cols) {
    allocate();
    clear(&other);
}

/*
 * Destroys the matrix
 */
template<class T>
Matrix<T>::~Matrix() {
    deallocate();
}

/*
 * Allocates space for the matrix
 */
template<class T>
void Matrix<T>::allocate() {
    if (rows < 1 || cols < 1)
        ERROR("Trying to create a non-existent matrix");

    if (space != 0)
        ERROR("Trying to allocate space for matrix that already has space");

    // Allocate space for the matrix
    space = new T*[rows];
    // Allocate space for the whole matrix as a row-major array
    space[0] = new T[rows * cols];

    // Set up the rest of the pointers to point to correct places in the
    // row-major array
    for (size_t r = 1; r < rows; r++) {
        space[r] = space[0] + (r * cols);
    }
}

/*
 * Deallocates space used by the matrix
 */
template<class T>
void Matrix<T>::deallocate() {
    if (space != 0) {
        // Free the row-major array
        delete[] space[0];
        // Free the array of pointers to the row-major array
        delete[] space;
        space = 0;
    }
}

/*
 * Clears the matrix to zero (or copies values from a given matrix)
 */
template<class T>
void Matrix<T>::clear(Matrix const * const other) {
    for (size_t h = 0; h < rows; h++) {
        for (size_t w = 0; w < cols; w++) {
            set(h, w, (other == 0) ? 0 : other->at(h, w));
        }
    }
}

/*
 * Generates an identity matrix
 */
template<class T>
Matrix<T> Matrix<T>::identity(size_t const size) {
    if (size < 1)
        ERROR("Trying to create a non-existent identity matrix");
    Matrix ident(size, size);
    for (size_t i = 0; i < size; i++)
        ident.set(i, i, 1);
    return ident;
}

/*
 * Returns a textual representation of the matrix
 */
template<class T>
String Matrix<T>::toString() const {
    size_t plen = 1;

    for (size_t h = 0; h < rows; h++) {
        for (size_t w = 0; w < cols; w++) {
            String a = String::format("%.2f", at(h, w));
            if (a.length() > plen)
                plen = a.length();
        }
    }

    std::ostringstream res;

    for (size_t h = 0; h < rows; h++) {
        res << "| ";
        for (size_t w = 0; w < cols; w++) {
            String a = String::format("%.2f", at(h, w));
            size_t p = a.length();
            while (p++ < plen)
                res << " ";
            res << a << " ";
        }
        res << "|" << std::endl;
    }

    return res.str();
}

/*
 * Shortcut for output streams
 */
template<class T>
inline std::ostream & operator<<(std::ostream & stream, Matrix<T> const & c) {
    stream << c.toString();
    return stream;
}

/*
 * Sets a row of values at once
 */
template<class T>
void Matrix<T>::setRow(size_t const row, ...) {
    va_list ap;
    if (row >= rows)
        ERROR("Matrix index out of bounds");

    va_start(ap, row);
    for (size_t w = 0; w < cols; w++) {
        T value = va_arg(ap, T);
        set(row, w, value);
    }
    va_end(ap);
}

/*
 * Sets all the values of the whole matrix at once
 */
template<class T>
void Matrix<T>::setArray(int const dummy, ...) {
    va_list ap;

    va_start(ap, dummy);
    for (size_t h = 0; h < rows; h++) {
        for (size_t w = 0; w < cols; w++) {
            T value = va_arg(ap, T);
            set(h, w, value);
        }
    }
    va_end(ap);
}

/*
 * Returns a pointer to a row-major array of the values
 */
template<class T>
T * Matrix<T>::getArray() const {
    return space[0];
}

/*
 * Returns the minor of this matrix (a matrix with the given
 * row & col removed)
 */
template<class T>
Matrix<T> Matrix<T>::minor(size_t const row, size_t const col) const {
    if (rows < 2 || cols < 2)
        ERROR("Trying to take a minor from too small a matrix");
    if (row >= rows || col >= cols)
        ERROR(
                "Trying to take a minor without a row/column that's not inside the original matrix");

    Matrix result(rows - 1, cols - 1);

    size_t rc = 0;
    for (size_t h = 0; h < rows; h++) {
        if (h != row) {
            size_t cc = 0;
            for (size_t w = 0; w < cols; w++) {
                if (w != col) {
                    result.set(rc, cc++, at(h, w));
                }
            }
            rc++;
        }
    }
    return result;
}

/*
 * Calculates the determinant of the matrix
 */
template<class T>
T Matrix<T>::determinant() const {
    if (!isSquare())
        ERROR("Trying to count the determinant of a non-square matrix");

    size_t side = rows;

    // Some basic situations
    ASSERT(side >= 1);
    if (side == 1)
        return at(0, 0);
    else if (side == 2)
        return at(1, 1) * at(0, 0) - at(0, 1) * at(1, 0);
    else if (side == 3)
        return space[0][0] * space[1][1] * space[2][2] - space[0][0]
                * space[1][2] * space[2][1] + space[1][0] * space[0][2]
                * space[2][1] - space[1][0] * space[0][1] * space[2][2]
                + space[2][0] * space[0][1] * space[1][2] - space[2][0]
                * space[0][2] * space[1][1];

    T sum = 0;
    for (size_t i = 0; i < side; i++) {
        sum += at(0, i) * pow(-1.0, (double) i) * minor(0, i).determinant();
    }

    return sum;
}

/*
 * Returns the transpose of this matrix
 */
template<class T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix result(cols, rows);

    for (size_t h = 0; h < rows; h++) {
        for (size_t w = 0; w < cols; w++) {
            result.set(w, h, at(h, w));
        }
    }
    return result;
}

/*
 * Calculates the adjoint to this matrix

 * B(i,j) = det(minor(i,j)) * (-1)^(i), where B is the adjoint-matrix.
 * Consult some math book in order to get better definition.
 */
template<class T>
Matrix<T> Matrix<T>::adjoint() const {
    if (!isSquare())
        ERROR("Trying to calculate adjoint for a non-square matrix");

    Matrix result(rows, cols);

    for (size_t h = 0; h < rows; h++) {
        for (size_t w = 0; w < cols; w++) {
            result.set(h, w, pow(-1.0, (double) (h + w))
                    * minor(h, w).determinant());
        }
    }
    return result.transpose();
}

/*
 * Calculates the inverse of this matrix
 */
template<class T>
Matrix<T> Matrix<T>::inverse() const {
    if (!canInvert())
        ERROR(
                "Trying to calculate inverse for a matrix for which it does not exist");

    return adjoint() * (1 / determinant());
}

/*
 * Assignment operator
 */
template<class T>
Matrix<T> & Matrix<T>::operator=(Matrix const & other) {
    if (rows != other.rows || cols != other.cols) {
        // Resize this matrix
        deallocate();
        rows = other.rows;
        cols = other.cols;
        allocate();
    }
    clear(&other);
    return *this;
}

/*
 * Equality comparison
 */
template<class T>
bool Matrix<T>::operator==(Matrix const & other) {
    // Different sizes?
    if (other.rows != rows || other.cols != cols)
        return false;

    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < cols; c++) {
            if (!equals(at(r, c), other.at(r, c), 0.001))
                return false;
        }
    }
    return true;
}

/*
 * Multiplication
 */
// Matrix * T
template<class T>
Matrix<T> Matrix<T>::operator*(T const value) const {
    Matrix result(*this);
    result *= value;
    return result;
}
// Matrix * Matrix
template<class T>
Matrix<T> Matrix<T>::operator*(Matrix const & other) const {
    // Matrix multiplication implemented here
    // Calculation result = this * other

    // The common size
    size_t common = cols;
    // Own width must be the same as other's height
    if (cols != other.rows)
        ERROR("Trying to multiply wrong sized matrices");

    Matrix result(rows, other.cols);

    // Matrix is now created
    // Calculate values
    for (size_t h = 0; h < rows; h++) {
        for (size_t w = 0; w < other.cols; w++) {
            T cell = 0;
            for (size_t i = 0; i < common; i++) {
                cell += at(h, i) * other.at(i, w);
            }
            result.set(h, w, cell);
        }
    }
    return result;
}
// Matrix *= T
template<class T>
Matrix<T> & Matrix<T>::operator*=(T const value) {
    for (size_t h = 0; h < rows; h++)
        for (size_t w = 0; w < cols; w++)
            set(h, w, at(h, w) * value);
    return *this;
}
// Matrix *= Matrix
template<class T>
Matrix<T> & Matrix<T>::operator*=(Matrix const & other) {
    *this = *this * other;
    return *this;
}

/*
 * Multiplication with T (other way around)
 */
template<class T>
inline Matrix<T> operator*(int const a, Matrix<T> const & b) {
    return b * a;
}

/*
 * Addition
 */
template<class T>
Matrix<T> Matrix<T>::operator+(Matrix const & other) const {
    Matrix result(*this);
    result += other;
    return result;
}

template<class T>
Matrix<T> & Matrix<T>::operator+=(Matrix const & other) {
    if (cols != other.cols || rows != other.rows)
        ERROR("Trying to add matrices of different sizes");
    for (size_t h = 0; h < rows; h++)
        for (size_t w = 0; w < cols; w++)
            set(h, w, at(h, w) + other.at(h, w));
    return *this;
}

/*
 * Subtraction
 */
template<class T>
Matrix<T> Matrix<T>::operator-(Matrix const & other) const {
    Matrix result(*this);
    result -= other;
    return result;
}

template<class T>
Matrix<T> & Matrix<T>::operator-=(Matrix const & other) {
    if (cols != other.cols || rows != other.rows)
        ERROR("Trying to add matrices of different sizes");
    for (size_t h = 0; h < rows; h++)
        for (size_t w = 0; w < cols; w++)
            set(h, w, at(h, w) - other.at(h, w));
    return *this;
}

}

#endif // !defined(UTIL_MATRIX_HPP)
