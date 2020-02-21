/*
Author: Shlok Patel
Class: ECE4122
Last Date Modified: 10/6/2019
Description: Homework 3 Problem 1: Lattice Paths
What is the purpose of this file?
 This file takes in the height and width of a lattice, and calculates
 the total number of possible routes from the top left origin (0,0) to
 the bottom right (Width, Height) corner. My program uses a combinatoric
 approach to this problem, with the final result being calculated using the
 formula (Width + Height)!/((Width!)(Height!)).
*/

#include <iostream>
#include <iomanip>
#include <string>

// function for calculating factorial of input value
long double factorial(int input)
{
    // long double return type to handle max input case of 40!
    long double factorial = 1;

    // using openMP for dynamic scheduling to split for loop execution into multiple chunks that are run in any order
    #pragma omp for schedule(dynamic)
    for(int i = 1; i <= input; ++i)
    {
        factorial *= i;     // using *= to calculate factorial
    }
    return factorial;
}

// int main() set up for command line arguments
int main(int argc, char* argv[])
{
    // convert command line arguments strings to integers using atoi function
    int height = atoi(argv[2]);
    int width = atoi(argv[4]);
    // calculate total number of routes using combinatoric approach
    long double result = factorial(height + width)/(factorial(width)*factorial(height));
    // output number of routes in standard notation with no decimal places
    std::cout << std::fixed << std::setprecision(0) << "Number of Routes: " << result << std::endl;
    return 0;
}
