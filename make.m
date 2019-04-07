function make(file, files, options)
% MAKE Compile MEX function with EasyLink library.
%
% Usages :
%   >> make(file)
%   >> make(file, files)
%   >> make(file, files, options)
%
% Description :
% MAKE calls the MATLAB MEX function that compiles and links source
% files into a shared library called a MEX file, executable from
% within MATLAB. The resulting file has a platform-dependent
% extension. Use the MEXEXT command to return the extension for this
% machine or for all supported platforms.
%
% Use 'files' argument to add any combinations of source files,
% object files, and library files.
%
% Use 'options' argument to add command line options to MEX. Use the
% mex -help command for more informations.
%
% The MEX function must have been configured previously using the
% mex -setup command.
%
% Example :
%   >> make('sfunOffset.cpp')
%
% MAKE is part of EasyLink Library.
% Copyright(c) 2014 FEMTO-ST, ENSMM, UFC, CNRS.

if (nargin < 3)
    options = '';
end
if (nargin < 2)
    files = '';
end

path = fileparts(which('make.m'));
files = ['-I' path '/include ' '-I' path '/3rdparty ' files];

if strcmp(computer, 'PCWIN') || strcmp(computer, 'PCWIN64')
    
elseif strcmp(computer, 'MACI') || strcmp(computer, 'GLNX86')
    options = ['CFLAGS="\$CFLAGS -m32" LDFLAGS="\$LDFLAGS -m32" ' options];
elseif strcmp(computer, 'MACI64') || strcmp(computer, 'GLNXA64')
    
else
    disp('Unknown platform');
    return;
end

disp(['EasyLink: Compiling ' file]);
P = ['mex ' options ' ' file ' ' files];
eval(P);

