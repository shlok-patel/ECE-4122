/*
Author: Shlok Patel
Class: ECE4122
Last Date Modified: 9/18/2019
Description: Homework Problem 2: Largest Product in a Grid
What is the purpose of this file?
    This program finds the largest product of any 4 adjacent elements
    in an input text file and outputs the result to a text file.
    The ECE_Matrix class used for problem 1 is reused here for the
    Matrix constructor from the input file.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include "ECE_Matrix.h"

int main(int argc, char* argv[])
{
    std::string f = argv[1];
    ECE_Matrix M1(f); // Construct matrix from file

    double result = 0; // stores max product

    // iterate through each row of input matrix
    for (int i = 0; i < M1.get_m_rows(); i++)
    {
        // iterate through each column of input matrix
        for (int j = 0; j < M1.get_m_cols(); j++)
        {
            // placeholder values for products in 4 potential directions,
            //  1) left to right
            //  2) top to bottom
            //  3) top left to bottom right
            //  4) top right to bottom left
            // the inverses of these 4 directions are not needed for this algorithm
            double LR = 1, TB = 1, TLBR = 1, TRBL = 1;

            // check 4 elements offset from current position
            for (int k = 0; k < 4; k++)
            {
                // check if matrix offset is within bounds of matrix
                if (i + k < M1.get_m_rows() && j + k < M1.get_m_cols())
                {
                    LR *= M1(i, j + k); // find product of 4 elements left to right (line)
                    TB *= M1(i + k, j); // find product of 4 elements top to bottom (line)
                    TLBR *= M1(i + k, j + k); // find product of 4 elements top left to bottom right (diagonal)
                    TRBL *= M1(i + k, j - k); // find product of 4 elements top right to bottom left (diagonal)
                }
            }
            // set max result to max of the 4 potential directions and the previous max
            result = std::max({LR, TB, TLBR, TRBL, result});
        }
    }

    // output final result to "output2.txt" file
    std::ofstream output;
    output.open("output2.txt");
    output << std::setprecision(16) << result;
    output.close();
    return 0;
}