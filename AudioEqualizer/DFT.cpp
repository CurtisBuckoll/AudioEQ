#include "stdafx.h"

#include "DFT.h"

#include "Util.h"

#include <iostream>
#include <cmath>

//========================================================================
//
void DFT::fillMatrix( Math::Complex omega, Math::Matrix<Math::Complex>& M )
{
    const size_t N = sz_;

    // Construct the size N = sz_ DFT Matrices
    for( Uint i = 0; i < N; ++i )
    {
        Math::Complex row_exp( 0.0, 0.0 );

        for( Uint j = 0; j < N; ++j )
        {
            Math::Complex entry( 1.0, 0.0 );

            if( i > 0 && j > 0 )
            {
                if( j == 1 )
                {
                    row_exp = M[i - 1][j] * omega;
                    entry = row_exp;
                }
                else
                {
                    entry = M[i][j - 1] * row_exp;
                }
            }

            M[i][j] = entry; 
        }
    }

    // To normalize the entries.
    const double norm_factor = 1.0 / std::sqrt( static_cast<double>( N ) );
    M.scaleBy( norm_factor );
}

//========================================================================
//
DFT::DFT( size_t mat_size )
    : sz_( mat_size )
    , T( sz_ )
    , T_inv( sz_ )
{
    // Construct the size N = sz_ DFT Matrices, (forward T and inverse T_t)
    Math::Complex omega( std::cos( -2.0 * PI / static_cast<double>( sz_ ) ), std::sin( -2.0 * PI / static_cast<double>( sz_ ) ) );
    fillMatrix( omega, T );
    omega = Math::Complex( std::cos( 2.0 * PI / static_cast<double>( sz_ ) ), std::sin( 2.0 * PI / static_cast<double>( sz_ ) ) );
    fillMatrix( omega, T_inv );

    // Construct the size N = sz_ DFT Matrices (forward and inverse).
    //for( Uint i = 0; i < N; ++i )
    //{
    //    Math::Complex row_exp( 0.0, 0.0 );

    //    for( Uint j = 0; j < N; ++j )
    //    {
    //        Math::Complex entry( 1.0, 0.0 );

    //        if( i > 0 && j > 0)
    //        {
    //            if( j == 1 )
    //            {
    //                row_exp = T.matrix_[i - 1][j] * omega;
    //                entry   = row_exp;
    //            }
    //            else
    //            {
    //                entry   = T.matrix_[i][j - 1] * row_exp;
    //            }
    //        }

    //        T.matrix_[i][j] = entry;
    //    }
    //}
};

//========================================================================
//
std::vector<Math::Complex> DFT::transform( const std::vector<Math::Complex>& signal )
{
    return std::move( T * signal );
}

//========================================================================
//
std::vector<Math::Complex> DFT::inverse_transform( const std::vector<Math::Complex>& signal )
{
    return std::move( T_inv * signal );
}

//========================================================================
//
void DFT::print(const Math::Matrix<Math::Complex>& M) const
{
    static const double close_to_zero = 0.00000000001;

    std::cout << "Real:" << std::endl;
    for( Uint i = 0; i < sz_; ++i )
    {
        for( Uint j = 0; j < sz_; ++j )
        {
            double entry = std::abs( M[i][j].re() ) < close_to_zero ? 0.0 : M[i][j].re();
            std::cout << std::setprecision( 4 ) << entry << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "Imaginary:" << std::endl;
    for( Uint i = 0; i < sz_; ++i )
    {
        for( Uint j = 0; j < sz_; ++j )
        {
            double entry = std::abs( M[i][j].im() ) < close_to_zero ? 0.0 : M[i][j].im();
            std::cout << std::setprecision( 4 ) << entry << "\t";
        }
        std::cout << std::endl;
    }
}

//========================================================================
//
void DFT::test() const
{
    std::cout << "Transform:" << std::endl;
    print( T );
    std::cout << "Inverse:" << std::endl;
    print( T_inv );
    std::cout << "Multiplying matrices:" << std::endl;
    Math::Matrix<Math::Complex> product( sz_ );
    product = T_inv * T;
    print( product );
}
