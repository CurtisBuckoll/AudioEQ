#pragma once

#include "IEqualizer.h"
#include "DFT.h"

// =======================================================================
// 
class EqualizerFFT : public IEqualizer
{
public:

    //--------------------------------------------------------------
    //
    EqualizerFFT( size_t chunk_size );

    //--------------------------------------------------------------
    //
    ~EqualizerFFT();

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
    void normalizeSpectrum( const size_t total_iterations ) override;

private:

    //--------------------------------------------------------------
    //
    std::vector<Math::Complex> fft( std::vector<Math::Complex> signal );
    
    //--------------------------------------------------------------
    //
    std::vector<Math::Complex> ifft( std::vector<Math::Complex> signal );

    //--------------------------------------------------------------
    //
    EqualizerFFT() = delete;

    //--------------------------------------------------------------
    //
    size_t                     _chunk_size;
    DFT                        _dft;
    std::vector<Math::Complex> _current_coeffs;
};
