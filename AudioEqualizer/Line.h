#pragma once

#include <iostream>

#include "Vertex.h"

//========================================================================
//

class IWindow;

static const size_t kLUT_SIZE = 2000;

//========================================================================
//
struct vec2 {

    // -------------------------------------------------------------
    //
    vec2()
        : x_( 0.0 )
        , y_( 0.0 )
    { }

    // -------------------------------------------------------------
    //
    vec2( double x, double y )
        : x_( x )
        , y_( y )
    { }

    // -------------------------------------------------------------
    //
    double dot( vec2 other )
    {
        return x_ * other.x_ + y_ * other.y_;
    }

    // -------------------------------------------------------------
    //
    double len()
    {
        return std::sqrt( x_ * x_ + y_ * y_ );
    }

    // -------------------------------------------------------------
    //
    void normalize()
    {
        const double length = len();
        if( length > 0.0 )
        {
            x_ = x_ / length;
            y_ = y_ / length;
        }
    }

    // -------------------------------------------------------------
    //
    vec2 operator+( vec2 rhs )
    {
        vec2 res( x_ + rhs.x_, y_ + rhs.y_ );
        return std::move( res );
    }

    // -------------------------------------------------------------
    //
    vec2 operator-( vec2 rhs )
    {
        vec2 res( x_ - rhs.x_, y_ - rhs.y_ );
        return std::move( res );
    }

    // -------------------------------------------------------------
    //
    double x_;
    double y_;
};

//========================================================================
//
class Line
{
public:

    // -------------------------------------------------------------
    //
    void antialiased( vec2 p0, 
                      vec2 p1, 
                      Color256 color, 
                      IWindow& window );

    // -------------------------------------------------------------
    //
    static Line& getInstance() 
    {
        static Line instance( kLUT_SIZE );
        return instance;
    }

    // -------------------------------------------------------------
    // Delete these to enforce singleton.
    Line( Line const& )           = delete;
    void operator=( Line const& ) = delete;

private:

    // -------------------------------------------------------------
    //
    Line( size_t area_lut_sz );

    // -------------------------------------------------------------
    //
    ~Line();

    // -------------------------------------------------------------
    //
    double* area_lookup_;
    size_t  lut_sz_;
};
