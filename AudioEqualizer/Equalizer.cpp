#include "stdafx.h"
#include "Equalizer.h"


//========================================================================
//
Equalizer::Equalizer( size_t chunk_size )
    : _chunk_size( chunk_size )
    , _dct( chunk_size )
    , _current_coeffs( _chunk_size, 0.0 )
{
    
}

//========================================================================
//
Equalizer::~Equalizer()
{

}

//========================================================================
//
void Equalizer::eq( const std::vector<double>& in,
                    std::vector<double>& out,
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
void Equalizer::eqChunks( const std::vector<std::vector<double>>& chunks_in,
                           std::vector<std::vector<double>>& chunks_out,
                           bool save_coefficients )
{
    for( auto chunk : chunks_in )
    {
        std::vector<double> transformed = std::move( _dct.transform( chunk ) );

        // Process here
        if( save_coefficients )
        {
            _current_coeffs = transformed;
        }
        

        chunks_out.push_back( std::move( _dct.inverse_transform( transformed ) ) );
    }
}

//========================================================================
//
void Equalizer::getCurrentSpectrum( std::vector<double>& coeffs )
{
    coeffs = _current_coeffs;
}

//========================================================================
//
void Equalizer::resetSpectrum()
{
    _current_coeffs.clear();
    _current_coeffs = std::vector<double>( _chunk_size, 0.0 );
}

//========================================================================
//
void Equalizer::normalizeSpectrum( size_t total_iterations )
{
    for( double& entry : _current_coeffs )
    {
        entry /= static_cast<double>( total_iterations );
    }
}
