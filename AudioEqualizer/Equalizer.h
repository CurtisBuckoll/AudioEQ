#pragma once

#include <vector>
#include "DCT.h"

class Equalizer
{

public:

    Equalizer( size_t chunk_size );

    ~Equalizer();

    void eq( const std::vector<double>& in, std::vector<double>& out );

private:

    Equalizer() = delete;

    size_t chunk_size_;
    DCT dct_;
};

