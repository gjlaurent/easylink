//------------------------------------------------------------------------------
//
//  This file is part of EasyLink Library.
//
//  Copyright (c) 2014 FEMTO-ST, ENSMM, UFC, CNRS.
//
//  License: GNU General Public License 3
//  Author: Guillaume J. Laurent
//
//------------------------------------------------------------------------------
#ifndef EASYLINK_UTILS_H
#define EASYLINK_UTILS_H

//------------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>


//------------------------------------------------------------------------------
#ifdef _MSC_VER
#include <windows.h>
#undef IN
#undef OUT
#endif

//------------------------------------------------------------------------------
static char ERROR_MSG_BUFFER[512];

/** \ingroup utils
 * Converts a double, int or char to a String.
 */
template <typename T> inline std::string toString(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

/** \ingroup utils
 * Returns true if the string is a valid MATLAB variable identifier.
 */
inline bool isIdentifier(std::string str) {
    unsigned int i;
    bool valid;
    valid = (((str[0] >= 'a')&&(str[0] <= 'z'))
            || ((str[0] >= 'A')&&(str[0] <= 'Z')));
    i = 1;
    while ((i < str.size()) && valid) {
        valid = (((str[i] >= 'a')&&(str[i] <= 'z'))
                || ((str[i] >= 'A')&&(str[i] <= 'Z'))
                || ((str[i] >= '0')&&(str[i] <= '9'))
                || (str[i] == '_'));
        i++;
    }
    return valid;
}

/** \ingroup utils
 * Returns the current computer time in milliseconds.
 */
inline unsigned long getComputerTime() {
#ifdef _MSC_VER
    return (unsigned long) (GetTickCount());
#else
    return (unsigned long) (clock() / 1000);
#endif
}

#ifdef _MSC_VER

inline int round(double x) {
    if (x > -0.5)
        return ((int) (x + 0.5));
    else
        return ((int) (x - 0.5));
}
#endif

#define EQUALITY_TOLERANCE 0.00000000000001

/** \ingroup utils
 * Returns true if x are equal to y. The test takes into account the
 * precision of the mantissa.
 */
inline bool areEqual(double x, double y, double tolerance = EQUALITY_TOLERANCE) {
    if (fabs(x) > 1.0) {
        double mx, my;
        int ex, ey;
        mx = frexp(x, &ex);
        my = frexp(y, &ey);
        return ( (ex == ey) && (mx < my + tolerance) && (mx > my - tolerance));
    } else
        return (fabs(x - y) < 2 * tolerance);
}

//------------------------------------------------------------------------------
#endif
