/*
Author: Shlok Patel
Class: ECE4122
Last Date Modified: 9/16/2019
Description: Homework Problem 1: Matrix Class
What is the purpose of this file?
    This program implements the various ECE_Matrix class member functions
    that are defined in ECE_Matrix.h
*/

#include "ECE_Matrix.h"
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;    // using namespace so std:: is not needed before ostream arguments

// default constructor for Matrix, creates a 0x0 matrix.
ECE_Matrix::ECE_Matrix()
{
    m_rows = 0;
    m_cols = 0;
    // dynamic allocation of space needed for each element of the array, with type double.
    m_vals = new double*[m_rows];
    for(int i = 0; i < m_rows; i++)
    {
        m_vals[i] = new double[m_cols];
    }
    for(int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m_vals[i][j] = 0;
        }
    }
}

// constructor for Matrix when given size and values. Creates a size x size matrix with values
// given as the second parameter of the constructor call.
ECE_Matrix::ECE_Matrix(int size, double values)
{
    m_rows = size;
    m_cols = size;
    // dynamic allocation of space needed for each element of the array, with type double.
    m_vals = new double*[m_rows];
    for(int i = 0; i < m_rows; i++)
    {
        m_vals[i] = new double[m_cols];
    }
    for(int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m_vals[i][j] = values;
        }
    }
}

// constructor for Matrix when given rows, columns, and values. Creates a row x col matrix with values
// given as the third parameter of the constructor call.
ECE_Matrix::ECE_Matrix(int rows, int cols, double values)
{
    m_rows = rows;
    m_cols = cols;
    // dynamic allocation of space needed for each element of the array, with type double.
    m_vals = new double*[m_rows];
    for(int i = 0; i < m_rows; i++)
    {
        m_vals[i] = new double[m_cols];
    }
    for(int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m_vals[i][j] = values;
        }
    }
}

// constructor for Matrix when given an input file. Creates a matrix whose size is given by
// the first two numbers in the input file, then fills in the values of the matrix with the
// rest of the values from the file.
ECE_Matrix::ECE_Matrix(const std::string& filename)
{
    // reading and storing the number of rows and columns
    ifstream f(filename);
    f >> m_rows;
    f >> m_cols;

    // dynamic allocation of space needed for each element of the array, with type double.
    m_vals = new double*[m_rows];
    for(int i = 0; i < m_rows; i++)
    {
        m_vals[i] = new double[m_cols];
    }
    for(int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            f >> m_vals[i][j];
        }
    }
}

// Copy constructor for matrix that copies the entirety of Matrix m2 to the newly defined Matrix
ECE_Matrix::ECE_Matrix(const ECE_Matrix& m2)
{
    m_rows = m2.m_rows;
    m_cols = m2.m_cols;

    // dynamic allocation of space needed for each element of the array, with type double.
    if(m2.m_vals)
    {
        m_vals = new double *[m_rows];
        for (int i = 0; i < m_rows; i++)
        {
            m_vals[i] = new double[m_cols];
        }

        for (int i = 0; i < m_rows; i++)
        {
            for (int j = 0; j < m_cols; j++)
            {
                m_vals[i][j] = m2.m_vals[i][j];
            }
        }
    }
}

// getter functions
int ECE_Matrix::get_m_rows() const
{
    return m_rows;
}
int ECE_Matrix::get_m_cols() const
{
    return m_cols;
}

// overloading + operator for Matrix addition with another Matrix
const ECE_Matrix ECE_Matrix::operator+(const ECE_Matrix& m2) const
{
    int rows, cols;

    // setting size of resultant Matrix as largest of both original Matrices
    // rows and columns.
    if(m_rows >= m2.m_rows) rows = m_rows;
    else rows = m2.m_rows;
    if(m_cols >= m2.m_cols) cols = m_cols;
    else cols = m2.m_cols;

    // creating a new Matrix with rows and columns from above and setting initial
    // values to 0
    ECE_Matrix m(rows, cols, 0);
    // adding the values from both matrices to new matrix, leaving out of bounds
    // elements as 0
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m.m_vals[i][j] = m_vals[i][j];
        }
    }
    for (int i = 0; i < m2.m_rows; i++)
    {
        for (int j = 0; j < m2.m_cols; j++)
        {
            m.m_vals[i][j] += m2.m_vals[i][j];

        }
    }
    // return new matrix.
    return m;
}

// overloading - operator for Matrix subtraction from another Matrix
const ECE_Matrix ECE_Matrix::operator-(const ECE_Matrix& m2) const
{
    int rows, cols;

    // setting size of resultant Matrix as largest of both original Matrices
    // rows and columns.
    if(m_rows >= m2.m_rows) rows = m_rows;
    else rows = m2.m_rows;
    if(m_cols >= m2.m_cols) cols = m_cols;
    else cols = m2.m_cols;

    // creating a new Matrix with rows and columns from above and setting initial
    // values to 0
    ECE_Matrix m(rows, cols, 0);
    // subtracting the values from both matrices to new matrix, leaving out of bounds
    // elements as 0
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m.m_vals[i][j] = m_vals[i][j];
        }
    }
    for (int i = 0; i < m2.m_rows; i++)
    {
        for (int j = 0; j < m2.m_cols; j++)
        {
            m.m_vals[i][j] -= m2.m_vals[i][j];

        }
    }
    // return new matrix.
    return m;
}

