#pragma once

#include <vector>

// =======================================================================
// 
class IEqualizer
{
public:

    //--------------------------------------------------------------
    //
    virtual ~IEqualizer() {};

    //--------------------------------------------------------------
    //
    virtual void eq( const std::vector<double>& in,
                     std::vector<double>& out,
                     bool save_coefficients ) = 0;

    //--------------------------------------------------------------
    //
    virtual void getCurrentSpectrum( std::vector<double>& coeffs ) = 0;

    //--------------------------------------------------------------
    //
    virtual void resetSpectrum() = 0;

    //--------------------------------------------------------------
    //
    virtual void normalizeSpectrum( size_t total_iterations ) = 0;


protected:

    //--------------------------------------------------------------
    //
    IEqualizer() {};
};
