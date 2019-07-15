#include "stdafx.h"

#ifdef _MATRIX_

#include <iostream>
#include <iomanip>
#include <math.h>

namespace Math
{

//#include <random>

////========================================================================
////
//int getRandomEntry(int min, int max)
//{
//    std::random_device device;
//    std::mt19937 rng(device());
//    std::uniform_int_distribution<int> distribution(min, max);
//
//    return distribution(rng);
//}

//========================================================================
//
template<typename T>
Matrix<T>::Matrix( size_t size )
    : sz_( size )
{
    matrix_.resize( sz_, std::vector<T>( sz_, T() ) );
}

//========================================================================
//
//template<typename T>
//void Matrix<T>::print()
//{
//    for( Uint i = 0; i < this->sz_; i++ )
//    {
//        for( Uint j = 0; j < this->sz_; j++ )
//        {
//            std::cout << std::setprecision( 4 ) << matrix_[i][j] << "\t";
//        }
//        std::cout << std::endl;
//    }
//}

//========================================================================
//
template<typename T>
void Matrix<T>::transpose()
{
    Matrix<T> t_mat( this->sz_ );

    for( Uint i = 0; i < this->sz_; i++ )
    {
        for( Uint j = 0; j < this->sz_; j++ )
        {
            t_mat.matrix_[i][j] = this->matrix_[j][i];
        }
    }

    // OPT should be able to std::move the whole thing..
    //matrix_ = std::move( t_mat.matrix_ );

    for( Uint i = 0; i < this->sz_; ++i )
    {
        matrix_[i] = t_mat.matrix_[i];
    }
}

//========================================================================
//
//template<typename T>
//void Matrix<T>::makeRandom( int low, int hi )
//{
//    for( Uint i = 0; i < this->sz_; i++ )
//    {
//        for( Uint j = 0; j < this->sz_; j++ )
//    	{
//            matrix_[i][j] = getRandomEntry( low, hi );
//    	}
//    }
//}

//========================================================================
//
template<typename T>
void Matrix<T>::scaleBy( double s )
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
template<typename T>
Matrix<T> Matrix<T>::operator*( const Matrix<T>& rhs ) const
{
    if( rhs.sz_ != this->sz_ )
    {
        // output err
        std::cout << "Matrix dimensions do not agree: " << this->sz_ << " " << rhs.sz_ << std::endl;
    }

    Matrix<T> result( this->sz_ );

    for( Uint i = 0; i < this->sz_; i++ )
    {
        for( Uint j = 0; j < this->sz_; j++ )
        {
            T sum = T();
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
template<typename T>
std::vector<T> Matrix<T>::operator*( const std::vector<T>& vec ) const
{
    if( this->sz_ != vec.size() )
    {
        // output err
        std::cout << "Matrix::operator* - Matrix dimensions do not agree with vector length: " << this->sz_ << " " << vec.size() << std::endl;
    }

    std::vector<T> result( this->sz_, T() );

    for( Uint i = 0; i < this->sz_; i++ )
    {
        T sum = T();

        for( Uint j = 0; j < this->sz_; j++ )
        {
            sum += this->matrix_[i][j] * vec[j];
        }

        result[i] = sum;
    }

    return std::move( result );
}

//========================================================================
//
template<typename T>
std::vector<T>& Matrix<T>::operator[]( size_t row )
{
    return matrix_[row];
}

//========================================================================
//
template<typename T>
const std::vector<T>& Matrix<T>::operator[]( size_t row ) const
{
    return matrix_[row];
}

}

#endif
