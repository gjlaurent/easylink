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

/**

\defgroup initialization S-function initialization methods
\defgroup runtime S-function runtime methods
\defgroup inputPort Input ports managing methods
\defgroup outputPort Output ports managing methods
\defgroup parameterPort S-function parameters managing methods
\defgroup statePort S-function state ports managing methods
\defgroup matlabArray Matlab variables managing methods
\defgroup utils Miscellaneous functions on strings and numbers

\mainpage Overview

You need to write efficient MATLAB functions or SIMULINK blocks?

You hate the MATLAB C API? You do not like the code produced by the S-function
generator either?

You would like to use C++ and to write classes? 

EasyLink is for you!!!

### Description

EasyLink is a C++ software development kit for developing MEX-functions 
for MATLAB and blocks for SIMULINK (S-functions).

EasyLink allows to develop C++ MEX-functions and S-functions with less code and
using friendly classes.

EasyLink is simpler and as efficient than the MATLAB C API. It provides a
direct access to input, output and parameter ports as well as MATLAB
variables thanks to a C++ Array class or using Eigen.

EasyLink is a multi-platform library that works under Linux, OSX and Windows.

### Documentation

  - \ref pageQuickReference
  - \ref pageExamples
  - <a href="modules.html">User reference</a>

### Download

The latest release can be downloaded <a href="https://github.com/gjlaurent/easylink">
here</a>.

Use Git or checkout with SVN using the web URL:

\verbatim https://github.com/gjlaurent/easylink.git \endverbatim

### Installation

  -# Clone ar download EasyLink in a local directory
  -# Open MATLAB
  -# Set the matlab current directory where EasyLink is located
  -# Execute the startup file "install.m" and follow
     instructions to select an installed C++ compiler
\verbatim >>install \endverbatim
  -# Execute the following command to compile and link a demo file
\verbatim >>make sfunOffset.cpp \endverbatim
  -# Open Simulink
  -# Open the file "testOffset.mdl/slx" and run it to check the installation

Use the file sfunOffset.cpp as a template to write a new S-function (SIMULINK).

Use the file mexArrayProduct.cpp as a template to write a new MEX-function (MATLAB).

### Licence

EasyLink is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

EasyLink is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with the sources files.  If not, see http://www.gnu.org/licenses/.

Copyright (c) 2014-2019 FEMTO-ST, ENSMM, UFC, CNRS.

### About

EasyLink is developed and maintained within the FEMTO-ST Institute located in
Besançon, France and hosted by Renater.

EasyLink official site is: http://sourcesup.renater.fr/easylink/

contact: Guillaume J. Laurent, https://gjlaurent.github.io/


\page pageQuickReference Quick reference 

### Input port methods

\code{.cpp}
    double input0=getInputScalar(0);
    Array<double>  input1=getInputArray<double>(1);
\endcode

### Output port methods

\code{.cpp}
    setOutputScalar(0,output0);
    Array<double> output1=getOutputArray<double>(1);
\endcode

### Parameter port methods

\code{.cpp}
    int parameter0 = getParameterScalar(0);
    double parameter1 = getParameterScalar(1);
    std::string parameter2 = getParameterString(2);
    Array<double> parameter3 = getParameterArray<double>(3);
\endcode

### Arrays

\code{.cpp}
    if (existMatlabArray("test")) ...

    Array<double> test = newMatlabArray(2, 2, "test");
    test(0, 0) = 2;
    test(1, 1) = 4;

    Array<double> test2 = getMatlabArray("test2");
    test2 += 1;
    test2.print();

    Array<double> a(2, 2, "A");
    a(0, 0) = -3;
    a(1, 1) = 4;
    a.print();
\endcode


\page pageExamples Examples

S-function basic example (use it as a template to write new S-functions):

  - sfunOffset.cpp shows how to write a S-function that adds a scalar value
    to a scalar input.

S-function examples that demonstrate all the features:

  - sfunInputs.cpp shows all the different ways to use the input ports.

  - sfunOutputs.cpp shows all the different ways to use the output ports.

  - sfunParameters.cpp shows all the different ways to use the parameters ports.

  - sfunMatlabArrays.cpp shows all the different ways to use the matlab arrays.

S-function advanced examples:

  - sfunSizeChange.cpp shows how to change the size of an output according
    to the value of a parameter.

  - sfunTimesTwo.cpp show how to change the size of an output according to
    the size of a dynamically-sized input array.

S-function examples using Eigen:

  - sfunTimesTwoWithEigen.cpp same as sfunTimesTwo.cpp but using Eigen in place 
    of built-in Array class.

  - sfunStateSpace.cpp shows how to write S-functions with continuous states
    and Eigen.

MEX-function basic example (use it as a template to write new MEX-functions):

  - mexArrayProduct.cpp shows how to write a MEX-function that multiplies
    a matrix and a scalar.

MEX-function example using Eigen:

  - mexArrayProductWithEigen.cpp same as mexArrayProduct.cpp but using Eigen 
    in place of built-in Array class.

 */

#ifndef EASYLINK_H
#define EASYLINK_H

#define __CPP2011__

#ifndef MATLAB_MEX_FILE
#define MATLAB_MEX_FILE
#endif

#ifdef S_FUNCTION_NAME
#ifndef S_FUNCTION_LEVEL
#define S_FUNCTION_LEVEL 2
#endif
#include "simstruc.h"
#include "BaseBlock.h"
#else
#include "mex.h"
#include "BaseFunction.h"
#endif

#include "MatlabArray.h"

#endif
