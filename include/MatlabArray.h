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

#ifndef EASYLINK_MATLABARRAY_H
#define EASYLINK_MATLABARRAY_H

#include "Array.h"

/** \ingroup matlabArray
 * Returns an Array connected to a MATLAB variable in a given workspace.
 * Works only with double array.
 * Values for workspace are: "base", "caller" or "global".
 */
inline Array<double> getMatlabArray(std::string name, std::string workspace = "base") {
    const mxArray *mxarray;

    mxarray = mexGetVariablePtr(workspace.c_str(), name.c_str());
    if (!mxarray)
        throw std::runtime_error("EasyLink error: Unable to find a variable named " + name + ".");

    return Array<double>(mxarray, name, true);
}

/** \ingroup matlabArray
 * Returns true if a MATLAB variable exists with the given name in a given workspace.
 * Works only with double array.
 * Values for workspace are: "base", "caller" or "global".
 */
inline bool existMatlabArray(std::string name, std::string workspace = "base") {
    const mxArray *mxarray;

    mxarray = mexGetVariablePtr(workspace.c_str(), name.c_str());

    return (mxarray != NULL);
}

/** \ingroup matlabArray
 * Create a new variable in a given MATALB workspace ("base" is the
 * default workspace) and returns an Array connected this variable.
 * Values for workspace are: "base", "caller" or "global".
 */
inline Array<double> newMatlabArray(int nrows, int ncols, std::string name, std::string workspace = "base") {
    mxArray *mxarray;

    mxarray = mxCreateDoubleMatrix(nrows, ncols, mxREAL);
    if (!mxarray)
        throw std::runtime_error("EasyLink error: Unable to create a double array named " + name + ".");

    if (mexPutVariable(workspace.c_str(), name.c_str(), mxarray))
        throw std::runtime_error("EasyLink error: Unable to put variable " + name + " in workspace " + workspace + ".");

    mxDestroyArray((mxArray*) mxarray);

    return getMatlabArray(name, workspace);
}

/** Call internal MATLAB numeric functions, MATLAB operators,
 * or user-defined functions and applied it to the array. */
template<typename _Scalar>
Array<_Scalar> callMatlab(std::string cmd, Array<_Scalar> & operand1) {
    mxArray *input, *output;

    if (operand1.getmxArray() == NULL) {
        throw std::runtime_error("EasyLink error: Unable to call Matlab with array " + operand1.getName() + ".");
    } else {
        input = operand1.getmxArray();
    }
    mexCallMATLAB(1, &output, 1, &input, cmd.c_str());

    Array<_Scalar> result(output, cmd + "(" + operand1.getName() + ")", true, true);
    mxDestroyArray(output);
    
    return result;
}

/** Call internal MATLAB numeric functions, MATLAB operators,
 * or user-defined functions and applied it to the array and to a second
 * operand. */
template<typename _Scalar>
Array<_Scalar> callMatlab(std::string cmd, Array<_Scalar> & operand1, Array<_Scalar> & operand2) {
    mxArray * input[2], *output;

    if (operand1.getmxArray() == NULL) {
        throw std::runtime_error("EasyLink error: Unable to call Matlab with array " + operand1.getName() + ".");
    } else {
        input[0] = operand1.getmxArray();
    }
    if (operand2.getmxArray() == NULL) {
        throw std::runtime_error("EasyLink error: Unable to call Matlab with array " + operand2.getName() + ".");
    } else {
        input[1] = operand2.getmxArray();
    }
    mexCallMATLAB(1, &output, 2, input, cmd.c_str());

    Array<_Scalar> result(output, cmd + "(" + operand1.getName() + "," + operand2.getName() + ")", true, true);
    mxDestroyArray(output);

    return result;
}

#endif
