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

//------------------------------------------------------------------------------
// Definition of the gateway function for MEX functions

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    try {
        Function::nrhs = nrhs;
        Function::prhs = &(prhs[0]);
        Function::nlhs = nlhs;
        Function::plhs = &(plhs[0]);
        Function::checkInputPortSizes();
        Function::initializeOutputPortSizes();
        Function::computeOutputs();
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        mexErrMsgIdAndTxt("Error in mex function", ERROR_MSG_BUFFER);
        return;
    }
}
