#pragma once

#include "Matrix.h"

#include <vector>
#include <array>
#include <math.h>

//--------------------------------------------------------------
//
class DCT
{
public:

    //--------------------------------------------------------------
    //
    DCT( size_t mat_size );

    //--------------------------------------------------------------
    //
    Matrix transform( const Matrix& block );

    //--------------------------------------------------------------
    //
    std::vector<double> transform( const std::vector<double>& chunk );

    //--------------------------------------------------------------
    //
    Matrix inverse_transform( const Matrix& block );

    //--------------------------------------------------------------
    //
    std::vector<double> inverse_transform( const std::vector<double>& chunk );

private:

    //--------------------------------------------------------------
    //
    size_t sz_;
    Matrix T;
    Matrix T_t;
};
