/*
 * C++ S-function that shows all the different ways to use the matlab arrays.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfunMatlabArrays.cpp
 *
 * Then open the file "testMatlabArrays.mdl/slx" and start the simulation.
 */

//------------------------------------------------------------------------------

#define S_FUNCTION_NAME  sfunMatlabArrays

#include "EasyLink.h"

//------------------------------------------------------------------------------

class Block : public BaseBlock {
private:

    Array<double> myArray;

public:

    void start() {
        printf("Starting the s-function for matlab array testing...\n");
        Array<double> test = newMatlabArray(2, 2, "test");
        test(0, 0) = 2;
        test(1, 1) = 4;
        myArray = test;
    }

    void outputs() {
        printf("---------- time = %f ----------\n", getSimulationTime());

        Array<double> test = getMatlabArray("test");
        test += 1;
        test.print();

        myArray += 1;
        myArray.print();

        Array<double> a(2, 2, "A");
        a(0, 0) = -3;
        a(1, 1) = 4;
        a.print();

        Array<double> b;
        b = callMatlab<double>("inv", test);
        b.print();

        Array<double> c = a * 3;
        c.print();
    }

    void terminate() {
        printf("Terminating the s-function for matlab array testing.\n");
    }

};

//------------------------------------------------------------------------------

#include "sfunDefinitions.h"

//------------------------------------------------------------------------------
