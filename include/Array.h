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

#ifndef EASYLINK_ARRAY_H
#define EASYLINK_ARRAY_H

#include "Utils.h"
#include <string>
#include <stdexcept>

/** Array is a template array class allowing element-wise operations.
 *
 * Array is used to access input, output and parameter ports as well
 * as MATLAB mxArray.
 */
template<typename _Scalar>
class Array {
public:

    /** Default constructor. */
    Array(std::string name = "untitled array") {
        nrows = 0;
        ncols = 0;
        mxarray = NULL;
        data = NULL;
        this->name = name;
        moveable = false;
        releaseData = false;
#ifdef __TEST__
        printf("EasyLink test message: constructing empty array called \"%s\".\n", getFullName().c_str());
#endif
    }

    /** Construct an Array and allocate memory for data.
     *
     * Set moveable to true if the Array is used as a return value (and
     * to avoid data copy in this case). */
    Array(int nrows, int ncols = 1, std::string name = "untitled array", bool moveable = false) {
        this->nrows = nrows;
        this->ncols = ncols;
        this->mxarray = NULL;
        this->data = new _Scalar[nrows * ncols];
        this->name = name;
        this->moveable = moveable;
        this->releaseData = true;
        memset((void*) data, 0, nrows * ncols * sizeof (_Scalar));
#ifdef __TEST__
        allocationNumber++;
        printf("EasyLink test message: constructing array called \"%s\".\n", getFullName().c_str());
#endif
    }

    /** Construct an Array using an existing mxArray.
     * The constructor do NOT allocate and NOT copy the data if dataCopy is false.
     *
     * Set moveable to true if the Array is used as a return value (and
     * to avoid data copy in this case). */
    Array(const mxArray *mxarray, std::string name = "untitled mxArray", bool moveable = false, bool dataCopy = false) {
        if (mxIsSparse(mxarray) || !mxIsNumeric(mxarray))
            throw std::runtime_error("The array must be a dense array of numeric values.");

        this->nrows = (int) mxGetM(mxarray);
        this->ncols = (int) mxGetN(mxarray);
        if (dataCopy) {
            this->mxarray = NULL;
            this->data = new _Scalar[nrows * ncols];
            memcpy((void*) data, (void*) mxGetPr(mxarray), nrows * ncols * sizeof (_Scalar));
            this->releaseData = true;
#ifdef __TEST__
            allocationNumber++;
#endif
        } else {
            this->mxarray = (mxArray*) mxarray;
            this->data = mxGetPr(mxarray);
            this->releaseData = false;
        }
        this->name = name;
        this->moveable = moveable;
#ifdef __TEST__
        printf("EasyLink test message: constructing array (using allocated mxarray) called \"%s\".\n", getFullName().c_str());
#endif
    }

    /** Construct an Array with already allocated data without using mxArray.
     * The constructor do NOT allocate and NOT copy the data.
     *
     * Set moveable to true if the Array is used as a return value (and
     * to avoid data copy in this case).
     *
     * Set releaseData to true if the Array must free the memory during deletion. */
    Array(const _Scalar *data, int nrows, int ncols = 1, std::string name = "untitled data", bool moveable = false, bool releaseData = false) {
        this->nrows = nrows;
        this->ncols = ncols;
        this->mxarray = NULL;
        this->data = (_Scalar*) data;
        this->name = name;
        this->moveable = moveable;
        this->releaseData = releaseData;
#ifdef __TEST__
        printf("EasyLink test message: constructing array (using allocated _Scalar array) \"%s\".\n", getFullName().c_str());
#endif
    }

    /** Copy constructor. The method generally does a copy of the data. If the
     * argument is moveable, the data are stolen to this argument before its
     * deletion (no data copy). */
    Array(Array<_Scalar> &array) {
        if (array.moveable) {
#ifdef __TEST__
            printf("EasyLink test message: stealing of \"%s\" in copy constructor.\n", array.name.c_str());
#endif
            stealData(array);
        } else {
#ifdef __TEST__
            printf("EasyLink test message: hard copy of \"%s\" in copy constructor.\n", array.name.c_str());
#endif
            copyOf(array);
        }
    }

    /** Move constructor. C++ 2011 only.*/
#ifdef __CPP2011__

    template<typename T> Array(T && array) {
#ifdef __TEST__
        printf("EasyLink test message: stealing of \"%s\" in move constructor.\n", array.name.c_str());
#endif
        stealData(array);
    }
#endif

    /** Destructor. Free the memory if not shared. */
    ~Array() {
        empty();
    }

