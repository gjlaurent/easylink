/* 
 * C++ S-function that shows all the different ways to use the parameters ports.
 *
 * To compile this C++ S-function, enter the following command in MATLAB:
 *
 *   >>make sfunParameters.cpp
 *
 * Then open the file "testParameters.mdl/slx" and start the simulation.
 */

//------------------------------------------------------------------------------

#define S_FUNCTION_NAME  sfunParameters

#include "EasyLink.h"

enum parameterName {
    PAR1, PAR2, PAR3, PAR4, PAR5, PAR6
};

//------------------------------------------------------------------------------

class Block : public BaseBlock {
public:

    static void checkParametersSizes() {
        assertParameterPortsCount(6);
        assertParameterPort(PAR1, true, 1, 1, mxDOUBLE_CLASS);
        assertParameterPort(PAR2, false, 2, 3, mxDOUBLE_CLASS);
        assertParameterPort(PAR3, true, 1, -1, mxDOUBLE_CLASS);
        assertParameterPort(PAR4, false, -1, -1, mxDOUBLE_CLASS);
        assertParameterPort(PAR5, true, 1, -1, mxCHAR_CLASS);
        assertParameterPort(PAR6, false, 1, -1, mxCHAR_CLASS);
    }

    void start() {
        printf("Starting the s-function for parameters testing....\n");
    }

    void outputs() {
        double par1 = getParameterDouble(PAR1);
        Array<double> par2 = getParameterArray<double>(PAR2);
        Array<double> par3 = getParameterArray<double>(PAR3);
        Array<double> par4 = getParameterArray<double>(PAR4);
        std::string par5 = getParameterString(PAR5);
        std::string par6 = getParameterString(PAR6);

        printf("---------- time = %f ----------\n", getSimulationTime());
        printf("parameter %i = %f\n", PAR1, par1);
        par2.print();
        par3.print();
        par4.print();
        printf("parameter %i = %s\n", PAR5, par5.c_str());
        printf("parameter %i = %s\n", PAR6, par6.c_str());

    }

    void terminate() {
        printf("Terminating the s-function for parameters testing.\n");
    }

};

//------------------------------------------------------------------------------

#include "sfunDefinitions.h"

//------------------------------------------------------------------------------
