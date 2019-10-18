/* 
 * This file is part of EasyLink Library.
 * 
 * Copyright (c) 2014 FEMTO-ST, ENSMM, UFC, CNRS.
 * 
 * License: GNU General Public License 3
 * 
 * Author: Guillaume J. Laurent
 * 
 */

#ifndef EASYLINK_BASEFUNCTION_H
#define EASYLINK_BASEFUNCTION_H

#include "Array.h"

/** BaseFunction is the basis class for designing new C++ MEX functions.
 *
 *  Use the file mex_array_product.cpp as a template to write a new function.
 */
class BaseFunction {
public:

    static int nrhs;
    const static mxArray **prhs;
    static int nlhs;
    static mxArray **plhs;

    /**
     * This method allows to check the number of input ports (right-side  
     * arguments).
     */
    static void checkInputPortsCount(int portsCount) {
        if (nrhs != portsCount)
            throw std::runtime_error(toString(portsCount) + " input arguments are required.");
    }

    /**
     * This method allows to check the dimensions and the type of an input 
     * port (right-side argument).
     * 
     * Use -1 to specify dynamically dimensioned intput arrays.
     */
    static void checkInputPort(int port, int nRows, int nCols, mxClassID type = mxDOUBLE_CLASS, mxComplexity complexFlag = mxREAL) {
        const mxArray* mxarray = prhs[port];
        if (mxGetClassID(mxarray) != type) {
            throw std::runtime_error("Input argument " + toString(port) + " has a wrong type.");
        }
        if (mxIsSparse(mxarray)) {
            throw std::runtime_error("Input argument " + toString(port) + " must not be sparse.");
        }
        if (mxIsComplex(mxarray)) {
            throw std::runtime_error("Input argument " + toString(port) + " must not be complex.");
        }
        if (nRows > 0 && mxGetM(mxarray) != nRows) {
            throw std::runtime_error("Input argument " + toString(port) + " must have " + toString(nRows) + " rows.");
        }
        if (nCols > 0 && mxGetN(mxarray) != nCols) {
            throw std::runtime_error("Input argument " + toString(port) + " must have " + toString(nCols) + " cols.");
        }
    }

    /**
     * This is the first static method that is called within the MEX-Function.
     *
     * This method must:
     *     1) specify the number of input ports (right-side arguments)
     *     2) specify the dimensions and the type of each intput port 
     * 
     * This method must be overridded in your custom function.
     */
    static void checkInputPortSizes() {
    }

    /**
     * This method allows to check the number of output ports (left-side 
     * arguments).
     */
    static void checkOutputPortsCount(int portsCount) {
        if (nlhs != portsCount)
            throw std::runtime_error(toString(portsCount) + " output arguments are required.");
    }

    /**
     * This method allows to set the dimensions and the type of an output 
     * port (left-side argument).
     */
    static void setOutputPort(int port, int nRows, int nCols, mxClassID type = mxDOUBLE_CLASS, mxComplexity complexFlag = mxREAL) {
        if (nRows > 0 && nCols > 0) {
            mwSize dims[2];
            dims[0] = nRows;
            dims[1] = nCols;
            plhs[port] = mxCreateNumericArray(2, dims, type, complexFlag);
        }
    }

    /**
     * This is the second static method that is called within the MEX-Function.
     *
     * This method must:
     *     1) specify the number of output ports (left-side arguments)
     *     2) specify the dimensions and the type of each output port 
     * 
     * This method must be overridded in your custom function.
     */
    static void initializeOutputPortSizes() {
    }

    /**
     * This is the third static method that is called within the MEX-Function.
     *
     * The method have to compute the function's outputs for given inputs
     * and to store the results using getOutput or using writeOutput.
     * 
     * This method must be overridded in your custom function.
     */
    static void computeOutputs() {
    }

    /**
     * Returns the number of input ports (right-side arguments).
     */
    static inline double getInputPortsCount() {
        return nrhs;
    }

    /**
     * Returns the double scalar value of an input port (right-side argument).
     */
    static inline double getInputDouble(int port) {
        if (port < 0 || port >= nrhs)
            throw std::runtime_error("Input argument " + toString(port) + " does not exist.");
        return mxGetScalar(prhs[port]);
    }

