#pragma once

#include <vector>
#include "DCT.h"

// =======================================================================
// 
class Equalizer
{
public:

    //--------------------------------------------------------------
    //
    Equalizer( size_t chunk_size );

    //--------------------------------------------------------------
    //
    ~Equalizer();

    //--------------------------------------------------------------
    //
    void eq( const std::vector<double>& in,
             std::vector<double>& out,
             bool save_coefficients );

    //--------------------------------------------------------------
    //
    void eqChunks( const std::vector<std::vector<double>>& chunks_in,
                    std::vector<std::vector<double>>& chunks_out,
                    bool save_coefficients );

    //--------------------------------------------------------------
    //
    void getCurrentSpectrum( std::vector<double>& coeffs );

    //--------------------------------------------------------------
    //
    void resetSpectrum();

    //--------------------------------------------------------------
    //
    void normalizeSpectrum( size_t total_iterations );

private:

    //--------------------------------------------------------------
    //
    Equalizer() = delete;

    //--------------------------------------------------------------
    //
    size_t              _chunk_size;
    DCT                 _dct;
    std::vector<double> _current_coeffs;
};
