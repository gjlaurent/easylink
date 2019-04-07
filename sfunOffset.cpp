/* 
 * This file illustrates how to construct a simple C++ S-function with
 * EasyLink.  The S-function has one scalar input, one scalar output and one
 * scalar parameter.  The output of the S-function is the sum of its input and
 * of the parameter,
 *
 *   y = u + p
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfunOffset.cpp
 *
 * Then open the file "testOffset.mdl/slx" and start the simulation.
 */

//------------------------------------------------------------------------------

#define S_FUNCTION_NAME  sfunOffset

#include "EasyLink.h"

//------------------------------------------------------------------------------

class Block : public BaseBlock {
public:

    static void checkParametersSizes() {
        assertParameterPortsCount(1);
        assertParameterPort(0, true, 1, 1, mxDOUBLE_CLASS);
    }

    static void initializeInputPortSizes() {
        setInputPortsCount(1);
        setInputPort(0, 1, 1, SS_DOUBLE);
    }

    static void initializeOutputPortSizes() {
        setOutputPortsCount(1);
        setOutputPort(0, 1, 1, SS_DOUBLE);
    }

    void start() {
    }

    void outputs() {
        double in = getInputDouble(0);
        double offset = getParameterDouble(0);

        setOutputDouble(0, in + offset);
    }

    void terminate() {
    }

};

//------------------------------------------------------------------------------

#include "sfunDefinitions.h"

//------------------------------------------------------------------------------
