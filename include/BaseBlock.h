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

#ifndef EASYLINK_BASEBLOCK_H
#define EASYLINK_BASEBLOCK_H

#include "Array.h"

/** BaseBlock is the basis class for designing new S-functions.
 *
 * Use the file sfun_offset.cpp as a template to write a new S-function.
 */
class BaseBlock {
protected:

    /** SimStruct data structure allocated for the S-function.
     * Can be used to call simulink macros. */
    static SimStruct *simStruct;

private:
    static int parameterPortsCount;
    static int inputPortsCount;
    static int outputPortsCount;

public:

    static inline void setSimStruct(SimStruct *S) {
        simStruct = S;
    }

    /**
     * This method checks the number of parameters.
     */
    static void assertParameterPortsCount(int portsCount) {
        ssSetNumSFcnParams(simStruct, portsCount);
        if (ssGetSFcnParamsCount(simStruct) != portsCount)
            throw std::runtime_error(toString(portsCount) + " parameters are expected.");
        parameterPortsCount = portsCount;
    }

    /**
     * This method checks the dimensions and the type of a parameter port.
     * 
     * This method allows also to set a parameter port not tunable. A parameter 
     * is tunable if its value can be changed by users during the simulation.
     * 
     * Use -1 to specify dynamically dimensioned parameter arrays.
     */
    static void assertParameterPort(int port, bool tunable, int nRows, int nCols, mxClassID type = mxDOUBLE_CLASS, mxComplexity complexFlag = mxREAL) {
        if (!tunable) {
            ssSetSFcnParamNotTunable(simStruct, port);
        }

        const mxArray* mxarray = ssGetSFcnParam(simStruct, port);
        if (mxIsEmpty(mxarray)) {
            throw std::runtime_error("Parameter port " + toString(port) + " is empty.");
        }
        if (mxGetClassID(mxarray) != type) {
            throw std::runtime_error("Parameter port " + toString(port) + " has a wrong type.");
        }
        if (mxIsSparse(mxarray)) {
            throw std::runtime_error("Parameter port " + toString(port) + " must not be sparse.");
        }
        if (mxIsComplex(mxarray)) {
            throw std::runtime_error("Parameter port " + toString(port) + " must not be complex.");
        }
        if (nRows > 0 && mxGetM(mxarray) != nRows) {
            throw std::runtime_error("Parameter port " + toString(port) + " must have " + toString(nRows) + " rows.");
        }
        if (nCols > 0 && mxGetN(mxarray) != nCols) {
            throw std::runtime_error("Parameter port " + toString(port) + " must have " + toString(nCols) + " cols.");
        }
    }

    /** \ingroup initialization
     * 
     * This is the first static method called before the simulation starts.
     *  
     * This static method is called with candidate parameter values.
     * 
     * This method must:
     *     1) check the number of parameters using assertParameterPortCount
     *     2) check the dimensions and the type of each parameter using assertParameterPort
     * 
     * This method must be overridded in your custom function.
     * 
     * For more information, see: https://fr.mathworks.com/help/simulink/sfg/mdlcheckparameters.html
     */
    static void checkParametersSizes() {
        assertParameterPortsCount(0);
    }

    /**
     * This method sets the number of input ports.
     */
    static void setInputPortsCount(int portsCount) {
        if (!ssSetNumInputPorts(simStruct, portsCount))
            throw std::runtime_error("Unable to set input port count to " + toString(portsCount) + ".");
        inputPortsCount = portsCount;
    }

    /**
     * This method sets the dimensions and the type of an input 
     * port.
     * 
     * Use -1 to specify dynamically dimensioned intput arrays.
     */
    static void setInputPort(int port, int nRows, int nCols, DTypeId type = SS_DOUBLE, bool directFeedThrough = true) {
        ssSetInputPortDataType(simStruct, port, type);
        if (nCols == 1) {
            ssSetInputPortWidth(simStruct, port, nRows);
        } else {
            ssSetInputPortMatrixDimensions(simStruct, port, nRows, nCols);
        }
        ssSetInputPortDirectFeedThrough(simStruct, port, directFeedThrough);
        ssSetInputPortRequiredContiguous(simStruct, port, 1);
        if (nRows < 0 || nCols < 0) {
            ssSetInputPortDimensionInfo(simStruct, port, DYNAMIC_DIMENSION);
        }
    }

