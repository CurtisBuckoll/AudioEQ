// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "AudioDevice.h"
#include "Window.h"
#include "InputManager.h"
#include "EQDraw.h"
#include "Equalizer.h"

#include "DFT.h"
#include "EqualizerDFT.h"
#include "EqualizerDCT.h"
#include "EqualizerFFT.h"

// -----------------------------------------------------------------
//

static const int kWIN_WIDTH = 600;
static const int kWIN_HEIGHT = 400;
static const int kNUM_EQ_SAMPLES = 256;

std::vector<std::string> wav_files = { "../resources/sine_sweep.wav",
                                       "../resources/white_noise.wav",
                                       "../resources/sample_piano.wav",
                                       "../resources/Q2_sample_2.wav" };

const char const* FILE_PATH = wav_files[1].c_str();

void test();

//========================================================================
//
int main( int argc, char** argv )
{
    // =================================================================
    // Add test code in here, comment out for regular execution.
    //test();
    // =================================================================
    

    // Init the main window. This will also initialize SDL.
    std::unique_ptr<IWindow> window( new Window( kWIN_WIDTH, kWIN_HEIGHT ) );
    window->init();

    // Get the audio data from file.
    WavFile wav_file( kNUM_EQ_SAMPLES );
    wav_file.openWavFile( FILE_PATH );
    wav_file.displayInformation( FILE_PATH );

    EQDraw eq_curve( *window, kNUM_EQ_SAMPLES );

    // Create our input/output eq vectors.
    //std::vector<std::vector<double>> eq_samples_in( kNUM_CHUNKS_TO_SAMPLE_PER_FRAME, std::vector<double>( kNUM_EQ_SAMPLES, 0.0 ) );

    //std::vector<std::vector<double>> eq_samples_out( kNUM_CHUNKS_TO_SAMPLE_PER_FRAME, std::vector<double>( kNUM_EQ_SAMPLES, 0.0 ) );

    //std::vector<std::vector<double>> eq_samples_out;

    Equalizer equalizer( kNUM_EQ_SAMPLES );

    InputManager input_manager;

    EqualizerDFT dft( kNUM_EQ_SAMPLES );
    EqualizerDCT dct( kNUM_EQ_SAMPLES );
    EqualizerFFT fft( kNUM_EQ_SAMPLES );

    AudioDevice audio_device( std::move(wav_file._vectorized_audio), kNUM_EQ_SAMPLES, dft, eq_curve.getEqCoeffsBuffer() );
    audio_device.setPlayState( DEVICE_STATE::PLAY );

    size_t chunk_index = 0;

    bool running = true;
    while( input_manager.pollForEvents() )
    {
        // Set up our input data vectors.
        //for( auto& chunk : eq_samples_in )
        //{
        //    if( chunk_index >= wav_file._vectorized_audio.size() )
        //    {
        //        chunk = std::move( std::vector<double>( kNUM_EQ_SAMPLES, 0.0 ) );
        //        running = false;
        //    }
        //    else
        //    {
        //        chunk = std::move( wav_file._vectorized_audio[chunk_index++] );
        //    }
        //}

        //if( !running ) break;

        eq_curve.processUserInput( input_manager.getKeys() );

        std::vector<double> audio_spectrum;
        audio_device.getFrequencySpectrum( audio_spectrum );

        eq_curve.drawSpectrumTowindow( audio_spectrum );
        eq_curve.drawToWindow();

        //equalizer.eq_chunks( eq_samples_in, eq_samples_out );

        //audio_device.addToBuffer( eq_samples_out );

        // Need to then reset the out data:
        //eq_samples_out.clear();
        
        window->RenderFrame();
    }

    audio_device.terminate();
    window->close();

    return 0;
}

//========================================================================
//
void test()
{
    std::cout << "=== COMPLEX CLASS ===============================================\n" << std::endl;

    Math::Complex z;
    z.test();

    std::cout << "\n=== DFT MATRIX ==================================================\n" << std::endl;

    DFT dft( 4 );
    dft.test();

    std::getchar();
    exit( 0 );
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