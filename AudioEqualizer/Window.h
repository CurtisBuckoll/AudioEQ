/*========================================================================

    Name:     Window.h

    Date:     June 2017

    Author:   Curtis Buckoll

    Overview:

        Declaration of the Window class which manages a handle to
    an SDL window object. Provides methods to open a window and
    manipulate pixel values. Implements the IWindow interface.

========================================================================*/

#pragma once

#include "IWindow.h"
#include "Vertex.h"

#include <SDL\SDL.h>
#include <vector>
#include <string>

//========================================================================
// Window
class Window : public IWindow
{
public:

    //--------------------------------------------------------------
    //
    Window( int width, 
            int height );

    //--------------------------------------------------------------
    //
    Window( int width,
            int height,
            std::string title );

    //--------------------------------------------------------------
    //
	~Window();

    //--------------------------------------------------------------
    //
    int getWidth() const override;

    //--------------------------------------------------------------
    //
    int getHeight() const override;

    //--------------------------------------------------------------
    //
	void init() override;

    //--------------------------------------------------------------
    //
	void clearBackground() override;

    //--------------------------------------------------------------
    //
    void setPixel( int x,
                   int y,
                   Color256 color ) override;

    //--------------------------------------------------------------
    //
    Color256 getPixel( int x,
                       int y ) const override;

    //--------------------------------------------------------------
    //
	void RenderFrame() override;

    //--------------------------------------------------------------
    //
    void close() override;


private:

    //==============================================================
    // Private Data Members
    //==============================================================

    SDL_Window*   _window;
    SDL_Renderer* _renderer;
    SDL_Texture*  _texture;

    std::vector<unsigned char> _pixels;

    int         _width;
    int         _height;
    std::string _title;
};