    /** \ingroup initialization
     * 
     * This is the second static method called before the simulation starts.
     * 
     * This method must:
     *     1) specify the number of input ports
     *     2) specify the dimensions and the type of each input port 
     * 
     * This method must be overridded in your custom function.
     * 
     * All input ports require contiguous data.
     *
     * Use -1 to specify dynamically dimensioned input signals.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeInputPortSizes() {
        setInputPortsCount(0);
    }

    /**
     * This method sets the number of output ports.
     */
    static void setOutputPortsCount(int portsCount) {
        if (!ssSetNumOutputPorts(simStruct, portsCount))
            throw std::runtime_error("Unable to set output port count to " + toString(portsCount) + ".");
        outputPortsCount = portsCount;
    }

    /**
     * This method sets the dimensions and the type of an output 
     * port.
     * 
     * Use -1 to specify dynamically dimensioned intput arrays.
     */
    static void setOutputPort(int port, int nRows, int nCols, DTypeId type = SS_DOUBLE) {
        ssSetOutputPortDataType(simStruct, port, type);
        if (nCols == 1) {
            ssSetOutputPortWidth(simStruct, port, nRows);
        } else {
            ssSetOutputPortMatrixDimensions(simStruct, port, nRows, nCols);
        }
        if (nRows < 0 || nCols < 0) {
            ssSetOutputPortDimensionInfo(simStruct, port, DYNAMIC_DIMENSION);
        }
    }

    /** \ingroup initialization
     * 
     * This is the third static method called before the simulation starts.
     * 
     * This method must:
     *     1) specify the number of output ports
     *     2) specify the dimensions and the type of each output port 
     * 
     * This method must be overridded in your custom function.
     *      
     * Use -1 to specify dynamically dimensioned output signals.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeOutputPortSizes() {
        setOutputPortsCount(0);
    }

    /** \ingroup initialization
     * 
     * Sets the number of continuous states.
     */
    static inline void setContinuousStatesWidth(int num) {
        ssSetNumContStates(simStruct, num);
    }

    /** \ingroup initialization
     * 
     * Sets the number of discrete states.
     */
    static inline void setDiscreteStatesWidth(int num) {
        ssSetNumDiscStates(simStruct, num);
    }

    /** \ingroup initialization
     * 
     * This is the fourth static method called before the simulation starts.
     * 
     * This method must set the number of continuous and discrete states.
     *
     * The default method specifies zero continuous states and zero discrete states.
     *
     * For more information, see: http://www.mathworks.fr/fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeStatePortSizes() {
        setContinuousStatesWidth(0);
        setDiscreteStatesWidth(0);
    }

    /** \ingroup initialization
     * 
     * This is the fifth static method called before the simulation starts.
     * 
     * This method must set the number of sample time (i.e., sample rates) at
     * which the block operates.
     *
     * The default method specifies is a smaple time value of 1.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeNumberSampleTimes() {
        ssSetNumSampleTimes(simStruct, 1);
    }

    /** \ingroup initialization
     * 
     * This is the sixth and last static method called before the simulation starts.
     * 
     * This method specifies the simulation options that this block implements,
     * using ssSetOptions.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlinitializesizes.html */
    static void initializeOptions() {
    }

    /** 
     * This method sets the final dimensions of an input port.
     */
    static void setInputPortFinalSizes(int port, int nRows, int nCols) {
        if (nRows < 0 || nCols < 0) {
            throw std::runtime_error("Unable to set final dimensions of input port " + toString(port) + ".");
        }
        int_T dims[2];
        dims[0] = nRows;
        dims[1] = nCols;
        DECL_AND_INIT_DIMSINFO(dimsInfo);
        dimsInfo.numDims = 2;
        dimsInfo.dims = dims;
        dimsInfo.width = nRows*nCols;
        ssSetInputPortDimensionInfo(simStruct, 0, &dimsInfo);
    }

    /** \ingroup initialization
     * 
     * This static method is called with candidate dimensions for input port.
     * 
     * If the proposed dimensions are unacceptable for the port, the method must
     * throw an exception.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlsetinputportdimensioninfo.html */
    static void checkInputPortFinalSizes(int port, int nRows, int nCols) {
    }

    /** 
     * This method sets the final dimensions of an output port.
     */
    static void setOutputPortFinalSizes(int port, int nRows, int nCols) {
        if (nRows < 0 || nCols < 0) {
            throw std::runtime_error("Unable to set final dimensions of output port " + toString(port) + ".");
        }
        int_T dims[2];
        dims[0] = nRows;
        dims[1] = nCols;
        DECL_AND_INIT_DIMSINFO(dimsInfo);
        dimsInfo.numDims = 2;
        dimsInfo.dims = dims;
        dimsInfo.width = nRows*nCols;
        ssSetOutputPortDimensionInfo(simStruct, 0, &dimsInfo);
    }