    /** Empty the array and free the memory if owned. */
    void empty() {
        if (releaseData) {
#ifdef __TEST__
            allocationNumber--;
            printf("EasyLink test message: releasing data of \"%s\".\n", getFullName().c_str());
#endif
            delete [] data;
        }
        nrows = 0;
        ncols = 0;
        mxarray = NULL;
        data = NULL;
        moveable = false;
        releaseData = false;
    }

    /** Array assignment. The method generally does a copy of the data. If the
     * argument is moveable, the data are stolen to this argument before its
     * deletion (no data copy).
     *
     * Throws an exception if the data is shared and sizes don't match. */
    Array<_Scalar>& operator=(Array<_Scalar> &array) {
        if (nrows == 0 && ncols == 0) // empty array
        {
            if (array.moveable) {
#ifdef __TEST__
                printf("EasyLink test message: stealing in assignment \"%s=%s\".\n", name.c_str(), array.name.c_str());
#endif
                stealData(array);
            } else {
#ifdef __TEST__
                printf("EasyLink test message: hard copy in assignment \"%s=%s\".\n", name.c_str(), array.name.c_str());
#endif
                copyOf(array);
            }
        } else if (releaseData) {
            if (array.moveable) {
                empty();
#ifdef __TEST__
                printf("EasyLink test message: stealing in assignment \"%s=%s\".\n", name.c_str(), array.name.c_str());
#endif
                stealData(array);
            } else if (nrows == array.nrows && ncols == array.ncols) {
#ifdef __TEST__
                printf("EasyLink test message: hard copy in assignment \"%s=%s\".\n", name.c_str(), array.name.c_str());
#endif
                memcpy((void*) data, (void*) array.data, nrows * ncols * sizeof (_Scalar));
            } else {
#ifdef __TEST__
                printf("EasyLink test message: hard copy in assignment \"%s=%s\".\n", name.c_str(), array.name.c_str());
#endif
                empty();
                copyOf(array);
            }
        } else // shared data
        {
            if (nrows != array.nrows || ncols != array.ncols)
                throw std::runtime_error("Unable to assign " + array.name + " to shared array " + name + ". Array dimensions must agree.");

#ifdef __TEST__
            printf("EasyLink test message: hard copy in assignment \"%s=%s\".\n", name.c_str(), array.name.c_str());
#endif
            memcpy((void*) data, (void*) array.data, nrows * ncols * sizeof (_Scalar));
        }
        return *this;
    }

    /** Move assignment. C++ 2011 only. */
#ifdef __CPP2011__

    template<typename T> T& operator=(T && array) {
        if (nrows == 0 && ncols == 0) // empty array
        {
#ifdef __TEST__
            printf("EasyLink test message: stealing in move assignment \"%s=%s\".\n", name.c_str(), array.name.c_str());
#endif
            stealData(array);
        } else if (releaseData) {
#ifdef __TEST__
            printf("EasyLink test message: stealing in move assignment \"%s=%s\".\n", name.c_str(), array.name.c_str());
#endif
            empty();
            stealData(array);
        } else // shared data
        {
            if (nrows != array.nrows || ncols != array.ncols)
                throw std::runtime_error("Unable to move assign " + array.name + " to shared array " + name + ". Array dimensions must agree.");

#ifdef __TEST__
            printf("EasyLink test message: hard copy in assignment \"%s=%s\".\n", name.c_str(), array.name.c_str());
#endif
            memcpy((void*) data, (void*) array.data, nrows * ncols * sizeof (_Scalar));
        }
        return *this;
    }
#endif

    /** Returns the address of the data. */
    _Scalar* getData() {
        return data;
    }

    /** Returns themxarray mapped by the array (no data copy).
     * 
     * The obtained mxArray must not be released using mxDestroyArray. */
    mxArray* getmxArray() {
        return mxarray;
    }

    /** Returns true is the array does not own the data. */
    bool isShared() {
        return !releaseData;
    }

    /** Returns the number of elements of the array.
     * For 1-D array with w elements, this method returns w.
     * For M-by-N array, this method returns m*n. */
    int getWidth() {
        return nrows*ncols;
    }

    /** Returns the number of columns of the array. */
    int getNCols() {
        return ncols;
    }

    /** Returns the number of rows of the array. */
    int getNRows() {
        return nrows;
    }

    /** Returns the name of the array. */
    std::string getName() {
        return name;
    }

    /** Read/write access to the element i of the array.
     * i can go from 0 to nrows*ncols-1.
     * Range errors throw an exception. */
    _Scalar & operator[](int i) {
        if ((i < 0) || (i >= nrows * ncols))
            throw std::range_error("Index exceeds array dimensions when accessing to " + name + "[" + toString(i) + "].");
        return *(data + i);
    }

