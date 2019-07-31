#include "stdafx.h"
#include "EqualizerFFT.h"

#include <cmath>

// We need this so we don't take log of a number too close to 0.
static const double kMAGNITUDE_THRESH = 0.01;

//========================================================================
//
EqualizerFFT::EqualizerFFT( size_t chunk_size )
    : _chunk_size( chunk_size )
    , _dft( chunk_size )
    , _current_coeffs( _chunk_size, Math::Complex( 0.0, 0.0 ) )
{

}

//========================================================================
//
EqualizerFFT::~EqualizerFFT()
{

}

//========================================================================
// Always expect size of input vector to be even, in particular, a power 
// of two.
std::vector<Math::Complex> EqualizerFFT::fft( std::vector<Math::Complex> signal )
{
    // Base case:
    if( signal.size() == 1 )
    {
        return std::move( signal );
    }
    //if( signal.size() <= 2 )
    //{
    //    std::vector<Math::Complex> base_result( 2, Math::Complex(0.0, 0.0) );
    //    base_result[0] = signal[0] + signal[1];
    //    base_result[1] = signal[0] - signal[1];
    //    return base_result;
    //}

    // General case:
    // Form two vectors containing even/odd indices resp.
    std::vector<Math::Complex> even( signal.size() / 2, Math::Complex( 0.0, 0.0 ) );
    std::vector<Math::Complex> odd( signal.size() / 2, Math::Complex( 0.0, 0.0 ) );

    for( size_t i = 0; i < signal.size() / 2; ++i )
    {
        even[i] = signal[i * 2];
        odd[i]  = signal[i * 2 + 1];
    }

    // Recurse for both odd/even signals.
    std::vector<Math::Complex> even_transformed = fft( std::move(even) );
    std::vector<Math::Complex> odd_transformed  = fft( std::move(odd) );

    // Now join the results using the butterfly technique.
    std::vector<Math::Complex> result( signal.size(), Math::Complex( 0.0, 0.0 ) );

    Math::Complex W   = Math::Complex( 1.0, 0.0 );
    Math::Complex W_1 = Math::Complex( std::cos( ( 2.0 * PI ) / static_cast<double>( signal.size() ) ),
                                      -std::sin( ( 2.0 * PI ) / static_cast<double>( signal.size() ) ) );
    
    const size_t half_signal_size = signal.size() / 2;
    for( size_t i = 0; i < half_signal_size; ++i )
    {
        result[i]                    = even_transformed[i] + ( W * odd_transformed[i] );
        result[i + half_signal_size] = even_transformed[i] - ( W * odd_transformed[i] );
        W = W * W_1;
    }

    // Return result.
    return std::move( result );
}

//========================================================================
// Always expect size of input vector to be even, in particular, a power 
// of two.
std::vector<Math::Complex> EqualizerFFT::ifft( std::vector<Math::Complex> signal )
{
    // Base case:
    if( signal.size() == 1 )
    {
        return std::move( signal );
    }
    //if( signal.size() <= 2 )
    //{
    //    std::vector<Math::Complex> base_result( 2, Math::Complex(0.0, 0.0) );
    //    base_result[0] = signal[0] + signal[1];
    //    base_result[1] = signal[0] - signal[1];
    //    return base_result;
    //}

    // General case:
    // Form two vectors containing even/odd indices resp.
    std::vector<Math::Complex> even( signal.size() / 2, Math::Complex( 0.0, 0.0 ) );
    std::vector<Math::Complex> odd( signal.size() / 2, Math::Complex( 0.0, 0.0 ) );

    for( size_t i = 0; i < signal.size() / 2; ++i )
    {
        even[i] = signal[i * 2];
        odd[i]  = signal[i * 2 + 1];
    }

    // Recurse for both odd/even signals.
    std::vector<Math::Complex> even_transformed = ifft( std::move(even) );
    std::vector<Math::Complex> odd_transformed  = ifft( std::move(odd) );

    // Now join the results using the butterfly technique.
    std::vector<Math::Complex> result( signal.size(), Math::Complex( 0.0, 0.0 ) );

    Math::Complex W   = Math::Complex( 1.0, 0.0 );
    Math::Complex W_1 = Math::Complex( std::cos( ( 2.0 * PI ) / static_cast<double>( signal.size() ) ),
                                       std::sin( ( 2.0 * PI ) / static_cast<double>( signal.size() ) ) );
    
    const size_t half_signal_size = signal.size() / 2;
    for( size_t i = 0; i < half_signal_size; ++i )
    {
        result[i]                    = even_transformed[i] + ( W * odd_transformed[i] );
        result[i + half_signal_size] = even_transformed[i] - ( W * odd_transformed[i] );
        W = W * W_1;
    }

    // Return result.
    return std::move( result );
}


//========================================================================
//
void EqualizerFFT::eq( const std::vector<double>& in,
                       std::vector<double>& out,
                       const std::vector<double>& eq_coeffs,
                       bool save_coefficients )
{
    // Extract real part of signal.
    std::vector<Math::Complex> signal_as_complex( _chunk_size, Math::Complex( 0.0, 0.0 ) );

    for( size_t i = 0; i < _chunk_size; ++i )
    {
        signal_as_complex[i].re( in[i] );
    }

    // Forward transform.
    //std::vector<Math::Complex> transformed = _dft.transform( std::move( signal_as_complex ) );
    std::vector<Math::Complex> transformed = std::move( fft( std::move( signal_as_complex ) ) );

    // Normalize the result.
    for( auto& c : transformed )
    {
        c = c * ( 1.0 / static_cast<double>( std::sqrt( transformed.size() ) ) );
    }

    // Process here //
    for( Uint i = 0; i < transformed.size(); ++i )
    {
        transformed[i] = transformed[i] * ( 1.0 / 20.0 * std::pow( 10, eq_coeffs[i] / 100.0 ) );
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
    //std::vector<Math::Complex> inverse_signal_as_complex = _dft.inverse_transform( std::move( transformed ) );
    std::vector<Math::Complex> inverse_signal_as_complex = std::move( ifft( std::move( transformed ) ) );

    // Normalize the result.
    for( auto& c : inverse_signal_as_complex )
    {
        c = c * ( 1.0 / static_cast<double>( std::sqrt( inverse_signal_as_complex.size() ) ) );
    }

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
//void EqualizerDFT::eqChunks( const std::vector<std::vector<double>>& chunks_in,
//                             std::vector<std::vector<double>>& chunks_out,
//                             bool save_coefficients )
//{
//    for( auto chunk : chunks_in )
//    {
//        std::vector<double> transformed = std::move( _dct.transform( chunk ) );
//
//        // Process here
//        if( save_coefficients )
//        {
//            _current_coeffs = transformed;
//        }
//
//
//        chunks_out.push_back( std::move( _dct.inverse_transform( transformed ) ) );
//    }
//}

//========================================================================
//
void EqualizerFFT::getCurrentSpectrum( std::vector<double>& coeffs )
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
void EqualizerFFT::resetSpectrum()
{
    _current_coeffs.clear();
    _current_coeffs = std::vector<Math::Complex>( _chunk_size, Math::Complex( 0.0, 0.0 ) );
}

//========================================================================
//
void EqualizerFFT::normalizeSpectrum( const size_t total_iterations )
{
    const double scale_factor = 1.0 / static_cast<double>( total_iterations );

    for( Math::Complex& entry : _current_coeffs )
    {
        entry = entry * scale_factor;
    }
}
