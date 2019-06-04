// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "AudioDevice.h"
#include "Window.h"
#include "InputManager.h"
#include "EQDraw.h"

static const int kWIN_WIDTH = 600;
static const int kWIN_HEIGHT = 400;

static const int NUM_EQ_SAMPLES = 64;

// -----------------------------------------------------------------
//
int main( int argc, char** argv )
{
    // Init the main window. This will also initialize SDL.
    std::unique_ptr<IWindow> window( new Window( kWIN_WIDTH, kWIN_HEIGHT ) );
    window->init();

    EQDraw eq_curve( *window, NUM_EQ_SAMPLES );

    InputManager input_manager;

    while( input_manager.pollForEvents() )
    {
        eq_curve.processUserInput( input_manager.getKeys() );
        eq_curve.drawToWindow();
        
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