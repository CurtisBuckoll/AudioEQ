#ifndef _MATRIX_
#define _MATRIX_

#include "Util.h"

#include <vector>

namespace Math
{

//========================================================================
// n x n square matrix
template<typename T>
struct Matrix
{
    //--------------------------------------------------------------
    // Init to identity
    Matrix( size_t size );

    //--------------------------------------------------------------
    //
    void scaleBy( double s );

    //--------------------------------------------------------------
    //
    void transpose();

    //--------------------------------------------------------------
    //
    //void makeRandom( int low, int hi );

    //--------------------------------------------------------------
    //
    void print();

    //--------------------------------------------------------------
    //
    Matrix operator*( const Matrix<T>& mat );

    //--------------------------------------------------------------
    //
    std::vector<T> operator*( const std::vector<T>& vec );

    //--------------------------------------------------------------
    //
    size_t                      sz_;
    std::vector<std::vector<T>> matrix_;
};
}

#include "Matrix.cpp"
#endif
