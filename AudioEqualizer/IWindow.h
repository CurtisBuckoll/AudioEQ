/*========================================================================

    Name:     IWindow.h

    Date:     May 2018

    Author:   Curtis Buckoll

    Overview:

        Defines the interface to a general window object. We do this so
    that we can more easily change the underlying implementation, if
    necessary.

========================================================================*/

#pragma once

//--------------------------------------------------------------

#include <SDL\SDL.h>

#include "Vertex.h"

//========================================================================
// IWindow Abstract Interface
class IWindow
{
public:

    //--------------------------------------------------------------
    //
    virtual ~IWindow() {};

    //--------------------------------------------------------------
    //
    virtual int getWidth() const = 0;

    //--------------------------------------------------------------
    //
    virtual int getHeight() const = 0;

    //--------------------------------------------------------------
    //
    virtual void init() = 0;

    //--------------------------------------------------------------
    //
    virtual void clearBackground() = 0;

    //--------------------------------------------------------------
    //
    virtual void setPixel( int x,
                           int y,
                           Color256 color ) = 0;

    //--------------------------------------------------------------
    //
    virtual Color256 getPixel( int x,
                               int y ) const = 0;

    //--------------------------------------------------------------
    //
    virtual void RenderFrame() = 0;

    //--------------------------------------------------------------
    //
    virtual void close() = 0;

protected:

    //--------------------------------------------------------------
    //
    IWindow() {};
};