    /** \ingroup initialization
     * 
     * This static method is called with candidate dimensions for output port.
     * 
     * If the proposed dimensions are unacceptable for the port, the method must
     * throw an exception.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlsetoutputportdimensioninfo.html */
    static void checkOutputPortFinalSizes(int port, int nRows, int nCols) {
    }

    /** \ingroup initialization
     * 
     * This static method specifies the sample time and offset time for
     * each sample rate at which this S-function operates.
     *
     * The default values are INHERITED_SAMPLE_TIME and FIXED_IN_MINOR_STEP_OFFSET
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlinitializesampletimes.html */
    static void initializeSampleTimes() {
        ssSetSampleTime(simStruct, 0, INHERITED_SAMPLE_TIME);
        ssSetOffsetTime(simStruct, 0, 0.0);
        ssSetModelReferenceSampleTimeDefaultInheritance(simStruct);
    }

    /** \ingroup runtime
     * 
     * This method is called at the beginning of a simulation right after contructing the class.
     * 
     * The method should perform initialization activities that this S-function
     * requires only once, such as allocating memory, setting up user data, or
     * initializing states.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlstart.html */
    void start() {
    }

    /** \ingroup runtime
     * 
     * This method is called at each simulation time step.
     * 
     * The method should compute the S-function's outputs at the current time
     * step and store the results using outputArray or using writeOutput.
     *
     * This method must be overridded in your custom function.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdloutputs.html */
    void outputs() {
    }

    /** \ingroup runtime
     * 
     * This optional method is called at each time step to compute the 
     * derivatives of the S-function's continuous states.
     * 
     * This method should store the derivatives in the S-function's state
     * derivatives array using derivativesArray.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlderivatives.html */
    void derivatives() {
    }

    /** \ingroup runtime
     * 
     * An S-function needs to provide this optional method only if it does
     * zero-crossing detection. Implementing zero-crossing detection typically
     * requires using the zero-crossing and mode work vectors to determine when
     * a zero crossing occurs and how the S-function's outputs should respond
     * to this event. The zeroCrossings method should update the S-function's
     * zero-crossing vector, using nonSampledZCsArray.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlzerocrossings.html */
    void zeroCrossings() {
    }

    /** \ingroup runtime
     * 
     * This optional method is called at each major simulation time step. 
     * 
     * The method should compute the S-function's states
     * at the current time step and store the states in the S-function's state
     * vector. The method can also perform any other tasks that the S-function
     * needs to perform at each major time step.
     *
     * Use this code if your S-function has one or more discrete states or does
     * not have direct feedthrough.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlupdate.html */
    void update() {
    }

    /** \ingroup runtime
     * 
     * This method is called when the simulation is terminated right before deleting the class.
     * 
     * This method should perform any terminal actions, such as freeing of
     * memory.
     *
     * For more information, see: http://www.mathworks.fr/help/simulink/sfg/mdlterminate.html */
    void terminate() {
    }

    /** \ingroup inputPort
     * 
     * Returns the double scalar value of an input port.
     */
    static inline double getInputDouble(int port) {
        if (port < 0 || port >= inputPortsCount)
            throw std::runtime_error("Input port number " + toString(port) + " does not exist.");
        return *ssGetInputPortRealSignal(simStruct, port);
    }

    /** \ingroup inputPort
     * 
     * Returns the scalar value of an input port.
     */
    template<typename _Scalar>
    static inline _Scalar getInputScalar(int port) {
        if (port < 0 || port >= inputPortsCount)
            throw std::runtime_error("Input port number " + toString(port) + " does not exist.");
        return *((_Scalar*) ssGetInputPortSignal(simStruct, port));
    }

    /** \ingroup inputPort
     * 
     * Returns an array mapping an input port.
     */
    template<typename _Scalar>
    static inline Array<_Scalar> getInputArray(int port) {
        if (port < 0 || port >= inputPortsCount)
            throw std::runtime_error("Input port number " + toString(port) + " does not exist.");
        return Array<_Scalar>((_Scalar*) ssGetInputPortSignal(simStruct, port), ssGetInputPortDimensionSize(simStruct, port, 0), ssGetInputPortDimensionSize(simStruct, port, 1), "input port " + toString(port), true);
    }

    /** \ingroup inputPort
     * 
     * Returns a pointer to the first element of the input port data.
     */
    static inline void* getInputData(int port) {
        if (port < 0 || port >= inputPortsCount)
            throw std::runtime_error("Input port number " + toString(port) + " does not exist.");
        return (void*) ssGetInputPortSignal(simStruct, port);
    }