// overloading = operator for setting a Matrix equal to another Matrix
ECE_Matrix& ECE_Matrix::operator=(const ECE_Matrix& m2)
{
    // delete old matrix
    if(this == &m2) return *this;
    if(m_vals != nullptr)
    {
        for (int i = 0; i < m_rows; i++)
        {
            delete[] m_vals[i];
        }
        delete[] m_vals;
    }
    m_rows = m2.m_rows;
    m_cols = m2.m_cols;
    // dynamically allocate new matrix with size and values equal to m2
    m_vals = new double*[m_rows];
    for(int i = 0; i < m_rows; i++)
    {
        m_vals[i] = new double[m_cols];
    }
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m_vals[i][j] = m2.m_vals[i][j];
        }
    }
    // return original matrix
    return *this;
}

// overloading + operator for adding a double to all Matrix elements
const ECE_Matrix ECE_Matrix::operator+(double rhs) const
{
    // iterate through each element and add double
    ECE_Matrix m(m_rows, m_cols, 0);
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m.m_vals[i][j] = m_vals[i][j] + rhs;
        }
    }
    return m;
}

// overloading - operator for subtracting a double from all Matrix elements
const ECE_Matrix ECE_Matrix::operator-(double rhs) const
{
    // iterate through each element and subtract double
    ECE_Matrix m(m_rows, m_cols, 0);
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m.m_vals[i][j] = m_vals[i][j] - rhs;
        }
    }
    return m;
}

// overloading * operator for multiplying a double to all Matrix elements
const ECE_Matrix ECE_Matrix::operator*(double rhs) const
{
    // iterate through each element and multiply double
    ECE_Matrix m(m_rows, m_cols, 0);
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m.m_vals[i][j] = m_vals[i][j] * rhs;
        }
    }
    return m;
}

// overloading / operator for dividing a double from all Matrix elements
const ECE_Matrix ECE_Matrix::operator/(double rhs) const
{
    // condition checks if double is 0, in which case a matrix of 0's is returned
    ECE_Matrix m(m_rows, m_cols, 0);
    if(rhs == 0) {
        return m;
    }
        // else, iterate through each element and divide double
    else
    {
        for (int i = 0; i < m_rows; i++)
        {
            for (int j = 0; j < m_cols; j++)
            {
                m.m_vals[i][j] = m_vals[i][j] / rhs;
            }
        }
        return m;
    }
}

// overload () operator for returning specific element of matrix
double ECE_Matrix::operator()(int row, int col) const
{
    return m_vals[row][col];
}

// overload + operator for adding double on left hand side to Matrix on right hand side
ECE_Matrix operator+(double lhs, const ECE_Matrix& rhs)
{
    // practically same function from previous + overload
    ECE_Matrix m(rhs.m_rows, rhs.m_cols, 0);
    for (int i = 0; i < m.m_rows; i++)
    {
        for (int j = 0; j < m.m_cols; j++)
        {
            m.m_vals[i][j] = lhs + rhs.m_vals[i][j];
        }
    }
    return m;
}

// overload + operator for subtracting double from left hand side to Matrix on right hand side
ECE_Matrix operator-(double lhs, const ECE_Matrix& rhs)
{
    // practically same function from previous - overload
    ECE_Matrix m(rhs.m_rows, rhs.m_cols, 0);
    for (int i = 0; i < m.m_rows; i++)
    {
        for (int j = 0; j < m.m_cols; j++)
        {
            m.m_vals[i][j] = lhs - rhs.m_vals[i][j];
        }
    }
    return m;
}

// overload * operator for multiplying double to left hand side to Matrix on right hand side
ECE_Matrix operator*(double lhs, const ECE_Matrix& rhs)
{
    // practically same function from previous * overload
    ECE_Matrix m(rhs.m_rows, rhs.m_cols, 0);
    for (int i = 0; i < m.m_rows; i++)
    {
        for (int j = 0; j < m.m_cols; j++)
        {
            m.m_vals[i][j] = lhs * rhs.m_vals[i][j];
        }
    }
    return m;
}

// transpose function that transposes all elements of a Matrix
const ECE_Matrix ECE_Matrix::transpose() const
{
    // create a new matrix with the original rows and columns swapped, then store the values inverted.
    ECE_Matrix m(m_cols, m_rows, 0);
    for (int i = 0; i < m.m_rows; i++)
    {
        for (int j = 0; j < m.m_cols; j++)
        {
            m.m_vals[i][j] = m_vals[j][i];
        }
    }
    return m;
}

// overload += operator for performing an addition with two matrices by directly modifying the resultant matrix
ECE_Matrix ECE_Matrix::operator+=(const ECE_Matrix& rhs)
{
    for (int i = 0; i < this->m_rows; i++)
    {
        for (int j = 0; j < this->m_cols; j++)
        {
            this->m_vals[i][j] += rhs.m_vals[i][j];
        }
    }
    // return original matrix
    return *this;
}

// overload -= operator for performing subtraction with two matrices by directly modifying the resultant matrix
ECE_Matrix ECE_Matrix::operator-=(const ECE_Matrix& rhs)
{
    for (int i = 0; i < this->m_rows; i++)
    {
        for (int j = 0; j < this->m_cols; j++)
        {
            this->m_vals[i][j] -= rhs.m_vals[i][j];
        }
    }
    // return original matrix
    return *this;
}

// ECE_Matrix destructor
ECE_Matrix::~ECE_Matrix()
{
    // deletes each element of Matrix then deletes pointer to matrix to free up all allocated memory
    if(m_vals) {
        for (int i = 0; i < m_rows; i++) {
            delete[] m_vals[i];
        }
    }
    delete[] m_vals;
}

// output stream function for outputting correctly formatted matrices
std::ostream& operator<<(std::ostream& os, const ECE_Matrix& m)
{
    for(int i = 0; i < m.get_m_rows(); i++)
    {
        for (int j = 0; j < m.get_m_cols(); j++)
        {
            os << scientific << setw(12) << setprecision(3) << m(i, j);
        }
        os << endl;
    }
    return os;
}

