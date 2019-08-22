#include "stdafx.h"
#include "Line.h"
#include <math.h>

#include "Window.h"

#include <cmath>

enum class Octant {
    ONE = 0,
    TWO,
    FOUR,
    THREE,
    EIGHT,
    SEVEN,
    FIVE,
    SIX
};

// for antialiasing:
#define		R			0.7071		// radius
#define		RSQUARED	0.5			// radius^2
#define		PI			3.1415		
#define		LINEWIDTH	1.0


/* DDA/Bresenham Line Drawing Helper Functions ------------------------------------- */


// Swap X, Y coordinates of a point
void swapXY( vec2& p )
{
    int temp = p.x_;
    p.x_ = p.y_;
    p.y_ = temp;
}


// Swap data contained in two points
void swapPoints( vec2& p0, vec2& p1 )
{
    vec2 temp = p0;
    p0 = p1;
    p1 = temp;
}

/* Antialiasing helper functions --------------------------------------------------- */


Color256 getPixelColorAntialiased( const Color256 old_colour, const Color256 color, double coverage )
{
    Color256 new_colour;
    new_colour.r = ( color.r * coverage ) + ( old_colour.r * ( 1 - coverage ) );
    new_colour.g = ( color.g * coverage ) + ( old_colour.g * ( 1 - coverage ) );
    new_colour.b = ( color.b * coverage ) + ( old_colour.b * ( 1 - coverage ) );
    return new_colour;
}


/** Antialiasing - Octant dependent drawing functions:
*  To be returned as function pointers by setDrawFunctionAA(): drawLineAA_Oct1 - drawLineAA_Oct8
*/
void drawLineAA_Oct1( vec2 p0, int x, double y, Color256 color, IWindow& window )
{
    window.setPixel( p0.x_ + x, (int)round( p0.y_ + y ), color );
}
void drawLineAA_Oct2( vec2 p0, int x, double y, Color256 color, IWindow& window )
{
    window.setPixel( (int)round( p0.x_ + y ), p0.y_ + x, color );
}
void drawLineAA_Oct3( vec2 p0, int x, double y, Color256 color, IWindow& window )
{
    window.setPixel( (int)round( p0.x_ + -y ), p0.y_ + x, color );
}
void drawLineAA_Oct4( vec2 p0, int x, double y, Color256 color, IWindow& window )
{
    window.setPixel( p0.x_ + -x, (int)round( p0.y_ + y ), color );
}
void drawLineAA_Oct5( vec2 p0, int x, double y, Color256 color, IWindow& window )
{
    window.setPixel( p0.x_ + -x, (int)round( p0.y_ + -y ), color );
}
void drawLineAA_Oct6( vec2 p0, int x, double y, Color256 color, IWindow& window )
{
    window.setPixel( (int)round( p0.x_ + -y ), p0.y_ + -x, color );
}
void drawLineAA_Oct7( vec2 p0, int x, double y, Color256 color, IWindow& window )
{
    window.setPixel( (int)round( p0.x_ + y ), p0.y_ + -x, color );
}
void drawLineAA_Oct8( vec2 p0, int x, double y, Color256 color, IWindow& window )
{
    window.setPixel( p0.x_ + x, (int)round( p0.y_ + -y ), color );
}


// Function pointer for Antialiased getPixel() functions
typedef void( *voidDrawFnAA )( vec2 p0, int x, double y, Color256 color, IWindow& window );


/** Antialiaised getPixel() functions
*  To be returned as function pointers by getPixelFunction(): drawLineDDA_Oct1 - drawLineDDA_Oct4
*/
Color256 getPixelAA_Oct1( vec2 p0, int x, double y, IWindow& window )
{
    return window.getPixel( p0.x_ + x, (int)round( p0.y_ + y ) );
}
Color256 getPixelAA_Oct2( vec2 p0, int x, double y, IWindow& window )
{
    return window.getPixel( (int)round( p0.x_ + y ), p0.y_ + x );
}
Color256 getPixelAA_Oct3( vec2 p0, int x, double y, IWindow& window )
{
    return window.getPixel( (int)round( p0.x_ + -y ), p0.y_ + x );
}
Color256 getPixelAA_Oct4( vec2 p0, int x, double y, IWindow& window )
{
    return window.getPixel( p0.x_ + -x, (int)round( p0.y_ + y ) );
}
Color256 getPixelAA_Oct5( vec2 p0, int x, double y, IWindow& window )
{
    return window.getPixel( p0.x_ + -x, (int)round( p0.y_ + -y ) );
}
Color256 getPixelAA_Oct6( vec2 p0, int x, double y, IWindow& window )
{
    return window.getPixel( (int)round( p0.x_ + -y ), p0.y_ + -x );
}
Color256 getPixelAA_Oct7( vec2 p0, int x, double y, IWindow& window )
{
    return window.getPixel( (int)round( p0.x_ + y ), p0.y_ + -x );
}
Color256 getPixelAA_Oct8( vec2 p0, int x, double y, IWindow& window )
{
    return window.getPixel( p0.x_ + x, (int)round( p0.y_ + -y ) );
}


