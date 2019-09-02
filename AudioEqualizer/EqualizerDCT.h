#pragma once

#include "IEqualizer.h"
#include "DCT.h"

// =======================================================================
// 
class EqualizerDCT : public IEqualizer
{
public:

    //--------------------------------------------------------------
    //
    EqualizerDCT( size_t chunk_size );

    //--------------------------------------------------------------
    //
    virtual ~EqualizerDCT();

    //--------------------------------------------------------------
    //
    void eq( const std::vector<double>& in,
             std::vector<double>& out,
             const std::vector<double>& eq_coeffs,
             bool save_coefficients ) override;

    //--------------------------------------------------------------
    //
    void getCurrentSpectrum( std::vector<double>& coeffs ) override;

    //--------------------------------------------------------------
    //
    void resetSpectrum() override;

    //--------------------------------------------------------------
    //
    void normalizeSpectrum( size_t total_iterations ) override;

private:

    //--------------------------------------------------------------
    //
    EqualizerDCT() = delete;

    //--------------------------------------------------------------
    //
    size_t              _chunk_size;
    DCT                 _dct;
    std::vector<double> _current_coeffs;
};
