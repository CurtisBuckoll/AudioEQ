#include "stdafx.h"

#include "EQDraw.h"

#include "InputManager.h"
#include "Line.h"

#include <array>
#include <algorithm>
#include <cmath>

// delete later
#include <iostream>

//static const double kEQ_MOVE_PEAK_SPEED = 0.3;
static const double kGAIN_SCALE_SPEED = 4.0;
static const double kQ_SCALE_SPEED = 1.3;
static const double kMIN_Q = 1.5;
static const double kMAX_Q = 30.0;

static const double kFREQ_SPECTRUM_HEIGHT_DAMPENING = 40.0; // 40.0

static const double kPI = 3.141592653589793;
static const double kE  = 2.718281828459045;

static const Color256 kEQ_CURVE_COLOUR( 255, 255, 255 );
static const Color256 kSPECTRUM_ANALYSIS_COLOUR( 0, 0, 155 );

// =======================================================================
//
EQDraw::EQDraw( IWindow& window, const size_t num_samples )
    : window_( window )
    , eq_sample_sz_( num_samples )
    , eq_coeffs_( num_samples, 0.0 )
    , line_colour_( 255, 255, 255 )
    , eq_gain_scale_( 0.0 )
    , q_scale( ( kMIN_Q + kMAX_Q ) / 2.0 )
    , update_curve_( false )
    , EQ_MOVE_PEAK_SPEED( 0.4 * std::log2( num_samples ) )
{
    //eq_center_ = static_cast<double>( window.getWidth() / 2 );
    eq_center_ = static_cast<double>( eq_sample_sz_ / 2 );
}


// =======================================================================
//
EQDraw::~EQDraw()
{

}

// =======================================================================
//
void EQDraw::processUserInput( bool* keys )
{
    if( keys[SDLK_w] )
    {
        eq_gain_scale_ += kGAIN_SCALE_SPEED;
        update_curve_ = true;
    }
    if( keys[SDLK_s] )
    {
        eq_gain_scale_ -= kGAIN_SCALE_SPEED;
        update_curve_ = true;
    }
    if( keys[SDLK_a] )
    {
        eq_center_ -= EQ_MOVE_PEAK_SPEED;
        update_curve_ = true;
    }
    if( keys[SDLK_d] )
    {
        eq_center_ += EQ_MOVE_PEAK_SPEED;
        update_curve_ = true;
    }
    if( keys[kWHEEL_UP] )
    {
        q_scale += kQ_SCALE_SPEED;
        if( q_scale > kMAX_Q )
        {
            q_scale = kMAX_Q;
        }
        update_curve_ = true;
        keys[kWHEEL_UP] = false;
    } 
    if( keys[kWHEEL_DOWN] )
    {
        q_scale -= kQ_SCALE_SPEED;
        if( q_scale < kMIN_Q )
        {
            q_scale = kMIN_Q;
        }
        update_curve_ = true;
        keys[kWHEEL_DOWN] = false;
    }
}

// =======================================================================
//
const std::vector<double>& EQDraw::getEqCoeffsBuffer() const
{
    return eq_coeffs_;
}

// =======================================================================
// Expect input to be in the [0..1] range. we will transform here
// fit the window or some parameterized size.
void EQDraw::drawToWindow()
{
    if( update_curve_ )
    {
        // This will be true when user interaction has occured with the
        // curve parameters, so we must update.
        updateEQCoeffs();
        update_curve_ = false;
    }

    vec2 prev( 0.0, eq_coeffs_[0] + static_cast<double>( window_.getHeight() / 2 ) );

    for( size_t i = 1; i < eq_coeffs_.size(); ++i )
    {
        vec2 next( ( i / static_cast<double>(eq_coeffs_.size()) ) * window_.getWidth(), ( window_.getHeight() / 2 ) + eq_coeffs_[i] );

        //drawLine( prev, next, kEQ_CURVE_COLOUR );
        Line::getInstance().antialiased( prev, next, kEQ_CURVE_COLOUR, window_ );

        prev = next;
    }

    //drawLine( vec2( 0.0, 0.0 ), vec2( 300, 200 ), kEQ_CURVE_COLOUR );
}

