/* 
 * C++ S-function that shows all the different ways to use the input ports.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfunInputs.cpp
 *
 * Then open the file "testOnputs.mdl/slx" and start the simulation.
 */

//------------------------------------------------------------------------------

#define S_FUNCTION_NAME  sfunInputs

#include "EasyLink.h"

enum inputPortName {
    IN1, IN2, IN3, IN4, IN5
};

//------------------------------------------------------------------------------

class Block : public BaseBlock {
public:

    static void initializeInputPortSizes() {
        setInputPortsCount(5);
        setInputPort(IN1, 1, 1, SS_DOUBLE);
        setInputPort(IN2, 3, 1, SS_DOUBLE);
        setInputPort(IN3, 4, 2, SS_DOUBLE);
        setInputPort(IN4, -1, 1, SS_DOUBLE);
        setInputPort(IN5, -1, -1, SS_DOUBLE);
    }

    void start() {
        printf("Starting the s-function for inputs testing...\n");
    }

    void outputs() {
        double in1 = getInputDouble(IN1);
        Array<double> in2 = getInputArray<double>(IN2);
        Array<double> in3 = getInputArray<double>(IN3);
        Array<double> in4 = getInputArray<double>(IN4);
        Array<double> in5 = getInputArray<double>(IN5);

        printf("---------- time = %f ----------\n", getSimulationTime());
        printf("input port %i = %f\n", IN1, in1);
        in2.print();
        in3.print();
        in4.print();
        in5.print();
    }

    void terminate() {
        printf("Terminating the s-function for inputs testing.\n");
    }

};

//------------------------------------------------------------------------------

#include "sfunDefinitions.h"

//------------------------------------------------------------------------------
