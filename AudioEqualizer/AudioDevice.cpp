#include "stdafx.h"
#include "AudioDevice.h"

#include <vector>
#include <iostream>

#include "DCT.h"

// For testing, delete this pls. ****************
void AudioDevice::temp_apply_eq( WavFile& wav_file )
{
    std::vector<std::vector<double>> to_process;

    double* raw_data = wav_file.get_raw_data_pointer();
    size_t index = 0;

    const Uint DCT_SZ = 128;

    DCT dct( DCT_SZ );

    while( true )
    {
        if( index > wav_file.getNumSamples() - DCT_SZ )
        {
            break;
        }

        std::vector<double> next_chunk( DCT_SZ, 0.0 );

        for( size_t i = 0; i < DCT_SZ; ++i )
        {
            next_chunk[i] = raw_data[index];
            ++index;
        }

        index -= DCT_SZ;

        std::vector<double> transformed = dct.transform( next_chunk );

        const Uint MIDDLE = DCT_SZ / 2;

 /*       size_t shift = 20;
        double s = 4;

        transformed[MIDDLE + 3 - shift] = transformed[MIDDLE + 3 - shift] * 2 * s;
        transformed[MIDDLE + 2 - shift] = transformed[MIDDLE + 2 - shift] * 3 * s;
        transformed[MIDDLE + 1 - shift] = transformed[MIDDLE + 1 - shift] * 4 * s;
        transformed[MIDDLE + 0 - shift] = transformed[MIDDLE + 0 - shift] * 5 * s;
        transformed[MIDDLE - 1 - shift] = transformed[MIDDLE - 1 - shift] * 4 * s;
        transformed[MIDDLE - 2 - shift] = transformed[MIDDLE - 2 - shift] * 3 * s;
        transformed[MIDDLE - 3 - shift] = transformed[MIDDLE - 3 - shift] * 2 * s;*/


        transformed = dct.inverse_transform( transformed );

        for( size_t i = 0; i < DCT_SZ; ++i )
        {
            raw_data[index] = transformed[i];
            ++index;
        }
    }
}

// Inline the call back function as a lambda.
auto read_func_callback = []( void* user_data, Uint8* device_buffer, int length ) -> void
{
    //AudioBuffer* audio_buffer = (AudioBuffer*)user_data;

    //// Maybe optimize out mod operation later.

    //for( size_t i = 0; i < length; ++i )
    //{
    //    size_t read_pos = ( audio_buffer->_read_pos + i ) % audio_buffer->_size;
    //    device_buffer[i] = audio_buffer->_buffer[read_pos];
    //}

    //audio_buffer->_read_pos = ( audio_buffer->_read_pos + length ) % audio_buffer->_size;


    AudioBuffer* audio_buffer = static_cast<AudioBuffer*>( user_data );

    audio_buffer->_output_buffer.getSamples( device_buffer, length );
};

// =======================================================================
//
AudioDevice::AudioDevice( std::vector<std::vector<double>>&& data, size_t chunk_size )
    : _audio_thread( nullptr )
    , _audio_buffer( std::move( data ), chunk_size )
{
    // Capture the input data

    // Set up our audio options.
    _audio_config = {};
    _audio_config._sampling_freq    = 44100;
    _audio_config._bit_depth_stereo = 2 * sizeof( Sint16 );
    _audio_config._volume_factor    = 1000;

    // Set up the buffer. Set _write_pos to next so that the buffer 
    // initally has some data.
    //_audio_buffer = {};
    //_audio_buffer._size         = _audio_config._sampling_freq * _audio_config._bit_depth_stereo;
    //_audio_buffer._read_pos     = 0;
    //_audio_buffer._write_pos    = _audio_config._bit_depth_stereo;
    _audio_buffer._audio_config = &_audio_config;

    //_audio_buffer._buffer = new Uint8[_audio_buffer._size];
    //std::memset( static_cast<void*>(_audio_buffer._buffer), 0, _audio_buffer._size );

    // Initialize on SDL end. Will have to customize later probably.
    SDL_AudioSpec sdl_settings = {};
    sdl_settings.freq     = _audio_config._sampling_freq;
    sdl_settings.format   = AUDIO_S16LSB;
    sdl_settings.channels = 2;
    sdl_settings.samples  = 4096;
    sdl_settings.userdata = &_audio_buffer;

    sdl_settings.callback = read_func_callback;

    // Give these settings to SDL and check if okay.
    SDL_AudioSpec obtained_settings = {};
    _audio_buffer._device_id = SDL_OpenAudioDevice( nullptr, 0, &sdl_settings, &obtained_settings, 0 );

    if( sdl_settings.format != obtained_settings.format )
    {
        std::cout << "AudioDevice::Constructor : Unable to obtain desired SDL audio settings" << std::endl;
        exit( -1 );
    }
}