// -------------------------------------------------------------
//
void EQDraw::drawSpectrumTowindow( const std::vector<double>& freq_spectrum )
{
    vec2 prev( 0.0, 0.0 );

    for( size_t i = 1; i < freq_spectrum.size(); ++i )
    {
        vec2 next( ( i / static_cast<double>( freq_spectrum.size() ) ) * window_.getWidth(), 
                     std::abs( freq_spectrum[i] ) / kFREQ_SPECTRUM_HEIGHT_DAMPENING );

        //drawLine( prev, next, kSPECTRUM_ANALYSIS_COLOUR );
        Line::getInstance().antialiased( prev, next, kSPECTRUM_ANALYSIS_COLOUR, window_ );

        prev = next;
    }
}

// =======================================================================
//
void EQDraw::updateEQCoeffs()
{
    // Compute Gaussian bell curve..
    double mu = eq_center_; // / window_.getWidth();
    double sigma = q_scale;

    double coeff = 1.0 / ( std::sqrt( 2.0 * kPI * pow( sigma, 2 ) ) );
    double exp_coeff = std::pow( kE, -1.0 / ( 2.0 * pow( sigma, 2 ) ) );

    // We need this to normalize the values.
    double peak_height = coeff; // * 1

    for( size_t i = 0; i < eq_coeffs_.size(); ++i )
    {
        double coeff_val = coeff * std::pow( exp_coeff, std::pow( static_cast<double>( i ) - mu, 2 ) );
        eq_coeffs_[i] = ( coeff_val / peak_height ) * eq_gain_scale_ ;

        //std::cout << eq_coeffs_[i] << std::endl;
    }
}

