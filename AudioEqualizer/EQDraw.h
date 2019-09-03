#pragma once

#include "IWindow.h"

#include <vector>
#include <cmath>

struct vec2;

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

    double eq_center_;
    double eq_gain_scale_;
    double q_scale;
    bool update_curve_;

    const double EQ_MOVE_PEAK_SPEED;
};
