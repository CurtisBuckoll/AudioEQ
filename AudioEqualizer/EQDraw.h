#pragma once

#include "Window.h"

#include <vector>

// -------------------------------------------------------------
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
    vec2(int x, int y)
        : x_( x )
        , y_( y )
    { }

    // -------------------------------------------------------------
    //
    double x_;
    double y_;
};

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
    void drawLine( vec2 p1, vec2 p2 );

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
