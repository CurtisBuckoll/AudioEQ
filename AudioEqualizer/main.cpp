// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "AudioDevice.h"
#include "Window.h"
#include "InputManager.h"
#include "EQDraw.h"
#include "Equalizer.h"

static const int kWIN_WIDTH = 600;
static const int kWIN_HEIGHT = 400;

static const int kNUM_EQ_SAMPLES = 64;
static const int kNUM_CHUNKS_TO_SAMPLE_PER_FRAME = 32;

const char const* FILE_PATH = "../resources/Q2_sample_2.wav";

// -----------------------------------------------------------------
//
int main( int argc, char** argv )
{
    // Init the main window. This will also initialize SDL.
    std::unique_ptr<IWindow> window( new Window( kWIN_WIDTH, kWIN_HEIGHT ) );
    window->init();

    // Get the audio data from file.
    WavFile wav_file( kNUM_EQ_SAMPLES );
    wav_file.openWavFile( FILE_PATH );
    wav_file.displayInformation( FILE_PATH );

    EQDraw eq_curve( *window, kNUM_EQ_SAMPLES );

    // Create our input/output eq vectors.
    std::vector<std::vector<double>> eq_samples_in( kNUM_CHUNKS_TO_SAMPLE_PER_FRAME, std::vector<double>( kNUM_EQ_SAMPLES, 0.0 ) );

    //std::vector<std::vector<double>> eq_samples_out( kNUM_CHUNKS_TO_SAMPLE_PER_FRAME, std::vector<double>( kNUM_EQ_SAMPLES, 0.0 ) );

    std::vector<std::vector<double>> eq_samples_out;

    Equalizer equalizer( kNUM_EQ_SAMPLES );

    InputManager input_manager;

    //AudioDevice audio_device();
    //audio_device.setPlayState( DEVICE_STATE::PLAY );

    size_t chunk_index = 0;

    bool running = true;
    while( input_manager.pollForEvents() )
    {
        // Set up our input data vectors.
        for( auto& chunk : eq_samples_in )
        {
            if( chunk_index >= wav_file.vectorized_audio_.size() )
            {
                // Something is broken here.. when we get out of range.
                // chunk = std::move( std::vector<double>( kNUM_EQ_SAMPLES, 0.0 ) );
                running = false;
            }
            else
            {
                chunk = std::move( wav_file.vectorized_audio_[chunk_index++] );
            }
        }

        if( !running ) break;

        eq_curve.processUserInput( input_manager.getKeys() );
        eq_curve.drawToWindow();

        equalizer.eq_chunks( eq_samples_in, eq_samples_out );

        // Need to then reset the out data:
        eq_samples_out.clear();
        
        window->RenderFrame();
    }


    window->close();

    return 0;
}

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
//
//    AudioDevice audio_device;
//    audio_device.setPlayState( DEVICE_STATE::PLAY );
//
//
//    bool running = true;
//    while( running )
//    {
//        SDL_Event e;
//        while( SDL_PollEvent( &e ) )
//        {
//            if( e.type == SDL_QUIT )
//            {
//                running = false;
//            }
//        }
//
//        SDL_SetRenderDrawColor( Renderer, 0, 0, 0, 0 );
//        SDL_RenderClear( Renderer );
//        SDL_RenderPresent( Renderer );
//    }
//
//    audio_device.terminate();
//
//    SDL_DestroyRenderer( Renderer );
//    SDL_DestroyWindow( Window );
//
//    SDL_Quit();
//
//
//    return 0;
//}