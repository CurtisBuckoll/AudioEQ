#pragma once

#include <SDL/SDL.h>
#include <cstring>
#include "WavFile.h"

// =======================================================================
// *********************************   We will optimize this little nugget   *********************************
class InputBuffer {
public:
    std::vector<double> samples;
    size_t front_index;

    InputBuffer()
        : front_index( 0 )
        , samples( 44100, 0.0 )
    {}

    void addToBuffer( std::vector<std::vector<double>> data )
    {
        for( std::vector<double> sub_arr : data )
        {
            for( double sample : sub_arr )
            {
                samples.push_back( sample );
            }
        }
    }

    Sint16 getSample()
    {
        if( front_index < samples.size() )
        {
            return static_cast<Sint16>( samples[front_index++] );
        }
        return 0;
    }
};

// =======================================================================
//
struct AudioConfig
{
    int _sample_index;
    int _sampling_freq;
    int _bit_depth_stereo;
    int _volume_factor;
    //WavFile _wav_file;
};

// =======================================================================
//
struct AudioBuffer
{
    Uint8* _buffer;
    size_t _size;
    int _read_pos;
    int _write_pos;
    SDL_AudioDeviceID _device_id;
    AudioConfig* _audio_config;

    InputBuffer _input_buffer;
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
    //
    AudioDevice();

    // -----------------------------------------------------------------
    //
    ~AudioDevice();

    // -----------------------------------------------------------------
    //
    void setPlayState( DEVICE_STATE state);

    // -----------------------------------------------------------------
    // Takes ownership of the buffer supplied.
    void addToBuffer( std::vector<std::vector<double>>& data );

    // -----------------------------------------------------------------
    //
    Sint16 getSampleFromInternalBuffer();

    // -----------------------------------------------------------------
    //
    void terminate();

    // TEMP***************
    void temp_apply_eq( WavFile& wav_file );

private:

    AudioConfig _audio_config;
    AudioBuffer _audio_buffer;
    ThreadContext _thread_context;

    SDL_Thread* _audio_thread;

    //InputBuffer _input_buffer;
};