// =======================================================================
//
AudioDevice::~AudioDevice()
{

}

// =======================================================================
//
void processAudioToOutputBuffer( AudioBuffer* audio_buffer )
{
    AudioBuffer::InputBuffer& input_buffer = audio_buffer->_input_buffer;
    AudioOutputBuffer& output_buffer       = audio_buffer->_output_buffer;

    bool save_coefficents = true;
    while( output_buffer.isEmpty() )
    {
        if( !input_buffer.outOfData() )
        {      
            std::vector<double> processed;
            audio_buffer->_equalizer.eq( std::move( input_buffer.getNextChunk() ), processed, save_coefficents );
            output_buffer.moveToBuffer( std::move( processed ) );
            save_coefficents = false;
        }
        else
        {
            output_buffer.moveToBuffer( std::move( std::vector<double>( output_buffer.getChunkSize(), 0.0 ) ) );
        }
    }

    return;

    //size_t bytes_written = 0;

    //Sint16* write_pos = reinterpret_cast<Sint16*>( &audio_buffer->_buffer[audio_buffer->_write_pos] );
    //Sint16* final_pos = reinterpret_cast<Sint16*>( &audio_buffer->_buffer[audio_buffer->_size - 4] );

    //// Handle a couple cases: first if write position is beyond read
    //// in the array, then fill to the end.
    //if( audio_buffer->_write_pos > audio_buffer->_read_pos )
    //{
    //    while( write_pos <= final_pos )
    //    {
    //        Sint16 sample_val = get_sample( audio_buffer ); //get_sample( audio_buffer->_audio_config )
    //        *write_pos++ = sample_val;
    //        *write_pos++ = sample_val;
    //        bytes_written += 4;
    //    }

    //    // Now write from the front of the buffer up to _read_pos.
    //    write_pos = reinterpret_cast<Sint16*>( &audio_buffer->_buffer[0] );
    //}

    //final_pos = reinterpret_cast<Sint16*>( &audio_buffer->_buffer[audio_buffer->_read_pos - 4] );

    //while( write_pos <= final_pos )
    //{
    //    Sint16 sample_val = get_sample( audio_buffer ); //get_sample( audio_buffer->_audio_config )
    //    *write_pos++ = sample_val;
    //    *write_pos++ = sample_val;
    //    bytes_written += 4;
    //}

    //audio_buffer->_write_pos = ( audio_buffer->_write_pos + bytes_written ) % audio_buffer->_size;
}

// =======================================================================
//
Sint16 sampleSineWave( AudioConfig* audio_config )
{
    static int sample_index = 0;
    double half_wave_counter = 90;
    return 3000 * sin( 2 * 3.14159265359 * sample_index++ / half_wave_counter );
}

// =======================================================================
//
int getUserAudioDataThread( void* user_data )
{
    ThreadContext* audio_thread = static_cast<ThreadContext*>( user_data );

    while( audio_thread->_thread_is_alive )
    {
        SDL_LockAudioDevice( audio_thread->_audio_buffer->_device_id );
        processAudioToOutputBuffer( audio_thread->_audio_buffer );
        SDL_UnlockAudioDevice( audio_thread->_audio_buffer->_device_id );
    }

    return 0;
}

// =======================================================================
//
void AudioDevice::setPlayState( DEVICE_STATE state )
{
    SDL_PauseAudioDevice( _audio_buffer._device_id, 0 );

    // Figure out a better organization of this stuff later.
    _thread_context = {};
    _thread_context._audio_buffer    = &_audio_buffer;
    _thread_context._thread_is_alive = true;

    _audio_thread = SDL_CreateThread( getUserAudioDataThread, "Audio", (void*)&_thread_context );
}

// -----------------------------------------------------------------
//
void AudioDevice::getFrequencySpectrum( std::vector<double>& freq_coeffs )
{
    SDL_LockAudioDevice( _audio_buffer._device_id );
    _audio_buffer._equalizer.get_current_coeffecients( freq_coeffs );
    SDL_UnlockAudioDevice( _audio_buffer._device_id );
}

// =======================================================================
//
void AudioDevice::terminate()
{
    _thread_context._thread_is_alive = false;

    SDL_WaitThread( _audio_thread, nullptr );
    SDL_CloseAudioDevice( _audio_buffer._device_id );

    //if( _audio_buffer._buffer )
    //{
    //    delete[] _audio_buffer._buffer;
    //}
}
