#include "stdafx.h"

#include "DCT.h"

#include <array>
#include <iostream>

#define PI 3.14159265359

//========================================================================
//
DCT::DCT( size_t mat_size )
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
            T.matrix_[i][j] = entry;
        }
    }

    T_t = T;
    T_t.transpose();
};

//========================================================================
//
Matrix DCT::transform( const Matrix& block )
{
    return T * block * T_t;
}

//========================================================================
//
std::vector<double> DCT::transform( const std::vector<double>& chunk )
{
    return std::move( T * chunk );
}

//========================================================================
//
Matrix DCT::inverse_transform( const Matrix& block )
{
    return T_t * block * T;
}

//========================================================================
//
std::vector<double> DCT::inverse_transform( const std::vector<double>& chunk )
{
    return std::move( T_t * chunk );
}
