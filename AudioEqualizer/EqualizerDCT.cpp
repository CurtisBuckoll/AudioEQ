#include "stdafx.h"
#include "EqualizerDCT.h"

#include <cmath>

static const double kMAGNITUDE_THRESH = 0.01;

//========================================================================
//
EqualizerDCT::EqualizerDCT( size_t chunk_size )
    : _chunk_size( chunk_size )
    , _dct( chunk_size )
    , _current_coeffs( _chunk_size, 0.0 )
{

}

//========================================================================
//
EqualizerDCT::~EqualizerDCT()
{

}

//========================================================================
//
void EqualizerDCT::eq( const std::vector<double>& in,
                       std::vector<double>& out,
                       const std::vector<double>& eq_coeffs,
                       bool save_coefficients )
{
    out = _dct.transform( in );

    // Process here
    for( Uint i = 0; i < out.size(); ++i )
    {
        out[i] = out[i] * ( 1.0 / 20.0 * std::pow( 10, eq_coeffs[i] / 100.0 ) );
    }

    // Accumulate spectrum coefficients
    if( save_coefficients )
    {
        for( size_t i = 0; i < out.size(); ++i )
        {
            _current_coeffs[i] += out[i];
        }
    }

    out = _dct.inverse_transform( out );
}

//========================================================================
//
void EqualizerDCT::getCurrentSpectrum( std::vector<double>& coeffs )
{
    coeffs.clear();
    coeffs.reserve( _chunk_size );

    for( size_t i = 0; i < _chunk_size; ++i )
    {
        double magnitude = _current_coeffs[i];
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
void EqualizerDCT::resetSpectrum()
{
    _current_coeffs.clear();
    _current_coeffs = std::vector<double>( _chunk_size, 0.0 );
}

//========================================================================
//
void EqualizerDCT::normalizeSpectrum( size_t total_iterations )
{
    const double scale_factor = 1.0 / static_cast<double>( total_iterations );

    for( double& entry : _current_coeffs )
    {
        entry = entry * scale_factor;
    }
}
