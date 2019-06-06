#pragma once

#include <vector>
#include "DCT.h"

class Equalizer
{

public:

    Equalizer( size_t chunk_size );

    ~Equalizer();

    void eq( const std::vector<double>& in, std::vector<double>& out );

    void eq_chunks( const std::vector<std::vector<double>>& chunks_in, 
                    std::vector<std::vector<double>>& chunks_out );

private:

    Equalizer() = delete;

    size_t chunk_size_;
    DCT dct_;
};