// Function pointer for Antialiased getPixel() functions
typedef Color256( *voidGetPixelFnAA )( vec2 p0, int x, double y, IWindow& window );


// Transform coordinates to octant I and return the corresponding DDA line draw function
voidDrawFnAA getDrawFunctionAA( vec2& v, Octant octant )
{
    switch( octant )
    {
    case Octant::ONE:
        return drawLineAA_Oct1;

    case Octant::TWO:
        swapXY( v );
        return drawLineAA_Oct2;

    case Octant::THREE:
        v.x_ = -v.x_;
        swapXY( v );
        return drawLineAA_Oct3;

    case Octant::FOUR:
        v.x_ = -v.x_;
        return drawLineAA_Oct4;

    case Octant::FIVE:
        v.x_ = -v.x_;
        v.y_ = -v.y_;
        return drawLineAA_Oct5;

    case Octant::SIX:
        v.y_ = -v.y_;
        v.x_ = -v.x_;
        swapXY( v );
        return drawLineAA_Oct6;

    case Octant::SEVEN:
        v.y_ = -v.y_;
        swapXY( v );
        return drawLineAA_Oct7;

    case Octant::EIGHT:
        v.y_ = -v.y_;
        return drawLineAA_Oct8;

    default:
        return nullptr;
    }
}


/* Transform coordinates to octant I and return the corresponding DDA line draw function
*Transformations to vector v to be done by getDrawFunction(), which must be called first.
*/
voidGetPixelFnAA getPixelFunction( Octant octant )
{
    switch( octant )
    {
    case Octant::ONE:
        return getPixelAA_Oct1;

    case Octant::TWO:
        return getPixelAA_Oct2;

    case Octant::THREE:
        return getPixelAA_Oct3;

    case Octant::FOUR:
        return getPixelAA_Oct4;

    case Octant::FIVE:
        return getPixelAA_Oct5;

    case Octant::SIX:
        return getPixelAA_Oct6;

    case Octant::SEVEN:
        return getPixelAA_Oct7;

    case Octant::EIGHT:
        return getPixelAA_Oct8;

    default:
        return nullptr;
    }
}


/* Returns octant ONE through EIGHT that vector v resides in:
* Use for antialiasing
*/
Octant getOctant_ALL( vec2& v )
{
    int octant = 0;

    if( v.y_ <= 0.0 )
    {
        octant |= 0x4;
    }
    if( v.x_ <= 0.0 )
    {
        octant |= 0x2;
    }
    if( std::abs( v.x_ ) < std::abs( v.y_ ) )
    {
        octant |= 0x1;
    }

    return (Octant)octant;
}


/* End Antialiasing help functions ------------------------------------------------- */

// =======================================================================
//
Line::Line( size_t area_lut_sz )
    : area_lookup_( nullptr )
    , lut_sz_( area_lut_sz )
{
    // Construct the area lookup for speed:
    // f_area is a function of x where x is how much of the distance
    // from the considered pixel center to the line center overlaps
    // with line width. It will accept a value between 0 and 2 and 
    // will return a value between 0 and 1 representing the fraction 
    // of area of coverage.
    auto f_area = []( double x ) -> double
    {
        return 2 / PI * ( 0.5 * std::sqrt( 2.0 * x - x * x ) * ( x - 1.0 ) - 0.5 * std::asin( 1.0 - x ) + PI / 4.0 );
    };

    // Add + 1 to lut size to account for full coverage.
    area_lookup_ = new double[area_lut_sz + 1];

    // Build the table:
    for( size_t i = 0; i <= area_lut_sz; ++i )
    {
        if( i == 0 )
        {
            area_lookup_[i] = 0.0;
            continue;
        }
        else if( i == area_lut_sz + 1 )
        {
            area_lookup_[i] = 1.0;
            continue;
        }

        double x = 2.0 * static_cast<double>( i ) / static_cast<double>( area_lut_sz );
        area_lookup_[i] = f_area( x );
        //std::cout << area_lookup_[i] << std::endl;
    }
}

