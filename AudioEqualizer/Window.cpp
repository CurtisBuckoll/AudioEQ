/*========================================================================

    Name:     Window.cpp

    Date:     June 2017

    Author:   Curtis Buckoll

    Overview:

        Implements an interface to an SDL window object.

========================================================================*/

#include "stdafx.h"

#include "Window.h"

#include <iostream>

//========================================================================
//
Window::Window( int width, int height )
    : _window( nullptr )
    , _renderer( nullptr )
    , _texture( nullptr )
    , _width( width )
    , _height( height )
    , _title( "Window" )
{

}

//========================================================================
//
Window::Window( int width, int height, std::string title )
    : _window( nullptr )
    , _renderer( nullptr )
    , _texture( nullptr )
    , _width( width )
    , _height( height )
    , _title( title )
{

}

//========================================================================
//
Window::~Window()
{
    SDL_DestroyRenderer( _renderer );
    SDL_DestroyWindow( _window );
}

//========================================================================
//
void Window::init()
{
    SDL_Init( SDL_INIT_EVERYTHING );

	_window = SDL_CreateWindow( _title.c_str(), 
								SDL_WINDOWPOS_CENTERED, 
		                        SDL_WINDOWPOS_CENTERED, 
		                        _width, 
			                    _height, 
                                SDL_WINDOW_SHOWN );

	_renderer = SDL_CreateRenderer( _window, 
                                    -1, 
                                    SDL_RENDERER_ACCELERATED );

    _texture = SDL_CreateTexture( _renderer, 
                                  SDL_PIXELFORMAT_ARGB8888, 
                                  SDL_TEXTUREACCESS_STREAMING, 
                                  _width, 
                                  _height );

    _pixels.resize( _width * _height * 4, 0 );

    SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 0 );
	SDL_RenderClear(_renderer);
}

//========================================================================
//
void Window::clearBackground()
{
    SDL_SetRenderDrawColor( _renderer, 0, 0, 0, 0 );
    SDL_RenderClear( _renderer );
}

//========================================================================
//
void Window::RenderFrame()
{
    SDL_SetRenderDrawColor( _renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
    SDL_RenderClear( _renderer );

    SDL_UpdateTexture( _texture, nullptr, &_pixels[0], _width * 4 );
    SDL_RenderCopy( _renderer, _texture, nullptr, nullptr );
    SDL_RenderPresent( _renderer );

    memset( &_pixels[0], 0, _width * _height * 4 );
}

//========================================================================
//
void Window::setPixel( int x, int y, Color256 color )
{
    int Y = -( y - _height + 1 );
    int offset = ( _width * 4 * Y ) + x * 4;

    if( static_cast<size_t>( offset ) >= _pixels.size() )
	{
        //std::cout << "setPixel of range: " << offset << " " << _pixels.size() << std::endl;
        return;
	}

	_pixels[offset]		= color.b;      
	_pixels[offset + 1] = color.g;       
	_pixels[offset + 2] = color.r;       
	_pixels[offset + 3] = SDL_ALPHA_OPAQUE;
}

//========================================================================
//
Color256 Window::getPixel( int x,
                           int y ) const
{
    int Y = -( y - _height + 1 );
    int offset = ( _width * 4 * Y ) + x * 4;

    Color256 pixel( 0, 0, 0 );

    if( static_cast<size_t>( offset ) >= _pixels.size() )
    {
        //std::cout << "getPixel of range: " << offset << " " << _pixels.size() << std::endl;
        return Color256( 0, 0, 0 );
    }

    return Color256( _pixels[offset + 2], _pixels[offset + 1], _pixels[offset] );
}

//========================================================================
//
int Window::getWidth() const
{
	return _width;
}

//========================================================================
//
int Window::getHeight() const
{
	return _height;
}

//========================================================================
//
void Window::close()
{
    SDL_DestroyRenderer( _renderer );
    SDL_DestroyWindow( _window );
}
