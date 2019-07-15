#pragma once

#include "Matrix.h"

#include <vector>
#include <array>
#include <math.h>

// =======================================================================
// 
class DCT
{
public:

    //--------------------------------------------------------------
    //
    DCT( size_t mat_size );

    //--------------------------------------------------------------
    //
    Math::Matrix<double> transform( const Math::Matrix<double>& block );

    //--------------------------------------------------------------
    //
    std::vector<double> transform( const std::vector<double>& chunk );

    //--------------------------------------------------------------
    //
    Math::Matrix<double> inverse_transform( const Math::Matrix<double>& block );

    //--------------------------------------------------------------
    //
    std::vector<double> inverse_transform( const std::vector<double>& chunk );

private:

    DCT() = delete;

    //--------------------------------------------------------------
    //
    size_t sz_;
    Math::Matrix<double> T;
    Math::Matrix<double> T_t;
};