    /** Read/write access to the element (row,col) of the array.
        Range errors throw an exception. */
    _Scalar & operator()(int row, int col) {
        if ((row < 0) || (row >= nrows) || (col < 0) || (col >= ncols))
            throw std::range_error("Index exceeds array dimensions when accessing to " + name + "(" + toString(row) + "," + toString(col) + ").");
        return *(data + row + nrows * col);
    }

    /** Initialization of all elements at the same value. */
    void init(_Scalar x = 0) {
        int i = nrows*ncols;
        _Scalar* p = data;
        for (; i--; p++)
            *p = x;
    }

    /** Reshaped to a new size with the same elements
     * nrows*ncols must equal the number of elements of the array. */
    void reshape(int nrows, int ncols) {
        if (this->nrows * this->ncols != nrows * ncols)
            throw std::runtime_error("Unable to reshape " + name + ".");

        this->nrows = nrows;
        this->ncols = ncols;
    }

    /** Print the array in the console. */
    void print() {
#ifdef __TEST__
        printf(getFullName().c_str());
#else
        printf(name.c_str());
#endif
        printf(" = \n");
        printf("[");
        for (int row = 0; row < nrows; row++) {
            for (int col = 0; col < ncols; col++)
                printf("  %7g", data[row + nrows * col]);

            if (row != nrows - 1)
                printf("\n ");
        }

        printf("]\n");
    }

    /** In-place element-by-element addition. */
    void operator+=(_Scalar x) {
        int i = nrows*ncols;
        _Scalar* p = data;
        for (; i--; p++)
            *p += x;
    }

    /** In-place element-by-element substraction. */
    void operator-=(_Scalar x) {
        int i = nrows*ncols;
        _Scalar* p = data;
        for (; i--; p++)
            *p -= x;
    }

    /** In-place element-by-element multiplication. */
    void operator*=(_Scalar x) {
        int i = nrows*ncols;
        _Scalar* p = data;
        for (; i--; p++)
            *p *= x;
    }

    /** In-place element-by-element division. */
    void operator/=(_Scalar x) {
        int i = nrows*ncols;
        _Scalar* p = data;
        for (; i--; p++)
            *p /= x;
    }

    /** Array-_Scalar addition. */
    Array<_Scalar> operator+(_Scalar x) {
        Array<_Scalar> result(*this);
        result += x;
        result.moveable = true;
        result.name = "(" + name + ")+" + toString(x);
        return result;
    }

    /** Array-_Scalar substraction. */
    Array<_Scalar> operator-(_Scalar x) {
        Array<_Scalar> result(*this);
        result -= x;
        result.moveable = true;
        result.name = "(" + name + ")-" + toString(x);
        return result;
    }

    /** Array-_Scalar multiplication. */
    Array<_Scalar> operator*(_Scalar x) {
        Array<_Scalar> result(*this);
        result *= x;
        result.moveable = true;
        result.name = "(" + name + ")*" + toString(x);
        return result;
    }

    /** Array-_Scalar division. */
    Array<_Scalar> operator/(_Scalar x) {
        Array<_Scalar> result(*this);
        result /= x;
        result.moveable = true;
        result.name = "(" + name + ")/" + toString(x);
        return result;
    }

    /** In-place element-by-element addition.
     * Arrays must have the same dimensions. */
    void operator+=(const Array<_Scalar> & operand) {
        if (nrows != operand.nrows || ncols != operand.ncols)
            throw std::runtime_error("Unable to add " + name + " and " + operand.name + ". Array dimensions must agree.");

        int i = nrows*ncols;
        _Scalar* p = data;
        _Scalar* s = operand.data;
        for (; i--; p++, s++)
            *p += (*s);
    }

    /** In-place element-by-element substraction.
     * Arrays must have the same dimensions. */
    void operator-=(const Array<_Scalar> & operand) {
        if (nrows != operand.nrows || ncols != operand.ncols)
            throw std::runtime_error("Unable to substract " + name + " and " + operand.name + ". Array dimensions must agree.");

        int i = nrows*ncols;
        _Scalar* p = data;
        _Scalar* s = operand.data;
        for (; i--; p++, s++)
            *p -= (*s);
    }

    /** In-place element-by-element multiplication.
     * Arrays must have the same dimensions. */
    void operator*=(const Array<_Scalar> & operand) {
        if (nrows != operand.nrows || ncols != operand.ncols)
            throw std::runtime_error("Unable to multiply " + name + " and " + operand.name + ". Array dimensions must agree.");

        int i = nrows*ncols;
        _Scalar* p = data;
        _Scalar* s = operand.data;
        for (; i--; p++, s++)
            *p *= (*s);
    }