// =======================================================================
//
Line::~Line()
{

}

// =======================================================================
//
void Line::antialiased( vec2 p0, vec2 p1, Color256 color, IWindow& window )
{
    static const int kPIXEL_WRITE_SPAN = 1;
    static const double e = 2.718281828459045235;
    static const double c = 3.0359;

    // Round the coordinates.
    p0 = vec2( std::round( p0.x_ ), std::round( p0.y_ ) );
    p1 = vec2( std::round( p1.x_ ), std::round( p1.y_ ) );

    vec2 v = p1 - p0;
    Octant octant = getOctant_ALL( v );
    voidDrawFnAA lineFunction = getDrawFunctionAA( v, octant );
    voidGetPixelFnAA pixelFunction = getPixelFunction( octant );

    double dx = v.x_;
    double dy = v.y_;
    double slope = dx != 0.0 ? dy / dx : 0.0;
    double coverage;
    Color256 oldColor;
    Color256 newColor;

    double x_range = std::round( v.x_ );

    double y = 0.0;

    for( int x = 0; x <= static_cast<int>( std::round( v.x_ ) ); ++x )
    {
        for( int i = -kPIXEL_WRITE_SPAN; i <= kPIXEL_WRITE_SPAN; i++ )
        {
            int currY = static_cast<int>( std::round( p0.y_ + y ) ) + i;
            double h = static_cast<double>( currY ) - ( p0.y_ + y );
            double D = std::abs( ( dx * h ) / sqrt( dx * dx + dy * dy ) );
            D = D - ( LINEWIDTH / 2 );

            double coverage = 0.0;

            if( D > R )
            {
                continue;
            }
            if( D <= 0.0 )
            {
                D = std::abs( D );
                double D_top = D;
                double D_bot = 2 * R - LINEWIDTH - D;

                D_top = R - D_top;
                double d_coverage = D_top / ( 2.0 * R );
                size_t index = static_cast<size_t>( std::round( d_coverage * lut_sz_ ) );
                coverage = area_lookup_[index];
                if( D_bot < R )
                {
                    D_bot = R - D_bot;
                    double d_coverage = D_bot / ( 2.0 * R );
                    size_t index = static_cast<size_t>( std::round( d_coverage * lut_sz_ ) );
                    coverage += area_lookup_[index];
                    coverage -= 1;
                }
            }
            else
            {
                //double coverage_orig = (double)( ( acos( D / R ) - ( ( D * sqrt( RSQUARED - D * D ) ) / RSQUARED ) ) * ( 1 / PI ) );

                D = R - D;
                double d_coverage = D / ( 2.0 * R );
                size_t index = static_cast<size_t>( std::round( d_coverage * lut_sz_ ) );
                coverage = area_lookup_[index];

                //if( std::abs( coverage - coverage_orig ) > 0.0005 )
                //    std::cout << ( coverage - coverage_orig ) << std::endl;
            }

            oldColor = pixelFunction( p0, x, y + i, window );
            newColor = getPixelColorAntialiased( oldColor, color, coverage );
            lineFunction( p0, x, y + i, newColor, window );
        }

        y += slope;
    }
}