    /** \ingroup inputPort
     * 
     * Returns the input port number of elements.
     * 
     * If the input port is a 1-D array with w elements, this function returns w.
     * 
     * If the input port is an M-by-N array, this function returns m*n.
     */
    static inline int getInputWidth(int port) {
        return ssGetInputPortWidth(simStruct, port);
    }

    /** \ingroup inputPort
     * 
     * Returns the input port number of rows.
     */
    static inline int getInputNRows(int port) {
        return ssGetInputPortDimensionSize(simStruct, port, 0);
    }

    /** \ingroup inputPort
     * 
     * Returns the input port number of cols.
     */
    static inline int getInputNCols(int port) {
        return ssGetInputPortDimensionSize(simStruct, port, 1);
    }

    /** \ingroup outputPort
     * 
     * Writes a double value to an output port.
     */
    static inline void setOutputDouble(int port, double value) {
        if (port < 0 || port >= outputPortsCount)
            throw std::runtime_error("Output port number " + toString(port) + " does not exist.");
        double *x = ssGetOutputPortRealSignal(simStruct, port);
        x[0] = value;
    }

    /** \ingroup outputPort
     * 
     * Writes a scalar value to an output port.
     */
    template<typename _Scalar>
    static inline void setOutputScalar(int port, _Scalar value) {
        if (port < 0 || port >= outputPortsCount)
            throw std::runtime_error("Output port number " + toString(port) + " does not exist.");
        _Scalar *x = ssGetOutputPortSignal(simStruct, port);
        x[0] = value;
    }

    /** \ingroup outputPort
     * 
     * Returns an array mapping an output port (left-side argument).
     */
    template<typename _Scalar>
    static inline Array<_Scalar> getOutputArray(int port) {
        if (port < 0 || port >= outputPortsCount)
            throw std::runtime_error("Output port number " + toString(port) + " does not exist.");
        return Array<_Scalar>((_Scalar*) ssGetOutputPortSignal(simStruct, port), ssGetOutputPortDimensionSize(simStruct, port, 0), ssGetOutputPortDimensionSize(simStruct, port, 1), "output port " + toString(port), true);
    }

    /** \ingroup outputPort
     * Returns a pointer to the first element of the output port data.
     */
    static inline void* getOutputData(int port) {
        if (port < 0 || port >= outputPortsCount)
            throw std::runtime_error("Output port number " + toString(port) + " does not exist.");
        return (void*) ssGetOutputPortSignal(simStruct, port);
    }

    /** \ingroup outputPort
     * 
     * Returns the output port number of elements.
     * 
     * If the output port is a 1-D array with w elements, this function returns w.
     * 
     * If the output port is an M-by-N array, this function returns m*n.
     */
    static inline int getOutputWidth(int port) {
        return ssGetOutputPortWidth(simStruct, port);
    }

    /** \ingroup outputPort
     * 
     * Returns the output port number of rows.
     */
    static inline int getOutputNRows(int port) {
        return ssGetOutputPortDimensionSize(simStruct, port, 0);
    }

    /** \ingroup outputPort
     * 
     * Returns the output port number of cols.
     */
    static inline int getOutputNCols(int port) {
        return ssGetOutputPortDimensionSize(simStruct, port, 1);
    }

    /** \ingroup parameterPort
     * 
     * Returns the double value of a parameter port.
     */
    static inline double getParameterDouble(int port) {
        if (port < 0 || port >= parameterPortsCount)
            throw std::runtime_error("Parameter port number " + toString(port) + " does not exist.");
        return mxGetPr(ssGetSFcnParam(simStruct, port))[0];
    }

    /** \ingroup parameterPort
     * 
     * Returns the scalar value of a parameter port.
     */
    template<typename _Scalar>
    static inline _Scalar getParameterScalar(int port) {
        if (port < 0 || port >= parameterPortsCount)
            throw std::runtime_error("Parameter port number " + toString(port) + " does not exist.");
        return *((_Scalar*) mxGetPr(ssGetSFcnParam(simStruct, port)));
    }

    /** \ingroup parameterPort
     * 
     * Returns the string value of a parameter port.
     */
    static inline std::string getParameterString(int port) {
        if (port < 0 || port >= parameterPortsCount)
            throw std::runtime_error("Parameter port number " + toString(port) + " does not exist.");
        char buffer[256];
        mxGetString(ssGetSFcnParam(simStruct, port), buffer, 256);
        return std::string(buffer);
    }