    /**
     * Returns the scalar value of an input port (right-side argument).
     */
    template<typename _Scalar>
    static inline _Scalar getInputScalar(int port) {
        if (port < 0 || port >= nrhs)
            throw std::runtime_error("Input argument " + toString(port) + " does not exist.");
        return *((_Scalar*) mxGetPr(prhs[port]));
    }

    /**
     * Returns an array mapping an input port (right-side argument).
     */
    template<typename _Scalar>
    static inline Array<_Scalar> getInputArray(int port) {
        if (port < 0 || port >= nrhs)
            throw std::runtime_error("Input argument " + toString(port) + " does not exist.");
        return Array<_Scalar>(prhs[port], "input port " + toString(port), true);
    }

    /**
     * Returns the string value of an input port (right-side argument).
     */
    static inline std::string getInputString(int port) {
        if (port < 0 || port >= nrhs)
            throw std::runtime_error("Input argument " + toString(port) + " does not exist.");
        char buffer[256];
        mxGetString(prhs[port], buffer, 256);
        return std::string(buffer);
    }

    /**
     * Returns a pointer to the first element of the input port data 
     * (right-side argument).
     */
    static inline void* getInputData(int port) {
        if (port < 0 || port >= nrhs)
            throw std::runtime_error("Input argument " + toString(port) + " does not exist.");
        return (void*) mxGetData(prhs[port]);
    }

    /**
     * Returns the number of elements of an input port (right-side argument).
     * 
     * If the input port is a 1-D array with w elements, this function returns 
     * w. 
     * 
     * If the input port is an M-by-N array, this function returns m*n.
     */
    static inline int getInputWidth(int port) {
        return getInputNRows(port) * getInputNCols(port);
    }

    /**
     * Returns the number of rows of an input port (right-side argument).
     */
    static inline int getInputNRows(int port) {
        return mxGetM(prhs[port]);
    }

    /**
     * Returns the number of cols of an input port (right-side argument).
     */
    static inline int getInputNCols(int port) {
        return mxGetN(prhs[port]);
    }

    /**
     * Returns the number of output ports (left-side arguments).
     */
    static inline double getOuputPortsCount() {
        return nlhs;
    }

    /**
     * Writes a double value to an output port (left-side argument).
     */
    static inline void setOutputDouble(int port, double value) {
        if (port < 0 || port >= nlhs)
            throw std::runtime_error("Output argument " + toString(port) + " does not exist.");
        double *x = mxGetPr(plhs[port]);
        x[0] = value;
    }

    /**
     * Writes a scalar value to an output port (left-side argument).
     */
    template<typename _Scalar>
    static inline void setOutputScalar(int port, _Scalar value) {
        if (port < 0 || port >= nlhs)
            throw std::runtime_error("Output argument " + toString(port) + " does not exist.");
        _Scalar *x = mxGetPr(plhs[port]);
        x[0] = value;
    }

    /**
     * Returns an array mapping an output port (left-side argument).
     */
    template<typename _Scalar>
    static inline Array<_Scalar> getOutputArray(int port) {
        if (port < 0 || port >= nlhs)
            throw std::runtime_error("Output argument " + toString(port) + " does not exist.");
        return Array<_Scalar>(plhs[port], "output port " + toString(port), true);
    }

    /**
     * Returns a pointer to the first element of the output port data 
     * (left-side argument).
     */
    static inline void* getOutputData(int port) {
        if (port < 0 || port >= nlhs)
            throw std::runtime_error("Output argument " + toString(port) + " does not exist.");
        return mxGetData(plhs[port]);
    }

    /**
     * Returns the number of elements of an output port (left-side argument).
     * 
     * If the output port is a 1-D array with w elements, this function returns 
     * w.
     * 
     * If the output port is an M-by-N array, this function returns m*n.
     */
    static inline int getOutputWidth(int port) {
        return getOutputNRows(port) * getOutputNCols(port);
    }

    /**
     * Returns the number of rows of an output port (left-side argument).
     */
    static inline int getOutputNRows(int port) {
        return mxGetM(plhs[port]);
    }

    /**
     * Returns the number of cols of an output port (left-side argument).
     */
    static inline int getOutputNCols(int port) {
        return mxGetN(plhs[port]);
    }

};

int BaseFunction::nrhs = 0;
const mxArray** BaseFunction::prhs = NULL;
int BaseFunction::nlhs = 0;
mxArray** BaseFunction::plhs = NULL;

#endif
