#include "stdafx.h"
#include "EqualizerDFT.h"

#include <cmath>


//--------------------------------------------------------------
// We need this so we don't take log of a number too close to 0.
static const double kMAGNITUDE_THRESH = 0.01;

//========================================================================
//
EqualizerDFT::EqualizerDFT( size_t chunk_size )
    : _chunk_size( chunk_size )
    , _dft( chunk_size )
    , _current_coeffs( _chunk_size, Math::Complex(0.0, 0.0) )
{

}

//========================================================================
//
EqualizerDFT::~EqualizerDFT()
{

}

//========================================================================
//
void EqualizerDFT::eq( const std::vector<double>& in,
                       std::vector<double>& out,
                       const std::vector<double>& eq_coeffs,
                       bool save_coefficients )
{
    // Extract real part of signal.
    std::vector<Math::Complex> signal_as_complex( _chunk_size, Math::Complex(0.0, 0.0) );

    for( size_t i = 0; i < _chunk_size; ++i )
    {
        signal_as_complex[i].re( in[i] );
    }

    // Forward transform.
    std::vector<Math::Complex> transformed = std::move( _dft.transform( signal_as_complex ) );

    // Process here
    for( Uint i = 0; i < transformed.size(); ++i )
    {
        transformed[i] = transformed[i] * (1.0 / 20.0 * std::pow( 10, eq_coeffs[i] / 100.0 ));
    }

    // Accumulate spectrum coefficients.
    if( save_coefficients )
    {
        for( size_t i = 0; i < _chunk_size; ++i )
        {
            _current_coeffs[i] += transformed[i];
        }
    }

    // Transform back.
    std::vector<Math::Complex> inverse_signal_as_complex = std::move( _dft.inverse_transform( transformed ) );

    out.clear();
    out.reserve( _chunk_size );

    // Output real part of inverse transform.
    for( size_t i = 0; i < _chunk_size; ++i )
    {
        out.push_back( inverse_signal_as_complex[i].re() );
    }

    return;
}

//========================================================================
//
void EqualizerDFT::getCurrentSpectrum( std::vector<double>& coeffs )
{
    coeffs.clear();
    coeffs.reserve( _chunk_size );

    for( size_t i = 0; i < _chunk_size; ++i )
    {
        double magnitude = _current_coeffs[i].magnitude();
        if( magnitude > kMAGNITUDE_THRESH )
        {
            coeffs.push_back( std::log2( magnitude ) * 800 );
        }
        else
        {
            coeffs.push_back( 0.0 );
        }
    }
}

//========================================================================
//
void EqualizerDFT::resetSpectrum()
{
    _current_coeffs.clear();
    _current_coeffs = std::vector<Math::Complex>( _chunk_size, Math::Complex( 0.0, 0.0 ) );
}

//========================================================================
//
void EqualizerDFT::normalizeSpectrum( const size_t total_iterations )
{
    const double scale_factor = 1.0 / static_cast<double>( total_iterations );

    for( Math::Complex& entry : _current_coeffs )
    {
        entry = entry * scale_factor;
    }
}
