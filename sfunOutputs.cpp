/* 
 * C++ S-function that shows all the different ways to use the output ports.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfunOutputs.cpp
 *
 * Then open the file "testOutputs.mdl/slx" and start the simulation.
 */

//------------------------------------------------------------------------------

#define S_FUNCTION_NAME  sfunOutputs

#include "EasyLink.h"

enum outputPortName {
    OUT1, OUT2, OUT3
};

//------------------------------------------------------------------------------

class Block : public BaseBlock {
public:

    static void initializeOutputPortSizes() {
        setOutputPortsCount(3);
        setOutputPort(OUT1, 1, 1, SS_DOUBLE);
        setOutputPort(OUT2, 3, 1, SS_DOUBLE);
        setOutputPort(OUT3, 2, 3, SS_INT32);
    }

    void start() {
        printf("Starting the s-function for outputs testing...\n");
    }

    void outputs() {
        Array<double> out2 = getOutputArray<double>(OUT2);
        Array<int> out3 = getOutputArray<int>(OUT3);

        printf("---------- time = %f ----------\n", getSimulationTime());
        printf("output port %i = 7\n", OUT1);
        setOutputDouble(OUT1, 7);

        out2.init(1);
        out2[1] = 2;
        out2.print();

        out3.init(3);
        out3(0, 2) = 4;
        out3.print();

    }

    void terminate() {
        printf("Terminating the s-function for outputs testing.\n");
    }

};

//------------------------------------------------------------------------------

#include "sfunDefinitions.h"

//------------------------------------------------------------------------------
