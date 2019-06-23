#pragma once

#include <vector>
#include "DCT.h"

class Equalizer
{

public:

    Equalizer( size_t chunk_size );

    ~Equalizer();

    void eq( const std::vector<double>& in,
             std::vector<double>& out,
             bool save_coefficients );

    void eq_chunks( const std::vector<std::vector<double>>& chunks_in,
                    std::vector<std::vector<double>>& chunks_out,
                    bool save_coefficients );

    void get_current_coeffecients( std::vector<double>& coeffs );

private:

    Equalizer() = delete;

    size_t chunk_size_;
    DCT dct_;
    
    std::vector<double> _current_coeffs;
};

