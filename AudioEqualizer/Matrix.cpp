#include "stdafx.h"

#include "Matrix.h"

#include <iostream>
#include <iomanip>
#include <math.h>

#include <random>

//========================================================================
//
int getRandomEntry(int min, int max)
{
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(rng);
}

//========================================================================
//
Matrix::Matrix( size_t size )
    : sz_( size )
{
    matrix_.resize( sz_, std::vector<double>( sz_, 0 ) );
}

//========================================================================
//
void Matrix::print()
{
    for( Uint i = 0; i < this->sz_; i++ )
    {
        for( Uint j = 0; j < this->sz_; j++ )
        {
            std::cout << std::setprecision( 4 ) << matrix_[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

//========================================================================
//
void Matrix::transpose()
{
    Matrix t_mat( this->sz_ );

    for( Uint i = 0; i < this->sz_; i++ )
    {
        for( Uint j = 0; j < this->sz_; j++ )
        {
            t_mat.matrix_[i][j] = this->matrix_[j][i];
        }
    }

    // OPT should be able to std::move the whole thing..
    //matrix_ = std::move( t_mat.matrix_ );

    for ( Uint i = 0; i < this->sz_; ++i )
    { 
        matrix_[i] = t_mat.matrix_[i];
    }
}

//========================================================================
//
void Matrix::makeRandom( int low, int hi )
{
    for( Uint i = 0; i < this->sz_; i++ )
    {
        for( Uint j = 0; j < this->sz_; j++ )
    	{
            matrix_[i][j] = getRandomEntry( low, hi );
    	}
    }
}

//========================================================================
//
void Matrix::scaleBy( double s )
{
    for( Uint i = 0; i < this->sz_; i++ )
    {
        for( Uint j = 0; j < this->sz_; j++ )
        {
            matrix_[i][j] = matrix_[i][j] * s;
        }
    }
}

//========================================================================
//
Matrix Matrix::operator*( const Matrix& rhs )
{
    if( rhs.sz_ != this->sz_ )
    {
        // output err
        std::cout << "Matrix dimensions do not agree: " << this->sz_ << " " << rhs.sz_ << std::endl;
    }

    Matrix result( this->sz_ );

    for( Uint i = 0; i < this->sz_; i++ )
    {
        for( Uint j = 0; j < this->sz_; j++ )
        {
            double sum = 0.0;
            for( Uint k = 0; k < this->sz_; k++ )
            {
                sum += this->matrix_[i][k] * rhs.matrix_[k][j];
            }
            result.matrix_[i][j] = sum;
        }
    }

    return result;
}

//========================================================================
//
std::vector<double> Matrix::operator*( const std::vector<double>& vec )
{
    if( this->sz_ != vec.size() )
    {
        // output err
        std::cout << "Matrix::operator* - Matrix dimensions do not agree with vector length: " << this->sz_ << " " << vec.size() << std::endl;
    }

    std::vector<double> result( this->sz_, 0.0 );

    for( Uint i = 0; i < this->sz_; i++ )
    {
        double sum = 0.0;

        for( Uint j = 0; j < this->sz_; j++ )
        {
            sum += this->matrix_[i][j] * vec[j];
        }

        result[i] = sum;
    }

    return std::move( result );
}
