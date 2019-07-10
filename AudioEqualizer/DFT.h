#pragma once

#include "Matrix.h"
#include "Complex.h"

#include <vector>
#include <array>
#include <math.h>

// =======================================================================
// 
class DFT
{
public:

    //--------------------------------------------------------------
    //
    DFT( size_t mat_size );

    //--------------------------------------------------------------
    //
    Math::Matrix<Math::Complex> transform( const Math::Matrix<Math::Complex>& block );

    //--------------------------------------------------------------
    //
    std::vector<Math::Complex> transform( const std::vector<Math::Complex> & chunk );

    //--------------------------------------------------------------
    //
    Math::Matrix<Math::Complex> inverse_transform( const Math::Matrix<Math::Complex>& block );

    //--------------------------------------------------------------
    //
    std::vector<Math::Complex> inverse_transform( const std::vector<Math::Complex>& chunk );

private:

    //--------------------------------------------------------------
    //
    size_t sz_;
    Math::Matrix<Math::Complex> T;
    Math::Matrix<Math::Complex> T_t;
};
