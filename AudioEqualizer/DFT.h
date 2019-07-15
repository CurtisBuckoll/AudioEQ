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
    std::vector<Math::Complex> transform( const std::vector<Math::Complex>& signal );

    //--------------------------------------------------------------
    //
    std::vector<Math::Complex> inverse_transform( const std::vector<Math::Complex>& signal );

    //--------------------------------------------------------------
    //
    void print( const Math::Matrix<Math::Complex>& M ) const;

    //--------------------------------------------------------------
    //
    void test() const;

private:

    //--------------------------------------------------------------
    //
    DFT() = delete;

    //--------------------------------------------------------------
    //
    void fillMatrix( Math::Complex omega, Math::Matrix<Math::Complex>& M );

    //--------------------------------------------------------------
    //
    size_t sz_;
    Math::Matrix<Math::Complex> T;
    Math::Matrix<Math::Complex> T_inv;
};
