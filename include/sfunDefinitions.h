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
#define MDL_CHECK_PARAMETERS

static void mdlCheckParameters(SimStruct *S) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlCheckParameters ------------------------------\n");
#endif
    try {
        Block::setSimStruct(S);
        Block::checkParametersSizes();
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
}

//------------------------------------------------------------------------------
#define MDL_INITIALIZE_SIZES

static void mdlInitializeSizes(SimStruct *S) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlInitializeSizes ------------------------------\n");
#endif
    try {
        Block::setSimStruct(S);
        //Block::initializeParameterPortSizes();
        Block::checkParametersSizes();
        Block::initializeInputPortSizes();
        Block::initializeOutputPortSizes();
        Block::initializeStatePortSizes();
        ssSetNumRWork(S, 0);
        ssSetNumIWork(S, 0);
        ssSetNumPWork(S, 1);
        Block::initializeNumberSampleTimes();
        Block::initializeOptions();
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
}

//---------------------------------------------------------------------------
#define MDL_SET_INPUT_PORT_DIMENSION_INFO

static void mdlSetInputPortDimensionInfo(SimStruct *S, int port, const DimsInfo_T *dimsInfo) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlSetInputPortDimensionInfo --------------------\n");
#endif
    try {
        Block::setSimStruct(S);
        if (dimsInfo->numDims == 1) {
            Block::checkInputPortFinalSizes(port, dimsInfo->width, 1);
        } else if (dimsInfo->numDims == 2) {
            Block::checkInputPortFinalSizes(port, dimsInfo->dims[0], dimsInfo->dims[1]);
        } else {
            throw std::runtime_error("Input port dimensions greater than two are not supported by easylink.");
        }
        if (!ssSetInputPortDimensionInfo(S, port, dimsInfo)) return;
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
}

//---------------------------------------------------------------------------
#define MDL_SET_OUTPUT_PORT_DIMENSION_INFO

static void mdlSetOutputPortDimensionInfo(SimStruct *S, int port, const DimsInfo_T *dimsInfo) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlSetOutputPortDimensionInfo -------------------\n");
#endif
    try {
        Block::setSimStruct(S);
        if (dimsInfo->numDims == 1) {
            Block::checkOutputPortFinalSizes(port, dimsInfo->width, 1);
        } else if (dimsInfo->numDims == 2) {
            Block::checkOutputPortFinalSizes(port, dimsInfo->dims[0], dimsInfo->dims[1]);
        } else {
            throw std::runtime_error("Output port dimensions greater than two are not supported by easylink.");
        }
        if (!ssSetOutputPortDimensionInfo(S, port, dimsInfo)) return;
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
}

//------------------------------------------------------------------------------
#define MDL_INITIALIZE_SAMPLE_TIME

static void mdlInitializeSampleTimes(SimStruct *S) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlInitializeSampleTimes ------------------------\n");
#endif
    try {
        Block::setSimStruct(S);
        Block::initializeSampleTimes();
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
}

//------------------------------------------------------------------------------
#define MDL_START

static void mdlStart(SimStruct *S) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlStart ----------------------------------------\n");
#endif
    Block *block = new Block;
    ssGetPWork(S)[0] = (void *) block;
    try {
        Block::setSimStruct(S);
        block->start();
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
}

//------------------------------------------------------------------------------
#define MDL_OUTPUTS

static void mdlOutputs(SimStruct *S, int tid) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlOutputs at time %f ---------------------------\n", Block::getSimulationTime());
#endif
    Block *block = (Block *) ssGetPWork(S)[0];
    try {
        Block::setSimStruct(S);
        block->outputs();
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
}

//------------------------------------------------------------------------------
#define MDL_DERIVATIVES

static void mdlDerivatives(SimStruct *S) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlDerivatives ----------------------------------\n");
#endif
    Block *block = (Block *) ssGetPWork(S)[0];
    try {
        Block::setSimStruct(S);
        block->derivatives();
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
}

//------------------------------------------------------------------------------
#define MDL_ZERO_CROSSINGS

static void mdlZeroCrossings(SimStruct *S) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlZeroCrossings --------------------------------\n");
#endif
    Block *block = (Block *) ssGetPWork(S)[0];
    try {
        Block::setSimStruct(S);
        block->zeroCrossings();
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
}

//------------------------------------------------------------------------------
#define MDL_UPDATE

static void mdlUpdate(SimStruct *S, int tid) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlUpdate ---------------------------------------\n");
#endif
    Block *block = (Block *) ssGetPWork(S)[0];
    try {
        Block::setSimStruct(S);
        block->update();
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
}

//------------------------------------------------------------------------------
#define MDL_TERMINATE

static void mdlTerminate(SimStruct *S) {
#ifdef __TEST__
    printf("EasyLink test message: entering mdlTerminate ------------------------------------\n");
#endif
    Block *block = (Block *) ssGetPWork(S)[0];
    try {
        Block::setSimStruct(S);
        block->terminate();
        delete block;
    } catch (std::exception const& e) {
        strcpy(ERROR_MSG_BUFFER, e.what());
        ssSetErrorStatus(S, ERROR_MSG_BUFFER);
        return;
    }
#ifdef __TEST__
    printf("EasyLink test message: allocation number = %i.\n", Array<double>::allocationNumber);
#endif
}

//------------------------------------------------------------------------------
#ifdef  MATLAB_MEX_FILE
#include "simulink.c"
#else
#include "cg_sfun.h"
#endif
//------------------------------------------------------------------------------
