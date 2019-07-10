#include "stdafx.h"
#include "Complex.h"

#include <cmath>
#include <iostream>

namespace math
{
//========================================================================
//
Complex::Complex()
    : _re( 0.0 )
    , _im( 0.0 )
{

}

//========================================================================
//
Complex::Complex( double re,
                  double im )
    : _re( re )
    , _im( im )
{

}

//========================================================================
//
Complex::~Complex()
{

}

//========================================================================
//
double Complex::magnitude()
{
    return std::sqrt( _re * _re + _im * _im );
}

//========================================================================
// (a+ib)(c+id) = (ac - bd) + i(ad + bc)
Complex Complex::operator*( const Complex& rhs )
{
    Complex res;
    res._re = _re * rhs.re() - _im * rhs.im();
    res._im = _re * rhs.im() + _im * rhs.re();
    return std::move( res );
}

//========================================================================
//
Complex Complex::operator+( const Complex& rhs )
{
    Complex res;
    res._re = _re + rhs.re();
    res._im = _im + rhs.im();
    return std::move( res );
}

//========================================================================
//
Complex Complex::operator-( const Complex& rhs )
{
    Complex res;
    res._re = _re - rhs.re();
    res._im = _im - rhs.im();
    return std::move( res );
}

//========================================================================
//
Complex& Complex::operator=( const Complex& rhs )
{
    _re = rhs.re();
    _im = rhs.im();
    return *this;
}

//========================================================================
//
void print( const Complex& c )
{
    std::cout << "Re: " << c.re() << " Im: " << c.im() << std::endl;
}

//========================================================================
//
void Complex::test()
{
    Complex a( 1.0, 0.5 );
    std::cout << "a: "; print( a );
    Complex b( 2.0, 1.5 );
    std::cout << "b: "; print( b );
    Complex c = a + b;
    std::cout << "c: "; print( c );

    Complex d( 5.0, 7.0 );
    std::cout << "d: "; print( d );
    Complex e( 4.0, 3.0 );
    std::cout << "e: "; print( e );
    Complex f = d - e;
    std::cout << "f: "; print( f );

    Complex g = c * f;
    print( g );

    std::cout << "\n";
    std::cout << "a: "; print( a );
    std::cout << "b: "; print( b );
    std::cout << "c: "; print( c );
    std::cout << "d: "; print( d );
    std::cout << "e: "; print( e );
    std::cout << "f: "; print( f );

    std::cout << "( 3 + 2i )( 1 + 7i ) = "; print( Complex( 3, 2 ) * Complex( 1, 7 ) );
}
}