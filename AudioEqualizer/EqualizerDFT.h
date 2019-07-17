#pragma once

#include "IEqualizer.h"
#include "DFT.h"

// =======================================================================
// 
class EqualizerDFT : public IEqualizer
{
public:

    //--------------------------------------------------------------
    //
    EqualizerDFT( size_t chunk_size );

    //--------------------------------------------------------------
    //
    ~EqualizerDFT();

    //--------------------------------------------------------------
    //
    void eq( const std::vector<double>& in,
             std::vector<double>& out,
             bool save_coefficients ) override;

    //--------------------------------------------------------------
    //
    //void eqChunks( const std::vector<std::vector<double>>& chunks_in,
    //               std::vector<std::vector<double>>& chunks_out,
    //               bool save_coefficients );

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
    EqualizerDFT() = delete;

    //--------------------------------------------------------------
    //
    size_t                     _chunk_size;
    DFT                        _dft;
    std::vector<Math::Complex> _current_coeffs;
};
