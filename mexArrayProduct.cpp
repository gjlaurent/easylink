/* 
 * This file illustrates how to construct a simple C++ MEX-File with
 * EasyLink.
 *
 * The function multiplies an input scalar times an input NxM array and outputs 
 * a NxM array.
 *
 * The calling syntax is:
 *
 *     outArray = mexArrayProduct(multiplier, inArray)
 *
 * To compile this C++ MEX-File, enter the following command in MATLAB:
 *
 *     >>make mexArrayProduct.cpp
 *
 */

//------------------------------------------------------------------------------

#include "EasyLink.h"

//------------------------------------------------------------------------------

class Function : public BaseFunction {
public:

    // Specifies the number and the sizes of the input ports of the function 
    // (right-side arguments)

    static void initializeInputPortSizes() {
        setInputPortsCount(2);
        setInputPort(0, 1, 1, mxDOUBLE_CLASS);
        setInputPort(1, -1, -1, mxDOUBLE_CLASS);
    }

    // Specifies the number and the sizes of the output ports of the function
    // (right-side arguments)

    static void initializeOutputPortSizes() {
        setOutputPortsCount(1);
        setOutputPort(0, getInputNRows(1), getInputNCols(1), mxDOUBLE_CLASS);
    }

    // Calculates the function

    static void outputs() {
        double multiplier = getInputDouble(0);
        Array<double> inArray = getInputArray<double>(1);
        Array<double> outArray = getOutputArray<double>(0);
        outArray = inArray*multiplier;
    }

};

//------------------------------------------------------------------------------

#include "mexDefinitions.h"

//------------------------------------------------------------------------------
