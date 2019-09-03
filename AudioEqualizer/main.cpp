// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "AudioDevice.h"
#include "Window.h"
#include "InputManager.h"
#include "FrameRateLimiter.h"
#include "EQDraw.h"
#include "Equalizer.h"

#include "DFT.h"
#include "EqualizerDFT.h"
#include "EqualizerDCT.h"
#include "EqualizerFFT.h"


// -----------------------------------------------------------------
// TODO: If we want, we can parameterize execution with a parameter file.
// Some parameters to consider:
//
// eq type (dct, dft, fft)
// num eq samples
// frame rate
// whether to print fps or not
// the file to play
// enter test mode
// -----------------------------------------------------------------

// -----------------------------------------------------------------
//
static const int kWIN_WIDTH = 600;
static const int kWIN_HEIGHT = 400;
static const int kNUM_EQ_SAMPLES = 256;

// -----------------------------------------------------------------
//
std::vector<std::string> wav_files = { "../resources/sine_sweep.wav",
                                       "../resources/white_noise.wav",
                                       "../resources/sample_piano.wav",
                                       "../resources/Q2_sample_2.wav" };

// -----------------------------------------------------------------
//
const char const* FILE_PATH = wav_files[1].c_str();

// -----------------------------------------------------------------
// Forward declare.
void test();

//========================================================================
//
int main( int argc, char** argv )
{
    // =================================================================
    // Test mode execution:
    // test();
    // =================================================================

    // Init the main window. This will also initialize SDL.
    std::unique_ptr<IWindow> window( new Window( kWIN_WIDTH, kWIN_HEIGHT ) );
    window->init();

    // Get the audio data from file.
    WavFile wav_file( kNUM_EQ_SAMPLES );
    wav_file.openWavFile( FILE_PATH );
    wav_file.displayInformation( FILE_PATH );

    EQDraw eq_curve( *window, kNUM_EQ_SAMPLES );

    Equalizer equalizer( kNUM_EQ_SAMPLES );

    InputManager input_manager;
    FrameRateLimiter frame_rate_limiter( 60, 1.0, 60 );

    EqualizerDFT dft( kNUM_EQ_SAMPLES );
    EqualizerDCT dct( kNUM_EQ_SAMPLES );
    EqualizerFFT fft( kNUM_EQ_SAMPLES );

    AudioDevice audio_device( std::move(wav_file._vectorized_audio), kNUM_EQ_SAMPLES, fft, eq_curve.getEqCoeffsBuffer() );
    audio_device.setPlayState( DEVICE_STATE::PLAY );

    size_t chunk_index = 0;

    bool running = true;
    while( input_manager.pollForEvents() )
    {
        frame_rate_limiter.setStartFrame();

        eq_curve.processUserInput( input_manager.getKeys() );

        std::vector<double> audio_spectrum;
        audio_device.getFrequencySpectrum( audio_spectrum );

        eq_curve.drawSpectrumTowindow( audio_spectrum );
        eq_curve.drawToWindow();

        audio_device.switchAnyalyzer( input_manager.getKeys() );

        frame_rate_limiter.LimitFPS();
        frame_rate_limiter.printFPS();

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