// =======================================================================
//
void EQDraw::drawLine( const vec2& p1, const vec2& p2, Color256 colour )
{
    static const double line_width = 3.0;

    if( update_curve_ )
    {
        // This will be true when user interaction has occured with the
        // curve parameters, so we must update.
        updateEQCoeffs();
        update_curve_ = false;
    }

    //if( p2.x_ < p1.x_ )
    //{
    //    std::swap( p1, p2 );
    //}

    double slope = static_cast<double>( p2.y_ - p1.y_ ) / static_cast<double>( p2.x_ - p1.x_ );
    
    // n := the normal to the line.
    vec2 n( p1.y_ - p2.y_, p2.x_ - p1.x_ );
    n.normalize();

    //std::cout << n.dot( vec2( p2.x_ - p1.x_, p2.y_ - p1.y_ ) ) << std::endl;
    //std::cout << n.len() << std::endl;

    // d is direction of line.
    vec2 d( ( p2.x_ - p1.x_ ), ( p2.y_ - p1.y_ ) );
    d.normalize();
    
    // p is a point on the line.
    vec2 p = vec2( p1.x_ - 20.0 * d.x_, p1.y_ - 20.0 * d.y_ );

    if( slope >= 0.0 )
    {
        if( slope > 1.0 )
        {
            slope = 1.0 / slope;
            double x = p1.x_;
            for( int y = static_cast<int>( std::round( p1.y_ ) ); y < static_cast<int>( std::round( p2.y_ ) ); ++y )
            {
                window_.setPixel( static_cast<int>( std::round( x ) ), y, colour );
                x += slope;
            }
        }
        else
        {
            double y = p1.y_;
            for( int x = static_cast<int>( std::round( p1.x_ ) ); x < static_cast<int>( std::round( p2.x_ ) ); ++x )
            {
                vec2 v( x - p.x_, y - p.y_ );
                double dist_scalar = std::abs( n.dot( v ) );
                if( dist_scalar <= line_width )
                {
                    double dissolve_factor = 1.0 - ( dist_scalar / line_width );
                    window_.setPixel( std::round( x ), static_cast<int>( std::round<int>( y ) ), colour );
                }

                // Set pixel above and below.
                v = vec2( x - p.x_, y + 1 - p.y_ );
                dist_scalar = std::abs( n.dot( v ) );
                if( dist_scalar <= line_width )
                {
                    double dissolve_factor = 1.0 - ( dist_scalar / line_width );
                    //std::cout << dissolve_factor << std::endl;
                    window_.setPixel( std::round( x ), static_cast<int>( std::round( y + 1 ) ), colour * dissolve_factor );
                }

                v = vec2( x - p.x_, y - 1 - p.y_ );
                dist_scalar = std::abs( n.dot( v ) );
                if( dist_scalar <= line_width )
                {
                    double dissolve_factor = 1.0 - ( dist_scalar / line_width );
                    window_.setPixel( std::round( x ), static_cast<int>( std::round( y - 1 ) ), colour * dissolve_factor );
                }
                
                v = vec2( x - p.x_, y + 2 - p.y_ );
                dist_scalar = std::abs( n.dot( v ) );
                if( dist_scalar <= line_width )
                {
                    double dissolve_factor = 1.0 - ( dist_scalar / line_width );
                    //std::cout << dissolve_factor << std::endl;
                    window_.setPixel( std::round( x ), static_cast<int>( std::round( y + 2 ) ), colour * dissolve_factor );
                }

                v = vec2( x - p.x_, y - 2 - p.y_ );
                dist_scalar = std::abs( n.dot( v ) );
                if( dist_scalar <= line_width )
                {
                    double dissolve_factor = 1.0 - ( dist_scalar / line_width );
                    window_.setPixel( std::round( x ), static_cast<int>( std::round( y - 2 ) ), colour * dissolve_factor );
                }

                y += slope;
            }

        }
    }
    else
    {
        if( slope < -1.0 )
        {
            slope = -1.0 / slope;
            double x = p1.x_;
            for( int y = static_cast<int>( std::round( p1.y_ ) ); y > static_cast<int>( std::round( p2.y_ ) ); --y )
            {
                window_.setPixel( static_cast<int>( std::round( x ) ), y, colour );
                x += slope;
            }
        }
        else
        {
            double y = p1.y_;
            for( int x = static_cast<int>( std::round( p1.x_ ) ); x < static_cast<int>( std::round( p2.x_ ) ); ++x )
            {
                window_.setPixel( std::round( x ), static_cast<int>( std::round<int>( y ) ), colour );
                y += slope;
            }

        }
    }

    //double y = p1.y_;
    //for( int x = static_cast<int>( std::round( p1.x_ ) ); x < static_cast<int>( std::round( p2.x_ ) ); ++x )
    //{
    //    window_.setPixel( x, static_cast<int>( std::round<int>( y ) ), line_colour_ );
    //    y += slope;
    //}

    // For test purposes...
    //window_.setPixel( eq_center_, static_cast<int>( std::round<int>( y + eq_gain_scale_ ) ), line_colour_ );
}