//
//void Line::antialiased( vec2 p0, vec2 p1, Color256 color, IWindow& window )
//{
//    static const int kPIXEL_WRITE_SPAN = 1;
//    static const double e = 2.718281828459045235;
//    static const double c = 3.0359;
//
//    // Round the coordinates.
//    p0 = vec2( std::round( p0.x_ ), std::round( p0.y_ ) );
//    p1 = vec2( std::round( p1.x_ ), std::round( p1.y_ ) );
//
//    vec2 v = p1 - p0;
//    Octant octant = getOctant_ALL( v );
//    voidDrawFnAA lineFunction = getDrawFunctionAA( v, octant );
//    voidGetPixelFnAA pixelFunction = getPixelFunction( octant );
//
//    double dx = v.x_;
//    double dy = v.y_;
//    double slope = dx != 0.0 ? dy / dx : 0.0;
//    double coverage;
//    Color256 oldColor;
//    Color256 newColor;
//
//    double x_range = std::round( v.x_ );
//
//    double y = 0.0;
//    
//    for( int x = 0; x <= static_cast<int>( std::round( v.x_ ) ); ++x )
//    {
//        for( int i = -kPIXEL_WRITE_SPAN; i <= kPIXEL_WRITE_SPAN; i++ )
//        {
//            int currY = static_cast<int>( std::round( p0.y_ + y ) ) + i;
//            double h = static_cast<double>(currY) - ( p0.y_ + y );
//            double D = std::abs( ( dx * h ) / sqrt( dx * dx + dy * dy ) );
//            D = D - ( static_cast<double>( LINEWIDTH ) / 2 );
//
//            double coverage = 0.0;
//
//            if( D > R )
//            {
//                continue;
//            }
//            if( D <= 0.0 )
//            {
//                D = std::abs( D );
//                double D_top = D;
//                double D_bot = 2 * R - LINEWIDTH - D;
//
//                D_top = R - D_top;
//                double d_coverage = D_top / ( 2.0 * R ) * 2.0;
//                coverage = ( 1.0 / ( 1.0 + pow( e, -c * ( d_coverage - 1.0 ) ) ) - 0.0458 ) / ( 0.9542 - 0.0458 );
//                if( D_bot < R )
//                {
//                    D_bot = R - D_bot;
//                    double d_coverage = D_bot / ( 2.0 * R ) * 2.0;
//                    coverage += ( 1.0 / ( 1.0 + pow( e, -c * ( d_coverage - 1.0 ) ) ) - 0.0458 ) / ( 0.9542 - 0.0458 );
//                    coverage -= 1;
//                }
//            }
//            else
//            {
//                double coverage_orig = (double)( ( acos( D / R ) - ( ( D * sqrt( RSQUARED - D * D ) ) / RSQUARED ) ) * ( 1 / PI ) );
//                
//                D = R - D;
//                double d_coverage = D / ( 2.0 * R ) * 2.0;
//                coverage = ( 1.0 / ( 1.0 + pow( e, -c * ( d_coverage - 1.0 ) ) ) - 0.0458 ) / ( 0.9542 - 0.0458 );
//
//                if( std::abs(coverage - coverage_orig) > 0.06 )
//                    std::cout << ( coverage - coverage_orig ) << std::endl;
//            }
//
//            
//            //if( D < LINEWIDTH )
//            //{
//            //    //coverage = std::pow( 1.0 - ( D / LINEWIDTH ), 1.5 );
//
//            //    // 1./(1+e.^(-best_c*(x-actual_radius)));
//            //    D = R - (D - ( static_cast<double>( LINEWIDTH ) / 2.0 ));
//            //    double d_coverage = D / ( 2.0 * R ) * 2.0;
//            //    coverage = 1.0 / ( 1 + pow( e, -c * ( d_coverage - R ) ) );
//            //}
//
//            
//            //D = D - ( static_cast<double>( LINEWIDTH ) / 2 );
//
//            //if( D > R )
//            //{
//            //    continue;
//            //}
//            //if( D < 0 )
//            //{
//            //    D = abs( D );
//            //    double D_top = D;
//            //    double D_bot = LINEWIDTH - D;
//            //    coverage = (double)( 1 - ( ( acos( D_top / R ) - ( ( D_top * sqrt( RSQUARED - D_top * D_top ) ) / RSQUARED ) ) * ( 1 / PI ) ) );
//            //    if( D_bot < R )
//            //    {
//            //        coverage += (double)( 1 - ( ( acos( D_bot / R ) - ( ( D_bot * sqrt( RSQUARED - D_bot * D_bot ) ) / RSQUARED ) ) * ( 1 / PI ) ) );
//            //        coverage -= 1;
//            //    }
//            //}
//            //else
//            //{
//            //    coverage = (double)( ( acos( D / R ) - ( ( D * sqrt( RSQUARED - D * D ) ) / RSQUARED ) ) * ( 1 / PI ) );
//            //}
//            
//
//            oldColor = pixelFunction( p0, x, y + i, window );
//            newColor = getPixelColorAntialiased( oldColor, color, coverage );
//            lineFunction( p0, x, y + i, newColor, window );
//        }
//
//        //double x_distance_fraction = static_cast<double>( x ) / x_range;
//        //y =  v.y_ * x_distance_fraction;
//        y += slope;
//    }
//}
