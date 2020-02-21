/*
Author: Shlok Patel
Class: ECE4122
Last Date Modified: 9/16/2019
Description: Homework Problem 1: Matrix Class
What is the purpose of this file?
    This program contains the various ECE_Matrix class member functions
    that are implemented in ECE_Matrix.cpp
*/

#pragma once
#include <sstream>
#include <algorithm>

class ECE_Matrix
{
public:

    // constructors
    ECE_Matrix();
    ECE_Matrix(int size, double values);
    ECE_Matrix(int rows, int cols, double values);

    explicit ECE_Matrix(const std::string& filename);

    // copy constructor
    ECE_Matrix(const ECE_Matrix& m2);

    // () overload for returning specific element value
    double operator()(int row, int col) const;

    // getter functions for output stream
    int get_m_rows() const;
    int get_m_cols() const;

    // matrix algebra
    const ECE_Matrix operator+(const ECE_Matrix& m2) const;
    const ECE_Matrix operator-(const ECE_Matrix& m2) const;

    // assigment operator
    ECE_Matrix& operator=(const ECE_Matrix& m2);

    // double on right side algebra
    const ECE_Matrix operator+(double rhs) const;
    const ECE_Matrix operator-(double rhs) const;
    const ECE_Matrix operator*(double rhs) const;
    const ECE_Matrix operator/(double rhs) const;

    // double on left side algebra
    friend ECE_Matrix operator+(double lhs, const ECE_Matrix& rhs);
    friend ECE_Matrix operator-(double lhs, const ECE_Matrix& rhs);
    friend ECE_Matrix operator*(double lhs, const ECE_Matrix& rhs);

    // transpose function
    const ECE_Matrix transpose() const;

    // overload for += and -= operators
    ECE_Matrix operator+=(const ECE_Matrix& rhs);
    ECE_Matrix operator-=(const ECE_Matrix& rhs);

    // destructor
    ~ECE_Matrix();

    // variables for ECE_Matrix
    // double** points to start of 2D array
    // int m_rows and m_cols hold values for rows and columns of a Matrix
private:
    double** m_vals;
    int m_rows;
    int m_cols;
};

// output stream for outputting correctly formatted arrays.
std::ostream& operator<<(std::ostream& os, const ECE_Matrix& m);
