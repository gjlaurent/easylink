/* 
 * This file illustrates how to use Eigen within a C++ MEX-File and EasyLink.
 *
 * The function multiplies an input scalar times an input NxM ArrayXd and 
 * outputs a NxM ArrayXd.
 *
 * The calling syntax is:
 *
 *     outArray = mexArrayProductWithEigen(multiplier, inArray)
 *
 * To compile this C++ MEX-File, enter the following command in MATLAB:
 *
 *     >>make mexArrayProductWithEigen.cpp
 *
 */

//------------------------------------------------------------------------------

#include "EasyLink.h"

#include <Eigen/Dense>

#define EIGEN_INPUT_MATRIX(type, name, port) Eigen::Map<Eigen::Matrix<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getInputData(port), getInputNRows(port), getInputNCols(port))
#define EIGEN_INPUT_ARRAY(type, name, port) Eigen::Map<Eigen::Array<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getInputData(port), getInputNRows(port), getInputNCols(port))
#define EIGEN_OUTPUT_MATRIX(type, name, port) Eigen::Map<Eigen::Matrix<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getOutputData(port), getOutputNRows(port), getOutputNCols(port))
#define EIGEN_OUTPUT_ARRAY(type, name, port) Eigen::Map<Eigen::Array<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getOutputData(port), getOutputNRows(port), getOutputNCols(port))

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

    // Calculates the function using Eigen

    static void outputs() {
        double multiplier = getInputDouble(0);
        EIGEN_INPUT_ARRAY(double, inArray, 1);
        EIGEN_OUTPUT_ARRAY(double, outArray, 0);
        outArray = inArray*multiplier;
    }

};

//------------------------------------------------------------------------------

#include "mexDefinitions.h"

//------------------------------------------------------------------------------