//#include "stdafx.h"
//
//#include "EQDraw.h"
//#include "InputManager.h"
//
//#include <array>
//#include <algorithm>
//
//// delete later
//#include <iostream>
//
//static const double kEQ_MOVE_PEAK_SPEED = 0.5;
//static const double kGAIN_SCALE_SPEED = 0.5;
//static const double kQ_SCALE_SPEED = 0.005;
//
//
//static const double kPI = 3.141592653589793;
//static const double kE = 2.718281828459045;
//
//// =======================================================================
////
//EQDraw::EQDraw( IWindow& window, const size_t num_samples )
//    : window_( window )
//    , eq_sample_sz_( num_samples )
//    , eq_coeffs_( num_samples, 0.0 )
//    , line_colour_( 255, 255, 255 )
//    , eq_gain_scale_( 0.0 )
//    , q_scale( 0.0 )
//    , update_curve_( false )
//{
//    eq_center_ = static_cast<double>( window.getWidth() / 2 );
//}
//
//
//// =======================================================================
////
//EQDraw::~EQDraw()
//{
//
//}
//
//// =======================================================================
////
//void EQDraw::processUserInput( bool* keys )
//{
//    if( keys[SDLK_w] )
//    {
//        eq_gain_scale_ += kGAIN_SCALE_SPEED;
//        update_curve_ = true;
//    }
//    if( keys[SDLK_s] )
//    {
//        eq_gain_scale_ -= kGAIN_SCALE_SPEED;
//        update_curve_ = true;
//    }
//    if( keys[SDLK_a] )
//    {
//        eq_center_ -= kEQ_MOVE_PEAK_SPEED;
//        update_curve_ = true;
//    }
//    if( keys[SDLK_d] )
//    {
//        eq_center_ += kEQ_MOVE_PEAK_SPEED;
//        update_curve_ = true;
//    }
//    if( keys[kWHEEL_UP] )
//    {
//        q_scale += kQ_SCALE_SPEED;
//        update_curve_ = true;
//        keys[kWHEEL_UP] = false;
//    }
//    if( keys[kWHEEL_DOWN] )
//    {
//        q_scale -= kQ_SCALE_SPEED;
//        if( q_scale < 0 )
//        {
//            q_scale = 0.0000000000001;
//        }
//        update_curve_ = true;
//        keys[kWHEEL_DOWN] = false;
//    }
//}
//
//// =======================================================================
//// Expect input to be in the [0..1] range. we will transform here
//// fit the window or some parameterized size.
//void EQDraw::drawToWindow()
//{
//    vec2 prev( 0.0, eq_coeffs_[0] + ( window_.getHeight() / 2 ) );
//
//    for( size_t i = 1; i < eq_coeffs_.size(); ++i )
//    {
//        vec2 next( ( i / static_cast<double>( eq_coeffs_.size() ) ) * window_.getWidth(), ( window_.getHeight() / 2 ) + eq_coeffs_[i] );
//
//        drawLine( prev, next );
//
//        prev = next;
//    }
//}
//
//// =======================================================================
////
//void EQDraw::updateEQCoeffs()
//{
//    // Compute Gaussian bell curve..
//    double mu = eq_center_ / window_.getWidth();
//    double sigma = 0.1 + q_scale;
//
//    double coeff = 1.0 / ( std::sqrt( 2.0 * kPI * pow( sigma, 2 ) ) );
//    double exp_coeff = std::pow( kE, -1.0 / ( 2.0 * pow( sigma, 2 ) ) );
//
//    // We need this to normalize the values.
//    double peak_height = coeff; // * 1
//
//    for( size_t i = 0; i < eq_coeffs_.size(); ++i )
//    {
//        double normalized_index = static_cast<double>( i ) / eq_coeffs_.size();
//        double coeff_val = coeff * std::pow( exp_coeff, std::pow( normalized_index - mu, 2 ) );
//
//        eq_coeffs_[i] = ( coeff_val / peak_height ) * eq_gain_scale_;
//
//        //std::cout << eq_coeffs_[i] / eq_scale_ << std::endl;
//    }
//
//    return;
//}
//
//// =======================================================================
////
//void EQDraw::drawLine( vec2 p1, vec2 p2 )
//{
//    if( update_curve_ )
//    {
//        // This will be true when user interaction has occured with the
//        // curve parameters, so we must update.
//        updateEQCoeffs();
//        update_curve_ = false;
//    }
//
//    if( p2.x_ < p1.x_ )
//    {
//        std::swap( p1, p2 );
//    }
//
//    double slope = static_cast<double>( p2.y_ - p1.y_ ) / static_cast<double>( p2.x_ - p1.x_ );
//    double y = p1.y_;
//
//    for( int x = static_cast<int>( std::round( p1.x_ ) ); x < static_cast<int>( std::round( p2.x_ ) ); ++x )
//    {
//        window_.setPixel( x, static_cast<int>( std::round<int>( y ) ), line_colour_ );
//        y += slope;
//    }
//
//    // For test purposes...
//    //window_.setPixel( eq_center_, static_cast<int>( std::round<int>( y + eq_gain_scale_ ) ), line_colour_ );
//}
//