    /** In-place element-by-element division.
     * Arrays must have the same dimensions. */
    void operator/=(const Array<_Scalar> & operand) {
        if (nrows != operand.nrows || ncols != operand.ncols)
            throw std::runtime_error("Unable to divide " + name + " and " + operand.name + ". Array dimensions must agree.");

        int i = nrows*ncols;
        _Scalar* p = data;
        _Scalar* s = operand.data;
        for (; i--; p++, s++)
            *p /= (*s);
    }

    /** Array-array element-by-element addition. */
    //    Array<_Scalar> operator+(const Array<_Scalar> & operand) {
    //        return callMatlab("plus", operand);
    //    }

    /** Array-array element-by-element substraction. */
    //    Array<_Scalar> operator-(const Array<_Scalar> & operand) {
    //        return callMatlab("minus", operand);
    //    }

    /** Array-array element-by-element multiplication (matrix product). */
    //    Array<_Scalar> operator*(const Array<_Scalar> & operand) {
    //        return callMatlab("mtimes", operand);
    //    }

    /** Element-by-element comparison.
     * Returns true if the array contains the same values.
     * Use areEqual to compare element values. */
    bool operator==(const Array<_Scalar> & operand) {
        bool result;
        if (nrows * ncols != operand.nrows * operand.ncols)
            result = false;
        else {
            int i = nrows*ncols;
            _Scalar *p = data;
            _Scalar *q = operand.data;
            for (; i-- && areEqual(*p, *q); p++, q++)
                ;
            result = (i < 0);
        }
        return result;
    }

    /** Returns the maximal value of the array */
    _Scalar getMax() {
        int i = nrows * ncols - 1;
        _Scalar *p = data;
        _Scalar result = *p;
        p++;
        for (; i--; p++)
            if (*p > result)
                result = *p;

        return result;
    }

    /** Returns the minimal value of the array */
    _Scalar getMin() {
        int i = nrows * ncols - 1;
        _Scalar *p = data;
        _Scalar result = *p;
        p++;
        for (; i--; p++)
            if (*p < result)
                result = *p;

        return result;
    }

    /** Returns the additive inverse of the array. */
    Array<_Scalar> operator-() {
        Array<_Scalar> result(*this);
        int i = nrows*ncols;
        _Scalar* p = result.data;
        for (; i--; p++)
            *p = -(*p);
        result.moveable = true;
        result.name = "-" + name;
        return result;
    }

    /** Returns the inverse of the array. */
    //    Array inverse() {
    //        return callMatlab("inv");
    //    }

    /** Return the identity matrix. */
    //    static Array eye(int nrows, int ncols, std::string name = "eye array") {
    //        Array result(nrows, ncols, name, true);
    //
    //        for (int i = 0; i < min(nrows, ncols); i++) {
    //            result(i, i) = 1;
    //        }
    //
    //        return result;
    //    }

protected:

    int ncols, nrows;
    mxArray *mxarray;
    _Scalar *data;
    std::string name;
    bool moveable, releaseData;

    inline void stealData(const Array<_Scalar> & array) {
        nrows = array.nrows;
        ncols = array.ncols;
        mxarray = array.mxarray;
        data = array.data;
        name = array.name;
        moveable = false;
        releaseData = array.releaseData;
        *(bool*)(&array.releaseData) = false; // Trick to write a member of a const argument
    }

    inline void copyOf(const Array<_Scalar> & array) {
        nrows = array.nrows;
        ncols = array.ncols;
        mxarray = NULL;
        data = new _Scalar[nrows * ncols];
        memcpy((void*) data, (void*) array.data, nrows * ncols * sizeof (_Scalar));
        name = std::string("copy of ") + array.name;
        moveable = false;
        releaseData = true;
#ifdef __TEST__
        allocationNumber++;
#endif
    }

    inline std::string getFullName() {
        std::string result = name + " (" + toString(nrows) + "x" + toString(ncols) + " ";

        if (moveable)
            result += "moveable ";
        if (releaseData)
            result += "releaseData ";
        if (mxarray != NULL)
            result += "mex array)";
        else
            result += "_Scalar array)";

        return result;
    }

#ifdef __TEST__    
public:
    static int allocationNumber;
#endif
};

#ifdef __TEST__  
template<typename _Scalar>
int Array<_Scalar>::allocationNumber = 0;
#endif

/** Double-array addition. */
template<typename _Scalar>
Array<_Scalar> operator+(_Scalar x, Array<_Scalar> & operand) {
    return operand.operator+(x);
}

/** Double-array substraction. */
template<typename _Scalar>
Array<_Scalar> operator-(_Scalar x, Array<_Scalar> & operand) {
    return operand.opposite().operator+(x);
}

/** Double-array multiplication. */
template<typename _Scalar>
Array<_Scalar> operator*(_Scalar x, Array<_Scalar> & operand) {
    return operand.operator*(x);
}

#endif
