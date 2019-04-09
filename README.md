# EasyLink

EasyLink is a C++ software development kit for developing MEX-functions 
for MATLAB and blocks for SIMULINK (S-functions).

EasyLink allows to develop C++ MEX-functions and S-functions with less code and
using friendly classes.

EasyLink is simpler and as efficient than the MATLAB C API. It provides a
direct access to input, output and parameter ports as well as MATLAB
variables thanks to a C++ Array class or using Eigen.

EasyLink is a multi-platform library that works under Linux, OSX and Windows.

EasyLink is developed and maintained within the FEMTO-ST Institute located in Besançon, France and hosted by Renater and Github.

EasyLink official site is: https://gjlaurent.github.io/easylink/

# Documentation

  - <a href="https://gjlaurent.github.io/easylink/html/page_quick_reference.html">Quick reference</a>
  - <a href="https://gjlaurent.github.io/easylink/html/page_examples.html">Examples</a>
  - <a href="https://gjlaurent.github.io/easylink/html/modules.html">User reference</a>


# Installation

  - Clone ar download EasyLink in a local directory
  - Open MATLAB
  - Set the matlab current directory where EasyLink is located
  - Execute the startup file "install.m" and follow
     instructions to select an installed C++ compiler
`>>install`
  - Execute the following command to compile and link a demo file
`>>make sfunOffset.cpp`
  - Open Simulink
  - Open the file "testOffset.mdl/slx" and run it to check the installation

Use the file sfunOffset.cpp as a template to write a new S-function (SIMULINK).

Use the file mexArrayProduct.cpp as a template to write a new MEX-function (MATLAB).

# Licence

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
