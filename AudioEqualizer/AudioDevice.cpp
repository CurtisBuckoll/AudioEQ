#include "stdafx.h"
#include "AudioDevice.h"

#include <vector>
#include <iostream>

#include "IEqualizer.h"

// Inline the call back function as a lambda.
auto read_func_callback = []( void* user_data, Uint8* device_buffer, int length ) -> void
{
    AudioBuffer* audio_buffer = static_cast<AudioBuffer*>( user_data );

    audio_buffer->_output_buffer.getSamples( device_buffer, length );
};

// =======================================================================
//
AudioDevice::AudioDevice( std::vector<std::vector<double>>&& data, 
                          size_t chunk_size, 
                          IEqualizer& eq,
                          const std::vector<double>& eq_coeffs )
    : _audio_thread( nullptr )
    , _audio_buffer( std::move( data ), chunk_size, eq, eq_coeffs )
    , _should_switch( false )
{
    // Capture the input data:
    // Set up our audio options.
    _audio_config = {};
    _audio_config._sampling_freq    = 44100;
    _audio_config._bit_depth_stereo = 2 * sizeof( Sint16 );
    _audio_config._volume_factor    = 1000;

    _audio_buffer._audio_config = &_audio_config;


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
    size_t num_iterations                  = 1;

    IEqualizer& equalizer = audio_buffer->_eq_manager.getCurrent();

    while( output_buffer.isEmpty() )
    {
        if( !input_buffer.outOfData() )
        {      
            std::vector<double> processed;
            equalizer.eq( std::move( input_buffer.getNextChunk() ), processed, audio_buffer->_eq_coeffs, true );
            // TODO: Add a set or add flag for above call regarding the eq coefficients.

            output_buffer.moveToBuffer( std::move( processed ) );
        }
        else
        {
            output_buffer.moveToBuffer( std::move( std::vector<double>( output_buffer.getChunkSize(), 0.0 ) ) );
        }

        ++num_iterations;
    }

    equalizer.normalizeSpectrum( num_iterations );
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

    _thread_context = {};
    _thread_context._audio_buffer    = &_audio_buffer;
    _thread_context._thread_is_alive = true;

    _audio_thread = SDL_CreateThread( getUserAudioDataThread, "Audio", (void*)&_thread_context );
}

// =======================================================================
//
void AudioDevice::getFrequencySpectrum( std::vector<double>& freq_coeffs )
{
    SDL_LockAudioDevice( _audio_buffer._device_id );
    _audio_buffer._eq_manager.getCurrent().getCurrentSpectrum( freq_coeffs );
    SDL_UnlockAudioDevice( _audio_buffer._device_id );
}

// =======================================================================
//
void AudioDevice::switchAnyalyzer( bool* keys )
{
    if( keys[SDLK_SPACE] )
    {
        if( _should_switch )
        {
            SDL_LockAudioDevice( _audio_buffer._device_id );
            _audio_buffer._eq_manager.update();
            SDL_UnlockAudioDevice( _audio_buffer._device_id );
            _should_switch = false;
        }
    }
    else
    {
        // Then we know the key has been released, so we can
        // know next switch is intentional.
        _should_switch = true;
    }
}

// =======================================================================
//
void AudioDevice::terminate()
{
    _thread_context._thread_is_alive = false;
    SDL_WaitThread( _audio_thread, nullptr );
    SDL_CloseAudioDevice( _audio_buffer._device_id );
}
