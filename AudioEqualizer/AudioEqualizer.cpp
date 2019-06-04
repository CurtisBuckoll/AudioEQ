// AudioEqualizer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// =======================================================================

#include <SDL/SDL.h>
#include <cstring>

#include "WavFile.h"

typedef float Real32;

static Real32 PI = 3.14159265359;
static Real32 TAU = 2 * PI;
static Uint32 SDL_AUDIO_TRANSPARENTLY_CONVERT_FORMAT = 0;

//const char const* FILE_PATH = "../resources/Q2_sample_2.wav";

///////////////////////////////////////////////////////////////////////////////

struct platform_program_state
{
    bool IsRunning;
    SDL_Event LastEvent;
};

struct platform_audio_config
{
    int ToneHz;
    int ToneVolume;
    int WavePeriod;
    int _sample_index;
    int SamplesPerSecond;
    int BytesPerSample;
    WavFile wav_file;
};

struct platform_audio_buffer
{
    Uint8* Buffer;
    int Size;
    int ReadCursor;
    int WriteCursor;
    SDL_AudioDeviceID DeviceID;
    platform_audio_config* AudioConfig;
};

struct platform_audio_thread_context
{
    platform_audio_buffer* AudioBuffer;
    platform_program_state* ProgramState;
};

///////////////////////////////////////////////////////////////////////////////

Sint16
SampleSquareWave( platform_audio_config* AudioConfig )
{
    int HalfSquareWaveCounter = AudioConfig->WavePeriod / 2;
    if( ( AudioConfig->_sample_index / HalfSquareWaveCounter ) % 2 == 0 )
    {
        return AudioConfig->ToneVolume;
    }

    return -AudioConfig->ToneVolume;
}

///////////////////////////////////////////////////////////////////////////////

Sint16 SampleSineWave( platform_audio_config* AudioConfig )
{
    int HalfWaveCounter = AudioConfig->WavePeriod / 2;
    return AudioConfig->ToneVolume * sin( TAU * AudioConfig->_sample_index / HalfWaveCounter );
}

///////////////////////////////////////////////////////////////////////////////

