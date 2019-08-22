#pragma once

#include <SDL/SDL.h>
#include <cstring>
#include "WavFile.h"

#include "AudioOutputBuffer.h"
#include "IEqualizer.h"


// =======================================================================
//
struct AudioConfig
{
    // -----------------------------------------------------------------
    //
    int _sample_index;
    int _sampling_freq;
    int _bit_depth_stereo;
    int _volume_factor;
};

// =======================================================================
//
struct AudioBuffer
{
    // -----------------------------------------------------------------
    //
    AudioBuffer( std::vector<std::vector<double>>&& data,
                 size_t chunk_size,
                 IEqualizer& eq,
                 const std::vector<double>& eq_coeffs)
        : _output_buffer( chunk_size )
        , _input_buffer( std::move( data ) )
        , _equalizer( eq )
        , _eq_coeffs( eq_coeffs )
    {};

    // -----------------------------------------------------------------
    //
    struct InputBuffer 
    {
        std::vector<std::vector<double>> _buffer;
        size_t _chunk_index;
        size_t _chunk_sample_index;

        InputBuffer( std::vector<std::vector<double>>&& input_data )
            : _chunk_index( 0 )
            , _chunk_sample_index( 0 )
            , _buffer( std::move( input_data ) )
        {}

        std::vector<double> getNextChunk()
        {
            return std::move( _buffer[_chunk_index++] );
        }

        bool outOfData()
        {
            return _chunk_index >= _buffer.size();
        }

        InputBuffer() = delete;
    };

    // -----------------------------------------------------------------
    //
    AudioBuffer() = delete;

    // -----------------------------------------------------------------
    //
    SDL_AudioDeviceID _device_id;
    AudioConfig*      _audio_config;
    InputBuffer       _input_buffer;
    AudioOutputBuffer _output_buffer;
    IEqualizer&       _equalizer;
    const std::vector<double>& _eq_coeffs;
};

// =======================================================================
//
struct ThreadContext
{
    AudioBuffer* _audio_buffer;
    bool _thread_is_alive;
};

// =======================================================================
//
enum class DEVICE_STATE
{
    PLAY,
    PAUSE
};


// =======================================================================
//
class AudioDevice
{
public:

    // -----------------------------------------------------------------
    // Takes ownership of the buffer supplied.
    AudioDevice( std::vector<std::vector<double>>&& data,
                 size_t chunk_size,
                 IEqualizer& eq,
                 const std::vector<double>& eq_coeffs );

    // -----------------------------------------------------------------
    //
    ~AudioDevice();

    // -----------------------------------------------------------------
    //
    void setPlayState( DEVICE_STATE state);

    // -----------------------------------------------------------------
    //
    void getFrequencySpectrum( std::vector<double>& freq_coeffs );

    // -----------------------------------------------------------------
    //
    void terminate();


private:

    AudioConfig   _audio_config;
    AudioBuffer   _audio_buffer;
    ThreadContext _thread_context;
    SDL_Thread*   _audio_thread;

    //InputBuffer _input_buffer;
};

