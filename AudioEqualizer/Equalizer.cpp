#include "stdafx.h"
#include "Equalizer.h"


Equalizer::Equalizer( size_t chunk_size )
    : chunk_size_( chunk_size )
    , dct_( chunk_size )
{
    
}

void Equalizer::eq( const std::vector<double>& in, std::vector<double>& out )
{
    out = dct_.transform( in );

    out = dct_.inverse_transform( out );
}

Equalizer::~Equalizer()
{

}
