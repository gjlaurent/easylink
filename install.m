% Installation script of EasyLink Library.
% Copyright(c) 2014 FEMTO-ST, ENSMM, UFC, CNRS.

disp('Installation of EasyLink Library.');
disp(' ');

path = fileparts(mfilename('fullpath'));
addpath(path);
disp('Current easyLink folder has been added to the MATLAB path.');
clear path

disp(' ');
r = input('Do you wish to save the current path for use in future MATLAB sessions [y/n]? ', 's');
if r == 'Y' || r == 'y'
    if (savepath == 0)
        disp('The path has been saved successfully for use in future sessions.');
    else
        disp('The path could not be saved for use in future sessions.');
    end
end

disp(' ');
r = input('Would you like to specify the compiler options with "mex -setup" command [y/n])? ', 's');
if r == 'Y' || r == 'y'
    mex -setup
end

clear r
disp(' ');
disp('Installation completed.');


