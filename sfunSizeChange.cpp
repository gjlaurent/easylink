/* 
 * C++ S-function that changes the size of an output according to the
 * value of a parameter.
 *
 *   y = [ u  u  u ... u ]'
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfunSizeChange.cpp
 *
 * Then open the file "testSizeChange.mdl/slx" and start the simulation.
 */

//------------------------------------------------------------------------------

#define S_FUNCTION_NAME sfunSizeChange

#include "EasyLink.h"

//------------------------------------------------------------------------------

class Block : public BaseBlock {
public:

    static void checkParametersSizes() {
        assertParameterPortsCount(1);
        assertParameterPort(0, false, 1, 1, mxINT32_CLASS);
    }

    static void initializeInputPortSizes() {
        setInputPortsCount(1);
        setInputPort(0, 1, 1, SS_DOUBLE);
    }

    static void initializeOutputPortSizes() {
        setOutputPortsCount(1);
        int nRows = getParameterScalar<int>(0);
        setOutputPort(0, 1, nRows, SS_DOUBLE);
    }

    void outputs() {
        double in = getInputDouble(0);
        Array<double> out = getOutputArray<double>(0);

        out.init(in);
    }

};

//------------------------------------------------------------------------------

#include "sfunDefinitions.h"

//------------------------------------------------------------------------------
