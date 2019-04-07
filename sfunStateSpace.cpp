//------------------------------------------------------------------------------
/* C++ S-function for defining a continuous system with EasyLink and Eigen.
 *
 *    x' = Ax + Bu
 *    y  = Cx + Du
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfunStateSpace.cpp
 *
 * Then open the file "testStateSpace.mdl/slx" and start the simulation.
 */
//------------------------------------------------------------------------------
#define S_FUNCTION_NAME  sfunStateSpace

enum inputPortName {
    U
};

enum outputPortName {
    Y
};

enum parameterName {
    A, B, C, D
};


//------------------------------------------------------------------------------
#include "EasyLink.h"

#include <Eigen/Dense>

#define EIGEN_INPUT_MATRIX(type, name, port) Eigen::Map<Eigen::Matrix<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getInputData(port), getInputNRows(port), getInputNCols(port))
#define EIGEN_OUTPUT_MATRIX(type, name, port) Eigen::Map<Eigen::Matrix<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getOutputData(port), getOutputNRows(port), getOutputNCols(port))
#define EIGEN_PARAMETER_MATRIX(type, name, port) name= Eigen::Map<Eigen::Matrix<type, Eigen::Dynamic, Eigen::Dynamic> >((type*)getParameterData(port), getParameterNRows(port), getParameterNCols(port))
#define EIGEN_STATE_MATRIX(type, name) Eigen::Map<Eigen::Matrix<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getContinuousStateData(), getContinuousStateWidth(), 1)
#define EIGEN_DERIVATIVE_STATE_MATRIX(type, name) Eigen::Map<Eigen::Matrix<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getDerivativeStateData(), getContinuousStateWidth(), 1)


//------------------------------------------------------------------------------

class Block : public BaseBlock {
private:

    Eigen::MatrixXd a, b, c, d;

public:

    static void checkParametersSizes() {
        assertParameterPortsCount(4);
        assertParameterPort(A, false, -1, -1, mxDOUBLE_CLASS);
        assertParameterPort(B, false, -1, -1, mxDOUBLE_CLASS);
        assertParameterPort(C, false, -1, -1, mxDOUBLE_CLASS);
        assertParameterPort(D, false, -1, -1, mxDOUBLE_CLASS);
    }

    static void initializeInputPortSizes() {
        setInputPortsCount(1);
        setInputPort(U, getParameterNCols(B), 1, SS_DOUBLE);
    }

    static void initializeOutputPortSizes() {
        setOutputPortsCount(1);
        setOutputPort(Y, getParameterNRows(C), 1, SS_DOUBLE);
    }

    static void initializeStatePortSizes() {
        setContinuousStatesWidth(getParameterNCols(A));
        setDiscreteStatesWidth(0);
    }

    static void initializeSampleTimes() {
        ssSetSampleTime(simStruct, 0, CONTINUOUS_SAMPLE_TIME);
        ssSetOffsetTime(simStruct, 0, 0.0);
        //        ssSetModelReferenceSampleTimeDefaultInheritance(simStruct);
    }

    void start() {
        EIGEN_PARAMETER_MATRIX(double, a, A);
        EIGEN_PARAMETER_MATRIX(double, b, B);
        EIGEN_PARAMETER_MATRIX(double, c, C);
        EIGEN_PARAMETER_MATRIX(double, d, D);
    }

    void outputs() {
        EIGEN_INPUT_MATRIX(double, u, U);
        EIGEN_OUTPUT_MATRIX(double, y, Y);
        EIGEN_STATE_MATRIX(double, x);
        
        y = c * x + d*u;
    }

    void derivatives() {
        EIGEN_INPUT_MATRIX(double, u, U);
        EIGEN_STATE_MATRIX(double, x);
        EIGEN_DERIVATIVE_STATE_MATRIX(double, dx);

        dx = a * x + b*u;
    }

};

//------------------------------------------------------------------------------
#include "sfunDefinitions.h"

//------------------------------------------------------------------------------
