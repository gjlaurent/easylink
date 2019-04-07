/* 
 * C++ S-function for multiplying an input by 2
 *
 *   y = 2*u
 *
 * Input and output ports are dynamically sized.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfunTimesTwo.cpp
 *
 * Then open the file "testTimesTwo.mdl/slx" and start the simulation.
 */

//------------------------------------------------------------------------------

#define S_FUNCTION_NAME  sfunTimesTwo

#include "EasyLink.h"

//------------------------------------------------------------------------------

class Block : public BaseBlock {
public:

    static void initializeInputPortSizes() {
        setInputPortsCount(1);
        setInputPort(0, -1, -1, SS_DOUBLE);
    }

    static void initializeOutputPortSizes() {
        setOutputPortsCount(1);
        setOutputPort(0, -1, -1, SS_DOUBLE);
    }

    static void checkInputPortFinalSizes(int port, int nRows, int nCols) {
        if (port == 0) {
            setOutputPortFinalSizes(0, nRows, nCols);
        }
    }

    void outputs() {
        Array<double> in = getInputArray<double>(0);
        Array<double> out = getOutputArray<double>(0);
        out = in * 2.0;
    }

};

//------------------------------------------------------------------------------

#include "sfunDefinitions.h"

//------------------------------------------------------------------------------
