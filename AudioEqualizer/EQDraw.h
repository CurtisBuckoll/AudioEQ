#pragma once

#include "IWindow.h"

#include <vector>
#include <cmath>

struct vec2;

//// -------------------------------------------------------------
////
//struct vec2 {
//
//    // -------------------------------------------------------------
//    //
//    vec2()
//        : x_( 0.0 )
//        , y_( 0.0 )
//    { }
//
//    // -------------------------------------------------------------
//    //
//    vec2( double x, double y )
//        : x_( x )
//        , y_( y )
//    { }
//
//    // -------------------------------------------------------------
//    //
//    double dot( vec2 other )
//    {
//        return x_ * other.x_ + y_ * other.y_;
//    }
//
//    // -------------------------------------------------------------
//    //
//    double len()
//    {
//        return std::sqrt( x_ * x_ + y_ * y_ );
//    }
//
//    // -------------------------------------------------------------
//    //
//    void normalize()
//    {
//        const double length = len();
//        if( length > 0.0 )
//        {
//            x_ = x_ / length;
//            y_ = y_ / length;
//        }
//    }
//
//    // -------------------------------------------------------------
//    //
//    double x_;
//    double y_;
//};

// -------------------------------------------------------------
//
class EQDraw
{
public:   

    // -------------------------------------------------------------
    //
    EQDraw( IWindow& window, const size_t eq_sample_size );

    // -------------------------------------------------------------
    //
    void processUserInput( bool* keys );

    // -------------------------------------------------------------
    //
    const std::vector<double>& getEqCoeffsBuffer() const;

    // -------------------------------------------------------------
    //
    void drawToWindow();

    // -------------------------------------------------------------
    //
    void drawSpectrumTowindow( const std::vector<double>& freq_spectrum );

    // -------------------------------------------------------------
    //
    ~EQDraw();

private:

    // -------------------------------------------------------------
    //
    EQDraw() = delete;

    // -------------------------------------------------------------
    //
    void updateEQCoeffs();


    // -------------------------------------------------------------
    //
    void drawLine( const vec2& p1, const vec2& p2, Color256 colour );

    // -------------------------------------------------------------
    // Member Variables
    // -------------------------------------------------------------

    IWindow& window_;
    size_t eq_sample_sz_;
    Color256 line_colour_;
    std::vector<double> eq_coeffs_;

    // For the testing EQ curve
    double eq_center_;
    double eq_gain_scale_;
    double q_scale;
    bool update_curve_;

    const double EQ_MOVE_PEAK_SPEED;
};