Sint16 SampleFromAudioStream( platform_audio_config* AudioConfig )
{
    if( AudioConfig->wav_file.ifMoreDataAvailable() )
    {
        return static_cast<Sint16>( AudioConfig->wav_file.readCurrentInput() );
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

void  SampleIntoAudioBuffer( platform_audio_buffer* AudioBuffer, Sint16( *GetSample )( platform_audio_config* ) )
{
    int Region1Size = AudioBuffer->ReadCursor - AudioBuffer->WriteCursor;
    int Region2Size = 0;
    if( AudioBuffer->ReadCursor < AudioBuffer->WriteCursor )
    {
        // Fill to the end of the buffer and loop back around and fill to the read
        // cursor.
        Region1Size = AudioBuffer->Size - AudioBuffer->WriteCursor;
        Region2Size = AudioBuffer->ReadCursor;
    }

    platform_audio_config* AudioConfig = AudioBuffer->AudioConfig;

    int Region1Samples = Region1Size / AudioConfig->BytesPerSample;
    int Region2Samples = Region2Size / AudioConfig->BytesPerSample;
    int BytesWritten = Region1Size + Region2Size;

    Sint16* Buffer = (Sint16*)&AudioBuffer->Buffer[AudioBuffer->WriteCursor];
    for( int SampleIndex = 0;
         SampleIndex < Region1Samples;
         SampleIndex++ )
    {
        Sint16 SampleValue = ( *GetSample )( AudioConfig );
        *Buffer++ = SampleValue;
        *Buffer++ = SampleValue;
        AudioConfig->_sample_index++;
    }

    Buffer = (Sint16*)AudioBuffer->Buffer;
    for( int SampleIndex = 0;
         SampleIndex < Region2Samples;
         SampleIndex++ )
    {
        Sint16 SampleValue = ( *GetSample )( AudioConfig );
        *Buffer++ = SampleValue;
        *Buffer++ = SampleValue;
        AudioConfig->_sample_index++;
    }

    AudioBuffer->WriteCursor =
        ( AudioBuffer->WriteCursor + BytesWritten ) % AudioBuffer->Size;
}

///////////////////////////////////////////////////////////////////////////////

void PlatformFillAudioDeviceBuffer( void* UserData, Uint8* DeviceBuffer, int Length )
{
    platform_audio_buffer* AudioBuffer = (platform_audio_buffer*)UserData;

    // Keep track of two regions. Region1 contains everything from the current
    // PlayCursor up until, potentially, the end of the buffer. Region2 only
    // exists if we need to circle back around. It contains all the data from the
    // beginning of the buffer up until sufficient bytes are read to meet Length.
    int Region1Size = Length;
    int Region2Size = 0;
    if( AudioBuffer->ReadCursor + Length > AudioBuffer->Size )
    {
        // Handle looping back from the beginning.
        Region1Size = AudioBuffer->Size - AudioBuffer->ReadCursor;
        Region2Size = Length - Region1Size;
    }

    SDL_memcpy(
        DeviceBuffer,
        ( AudioBuffer->Buffer + AudioBuffer->ReadCursor ),
        Region1Size
    );
    SDL_memcpy(
        &DeviceBuffer[Region1Size],
        AudioBuffer->Buffer,
        Region2Size
    );

    AudioBuffer->ReadCursor =
        ( AudioBuffer->ReadCursor + Length ) % AudioBuffer->Size;
}

///////////////////////////////////////////////////////////////////////////////

void PlatformInitializeAudio( platform_audio_buffer* AudioBuffer )
{
    SDL_AudioSpec AudioSettings = {};
    AudioSettings.freq = AudioBuffer->AudioConfig->SamplesPerSecond;
    AudioSettings.format = AUDIO_S16LSB;
    AudioSettings.channels = 2;
    AudioSettings.samples = 4096;
    AudioSettings.callback = &PlatformFillAudioDeviceBuffer;
    AudioSettings.userdata = AudioBuffer;

    SDL_AudioSpec ObtainedSettings = {};
    AudioBuffer->DeviceID = SDL_OpenAudioDevice(
        NULL, 0, &AudioSettings, &ObtainedSettings,
        SDL_AUDIO_TRANSPARENTLY_CONVERT_FORMAT
    );

    if( AudioSettings.format != ObtainedSettings.format )
    {
        SDL_Log( "Unable to obtain expected audio settings: %s", SDL_GetError() );
        exit( 1 );
    }

    // Start playing the audio buffer
    SDL_PauseAudioDevice( AudioBuffer->DeviceID, 0 );
}

///////////////////////////////////////////////////////////////////////////////

void PlatformHandleEvent( platform_program_state* ProgramState )
{
    if( ProgramState->LastEvent.type == SDL_QUIT )
    {
        ProgramState->IsRunning = false;
    }
}

///////////////////////////////////////////////////////////////////////////////

int PlatformAudioThread( void* UserData )
{
    platform_audio_thread_context* AudioThread =
        (platform_audio_thread_context*)UserData;

    while( AudioThread->ProgramState->IsRunning )
    {
        SDL_LockAudioDevice( AudioThread->AudioBuffer->DeviceID );
        SampleIntoAudioBuffer( AudioThread->AudioBuffer, &SampleFromAudioStream );
        SDL_UnlockAudioDevice( AudioThread->AudioBuffer->DeviceID );
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

//int main( int argc, char** argv )
//{
//    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
//    {
//        SDL_Log( "Unable to initialized SDL: %s", SDL_GetError() );
//        return 1;
//    }
//
//    SDL_Window* Window = SDL_CreateWindow(
//        "Circular Audio Buffer Example",
//        SDL_WINDOWPOS_UNDEFINED,
//        SDL_WINDOWPOS_UNDEFINED,
//        600,
//        400,
//        0
//    );
//
//    if( !Window )
//    {
//        SDL_Log( "Unable to initialize window: %s", SDL_GetError() );
//        return 1;
//    }
//
//    SDL_Renderer* Renderer = SDL_CreateRenderer( Window, -1, 0 );
//
//    platform_audio_config AudioConfig = {};
//    AudioConfig.SamplesPerSecond = 44100;
//    AudioConfig.BytesPerSample = 2 * sizeof( Sint16 );
//    AudioConfig._sample_index = 0;
//    AudioConfig.ToneHz = 300;
//    AudioConfig.ToneVolume = 3000;
//    AudioConfig.WavePeriod = AudioConfig.SamplesPerSecond / AudioConfig.ToneHz;
//
//    AudioConfig.wav_file.openWavFile( FILE_PATH );
//    AudioConfig.wav_file.displayInformation( FILE_PATH );
//
//    //AudioConfig.wav_file.readCurrentInput();
//
//    platform_audio_buffer AudioBuffer = {};
//    AudioBuffer.Size = AudioConfig.SamplesPerSecond * AudioConfig.BytesPerSample;
//    AudioBuffer.Buffer = new Uint8[AudioBuffer.Size];
//    // Two data points per sample. One for the left speaker, one for the right.
//    AudioBuffer.ReadCursor = 0;
//    // NOTE: Offset by 1 sample in order to cause the circular buffer to
//    // initially be filled.
//    AudioBuffer.WriteCursor = AudioConfig.BytesPerSample;
//    AudioBuffer.AudioConfig = &AudioConfig;
//    std::memset( AudioBuffer.Buffer, 0, AudioBuffer.Size );
//
//    PlatformInitializeAudio( &AudioBuffer );
//
//    platform_program_state ProgramState = {};
//    ProgramState.IsRunning = true;
//
//    platform_audio_thread_context AudioThreadContext = {};
//    AudioThreadContext.AudioBuffer = &AudioBuffer;
//    AudioThreadContext.ProgramState = &ProgramState;
//    SDL_Thread* AudioThread = SDL_CreateThread( PlatformAudioThread, "Audio", (void*)&AudioThreadContext );
//
//    while( ProgramState.IsRunning )
//    {
//        while( SDL_PollEvent( &ProgramState.LastEvent ) )
//        {
//            PlatformHandleEvent( &ProgramState );
//        }
//
//        SDL_SetRenderDrawColor( Renderer, 0, 0, 0, 0 );
//        SDL_RenderClear( Renderer );
//        SDL_RenderPresent( Renderer );
//    }
//
//    SDL_WaitThread( AudioThread, NULL );
//
//    SDL_DestroyRenderer( Renderer );
//    SDL_DestroyWindow( Window );
//    SDL_CloseAudioDevice( AudioBuffer.DeviceID );
//    SDL_Quit();
//
//    delete[] AudioBuffer.Buffer;
//    return 0;
//}