    /** \ingroup parameterPort
     * 
     * Returns an array mapping a parameter port.
     */
    template<typename _Scalar>
    static inline Array<_Scalar> getParameterArray(int port) {
        if (port < 0 || port >= parameterPortsCount)
            throw std::runtime_error("Parameter port number " + toString(port) + " does not exist.");
        return Array<_Scalar>(ssGetSFcnParam(simStruct, port), "parameter " + toString(port), true);
    }

    /** \ingroup parameterPort
     * 
     * Returns the parameter port number of elements.
     *
     * If the parameter port is a 1-D array with w elements, this function returns w. 
     * 
     * If the parameter port is an M-by-N array, this function returns m*n.
     */
    static inline int getParameterWidth(int port) {
        return (int) mxGetM(ssGetSFcnParam(simStruct, port))*(int) mxGetN(ssGetSFcnParam(simStruct, port));
    }

    /** \ingroup parameterPort
     * 
     * Returns the parameter port number of rows.
     */
    static inline int getParameterNRows(int port) {
        return (int) mxGetM(ssGetSFcnParam(simStruct, port));
    }

    /** \ingroup parameterPort
     * 
     * Returns the parameter port number of cols.
     */
    static inline int getParameterNCols(int port) {
        return (int) mxGetN(ssGetSFcnParam(simStruct, port));
    }

    /** \ingroup parameterPort
     * 
     * Returns a pointer to the first element of the parameter port data.
     */
    static inline void* getParameterData(int port) {
        return (void*) ssGetSFcnParam(simStruct, port);
    }

    /** \ingroup statePort
     * 
     * Returns the continuous state Array (1-D array).
     */
    static inline Array<double> getContinuousStateArray() {
        return Array<double>(ssGetContStates(simStruct), ssGetNumContStates(simStruct), 1, "continuous state", true);
    }

    /** \ingroup statePort
     * 
     * Returns a pointer to the first element of the state data (1-D array).
     */
    static inline double* getContinuousStateData() {
        return (double*) ssGetContStates(simStruct);
    }

    /** \ingroup statePort
     * 
     * Returns the continuous state number of elements (1-D array).
     */
    static inline int getContinuousStateWidth() {

        return ssGetNumContStates(simStruct);
    }

    /** \ingroup statePort
     * 
     * Returns the derivative state Array (1-D array).
     */
    static inline Array<double> getDerivativeStateArray() {
        return Array<double>((double*) ssGetdX(simStruct), ssGetNumContStates(simStruct), 1, "continuous state", true);
    }

    /** \ingroup statePort
     * 
     * Returns a pointer to the first element of the derivative state data (1-D array).
     */
    static inline double* getDerivativeStateData() {
        return (double*) ssGetdX(simStruct);
    }

    /** \ingroup statePort
     * 
     * Writes the derivative state Array (1-D array).
     */
    static inline void setDerivativeStateArray(Array<double> & array) {
        if (ssGetNumContStates(simStruct) != array.getNRows() || array.getNCols() != 1)
            throw std::runtime_error("Unable to write " + array.getName() + " to derivative of state port. Array dimensions must agree.");

        memcpy((void*) ssGetdX(simStruct), (void*) array.getData(), array.getWidth() * sizeof (double));
    }

    /** \ingroup statePort
     * 
     * Returns the discrete state Array (1-D array).
     */
    static inline Array<double> getDiscreteStateArray() {
        return Array<double>((double*) ssGetDiscStates(simStruct), ssGetNumDiscStates(simStruct), 1, "discrete state", true);
    }

    /** \ingroup statePort
     * 
     * Returns a pointer to the first element of the discrete state data (1-D array).
     */
    static inline double* getDiscreteStateData() {
        return (double*) ssGetDiscStates(simStruct);
    }

    /** \ingroup statePort
     * 
     * Returns the discrete state number of elements (1-D array).
     */
    static inline int getDiscreteStateWidth(int port) {
        return ssGetNumDiscStates(simStruct);
    }

    /** \ingroup statePort
     * 
     * Writes the discrete state Array (1-D array).
     */
    static inline void setDiscreteStateArray(Array<double> & array) {
        if (ssGetNumDiscStates(simStruct) != array.getNRows() || array.getNCols() != 1)
            throw std::runtime_error("Unable to write " + array.getName() + " to discrete state port. Array dimensions must agree.");

        memcpy((void*) ssGetDiscStates(simStruct), (void*) array.getData(), array.getWidth() * sizeof (double));
    }

    /** Get the current simulation time */
    static inline time_T getSimulationTime() {
        return ssGetT(simStruct);
    }



};

SimStruct *BaseBlock::simStruct = NULL;
int BaseBlock::parameterPortsCount = 0;
int BaseBlock::inputPortsCount = 0;
int BaseBlock::outputPortsCount = 0;


#endif
