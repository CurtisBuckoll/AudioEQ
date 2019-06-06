#pragma once

#include <SDL/SDL.h>
#include <cstring>
#include "WavFile.h"

struct AudioConfig
{
    int _sample_index;
    int _sampling_freq;
    int _bit_depth_stereo;
    int _volume_factor;
    //WavFile _wav_file;
};

struct AudioBuffer
{
    Uint8* _buffer;
    size_t _size;
    int _read_pos;
    int _write_pos;
    SDL_AudioDeviceID _device_id;
    AudioConfig* _audio_config;
};

struct ThreadContext
{
    AudioBuffer* _audio_buffer;
    bool _thread_is_alive;
};

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
    //
    void terminate();

    // TEMP***************
    void temp_apply_eq( WavFile& wav_file );

private:

    AudioConfig _audio_config;
    AudioBuffer _audio_buffer;
    ThreadContext _thread_context;

    SDL_Thread* _audio_thread;

};

