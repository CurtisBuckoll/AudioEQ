#include "stdafx.h"
#include "Equalizer.h"


Equalizer::Equalizer( size_t chunk_size )
    : chunk_size_( chunk_size )
    , dct_( chunk_size )
{
    
}

Equalizer::~Equalizer()
{

}

void Equalizer::eq( const std::vector<double>& in,
                    std::vector<double>& out,
                    bool save_coefficients )
{
    out = dct_.transform( in );

    // Process here
    if( save_coefficients )
    {
        _current_coeffs = out;
    }

    out = dct_.inverse_transform( out );
}

void Equalizer::eq_chunks( const std::vector<std::vector<double>>& chunks_in,
                           std::vector<std::vector<double>>& chunks_out,
                           bool save_coefficients )
{
    for( auto chunk : chunks_in )
    {
        std::vector<double> transformed = std::move( dct_.transform( chunk ) );

        // Process here
        if( save_coefficients )
        {
            _current_coeffs = transformed;
        }
        

        chunks_out.push_back( std::move( dct_.inverse_transform( transformed ) ) );
    }
}

void Equalizer::get_current_coeffecients( std::vector<double>& coeffs )
{
    coeffs = _current_coeffs;
}
