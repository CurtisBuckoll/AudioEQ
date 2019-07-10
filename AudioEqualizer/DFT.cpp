#include "stdafx.h"

#include "DFT.h"

#include <array>
#include <iostream>

#define PI 3.14159265359

//========================================================================
//
DFT::DFT( size_t mat_size )
    : sz_( mat_size )
    , T( sz_ )
    , T_t( sz_ )
{
    // Construct the size sz_ DCT Matrices (forward and inverse).
    const size_t N = sz_;
    for( Uint i = 0; i < N; ++i )
    {
        for( Uint j = 0; j < N; ++j )
        {
            const double a = i == 0 ? sqrt( 1.0 / N ) : sqrt( 2.0 / N );
            double entry = a * std::cos( ( ( 2.0 * j + 1.0 ) * i * PI ) / ( 2.0 * N ) );
            //T.matrix_[i][j] = entry;
        }
    }

    T_t = T;
    T_t.transpose();
};

//========================================================================
//
Math::Matrix<Math::Complex> DFT::transform( const Math::Matrix<Math::Complex>& block )
{
    return T * block * T_t;
}

//========================================================================
//
std::vector<Math::Complex> DFT::transform( const std::vector<Math::Complex>& chunk )
{
    return std::move( T * chunk );
}

//========================================================================
//
Math::Matrix<Math::Complex> DFT::inverse_transform( const Math::Matrix<Math::Complex>& block )
{
    return T_t * block * T;
}

//========================================================================
//
std::vector<Math::Complex> DFT::inverse_transform( const std::vector<Math::Complex>& chunk )
{
    return std::move( T_t * chunk );
}
