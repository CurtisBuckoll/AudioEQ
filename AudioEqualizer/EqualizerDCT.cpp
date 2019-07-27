#include "stdafx.h"
#include "EqualizerDCT.h"

#include <cmath>

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
    coeffs = _current_coeffs;
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
        //entry /= static_cast<double>( total_iterations );
        entry = entry * scale_factor;
    }
}
