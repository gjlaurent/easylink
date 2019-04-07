/* 
 * C++ S-function for multiplying an input by 2 using EasyLink and Eigen
 *
 *   y = 2*u
 *
 * Input and output ports are dynamically sized.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfunTimesTwoWithEigen.cpp
 *
 * Then open the file "testTimesTwoWithEigen.mdl/slx" and start the simulation.
 */

//------------------------------------------------------------------------------

#define S_FUNCTION_NAME  sfunTimesTwoWithEigen

#include "EasyLink.h"

#include <Eigen/Dense>

#define EIGEN_INPUT_MATRIX(type, name, port) Eigen::Map<Eigen::Matrix<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getInputData(port), getInputNRows(port), getInputNCols(port))
#define EIGEN_INPUT_ARRAY(type, name, port) Eigen::Map<Eigen::Array<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getInputData(port), getInputNRows(port), getInputNCols(port))
#define EIGEN_OUTPUT_MATRIX(type, name, port) Eigen::Map<Eigen::Matrix<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getOutputData(port), getOutputNRows(port), getOutputNCols(port))
#define EIGEN_OUTPUT_ARRAY(type, name, port) Eigen::Map<Eigen::Array<type, Eigen::Dynamic, Eigen::Dynamic> > name((type*)getOutputData(port), getOutputNRows(port), getOutputNCols(port))


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
        EIGEN_INPUT_ARRAY(double, in, 0);
        EIGEN_OUTPUT_ARRAY(double, out, 0);
        out = in * 2.0;
    }

};

//------------------------------------------------------------------------------

#include "sfunDefinitions.h"

//------------------------------------------------------------------------------
